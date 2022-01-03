#include "stdafx.h"
#include "TheFlock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"

//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/,
	float worldSize /*= 100.f*/,
	SteeringAgent* pAgentToEvade /*= nullptr*/,
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld{ trimWorld }
	, m_pAgentToEvade{ pAgentToEvade }
	, m_NeighborhoodRadius{ 15 }
	, m_NrOfNeighbors{ 0 }
	, m_SeperationWeight{ 0.25f }
	, m_CohesionWeight{ 0.25f }
	, m_AlignmentWeight{ 0.25f }
	, m_EnableDebugRendering{ false }
	, m_DrawNeighbourRadius{ false }
	, m_DrawNeighbours{ false }
	, m_UsePartitioning{ false }

{
	// TODO: initialize the flock and the memory pool

	int rows{ 10 }, cols{ 10 };
	m_pPartitionedSpace = new CellSpace{ m_WorldSize,m_WorldSize,rows,cols,rows * cols };

	// Behaviours
	m_pSeparationBehavior = new Seperation{ this };
	m_pCohesionBehavior = new Cohesion{ this };
	m_pVelMatchBehavior = new VelocityMatch{ this };

	m_pSeekBehavior = new Seek{};
	m_pWanderBehavior = new Wander{};
	m_pEvadeBehavior = new Evade{};

	std::vector<BlendedSteering::WeightedBehavior> behaviourList{};
	behaviourList.push_back(BlendedSteering::WeightedBehavior{ m_pSeparationBehavior,m_SeperationWeight });
	behaviourList.push_back(BlendedSteering::WeightedBehavior{ m_pCohesionBehavior,m_CohesionWeight });
	behaviourList.push_back(BlendedSteering::WeightedBehavior{ m_pVelMatchBehavior,m_AlignmentWeight });


	m_pBlendedSteering = new BlendedSteering{ behaviourList };

	m_pAgentToEvade = new SteeringAgent{};
	m_pAgentToEvade->SetPosition(Elite::Vector2{ Elite::randomFloat(static_cast<float>(m_WorldSize)), Elite::randomFloat(static_cast<float>(m_WorldSize)) });
	m_pAgentToEvade->SetSteeringBehavior(m_pWanderBehavior);
	m_pAgentToEvade->SetAutoOrient(true);
	m_pAgentToEvade->SetMaxLinearSpeed(5.f);


	m_pEvadeBehavior->SetTarget(m_pAgentToEvade->GetPosition());
	// Priority steering
	vector<ISteeringBehavior*> priorityBehaviors{ m_pEvadeBehavior,m_pBlendedSteering };

	m_pPrioritySteering = new PrioritySteering{ priorityBehaviors };


	m_Agents.resize(m_FlockSize);
	for (int i{ 0 }; i < m_FlockSize; i++)
	{
		m_Agents[i] = new SteeringAgent{};
		m_Agents[i]->SetPosition(Elite::Vector2{ Elite::randomFloat(static_cast<float>(m_WorldSize)), Elite::randomFloat(static_cast<float>(m_WorldSize)) });
		m_Agents[i]->SetSteeringBehavior(m_pPrioritySteering);
		m_Agents[i]->SetAutoOrient(true);

		m_pPartitionedSpace->AddAgent(m_Agents[i]);
	}
	m_Agents[0]->SetRenderBehavior(true);

}

Flock::~Flock()
{
	// TODO: clean up any additional data
	SAFE_DELETE(m_pSeparationBehavior);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pVelMatchBehavior);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pWanderBehavior);
	SAFE_DELETE(m_pEvadeBehavior);


	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);

	SAFE_DELETE(m_pAgentToEvade);

	SAFE_DELETE(m_pPartitionedSpace);

	for (auto pAgent : m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();

	m_Neighbors.clear();
}

void Flock::Update(float deltaT)
{

	// TODO: update the flock
	// loop over all the agents
		// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
		// update it				(-> the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)
		// trim it to the world

	RegisterNeighbors(m_pAgentToEvade);

	m_pAgentToEvade->Update(deltaT);
	if (m_TrimWorld)
	{
		Elite::Vector2 botLeft{ m_MaxWorldSize - m_WorldSize,m_MaxWorldSize - m_WorldSize }, topRight{ m_WorldSize,m_WorldSize };
		m_pAgentToEvade->TrimToWorld(botLeft, topRight);
	}
	m_pEvadeBehavior->SetTarget(m_pAgentToEvade->GetPosition());
	for (auto agent : m_Agents)
	{


	//	std::cout << agent->GetPosition().x << " - " << agent->GetPosition().y << std::endl;

		RegisterNeighbors(agent);

		agent->Update(deltaT);
		if (m_TrimWorld)
		{
			Elite::Vector2 botLeft{ m_MaxWorldSize - m_WorldSize,m_MaxWorldSize - m_WorldSize }, topRight{ m_WorldSize,m_WorldSize };
			agent->TrimToWorld(botLeft, topRight);
		}

	}
	if (m_UsePartitioning)
	{
		m_pPartitionedSpace->UpdateAllAgentCells();
	}
}

void Flock::Render(float deltaT)
{
	// TODO: render the flock
	//for (auto agent : m_Agents)
	//{
	//	agent->Render(deltaT);
	//}

	// Debug
	if (m_Agents[0]->CanRenderBehavior() && m_EnableDebugRendering)
	{

		if (m_DrawNeighbourRadius)
		{
			DEBUGRENDERER2D->DrawCircle(m_Agents[0]->GetPosition(), m_NeighborhoodRadius, Elite::Color{ 1.f,1.f,1.f,1.f }, -1);
		}
		if (m_DrawNeighbours)
		{
			RegisterNeighbors(m_Agents[0]);

			for (auto neighbour : m_Neighbors)
			{
				if (neighbour != m_Agents[0])
				{
					DEBUGRENDERER2D->DrawSolidCircle(neighbour->GetPosition(), 1.f, Elite::Vector2{}, Elite::Color{ 0.f,1.f,0.f,1.f });
				}
			}
		}
		if (m_UsePartitioning)
		{
			m_pPartitionedSpace->RenderCells();
		}
	}

}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	ImGui::Checkbox("Debug Rendering", &m_EnableDebugRendering);
	ImGui::Checkbox("DEBUG: Neighbourhood radius", &m_DrawNeighbourRadius);
	ImGui::Checkbox("DEBUG: Neighbours", &m_DrawNeighbours);
	ImGui::Checkbox("Use partitioning", &m_UsePartitioning);
	ImGui::Checkbox("Trim world", &m_TrimWorld);
	ImGui::SliderFloat("Trim Size", &m_WorldSize, 50.f, m_MaxWorldSize);
	ImGui::Spacing();
	// TODO: Implement checkboxes for debug rendering and weight sliders here
	ImGui::Text("Behaviour weights");
	ImGui::SliderFloat("Seperation Weight", &m_pBlendedSteering->GetWeightedBehaviorsRef().at(0).weight, 0.f, 1.f);
	ImGui::SliderFloat("Aligmnent Weight", &m_pBlendedSteering->GetWeightedBehaviorsRef().at(1).weight, 0.f, 1.f);
	ImGui::SliderFloat("Cohesion Weight", &m_pBlendedSteering->GetWeightedBehaviorsRef().at(2).weight, 0.f, 1.f);

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();

}

void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	m_NrOfNeighbors = 0;
	m_Neighbors.resize(0);
	if (m_UsePartitioning)
	{

		m_pPartitionedSpace->RegisterNeighbors(pAgent, m_NeighborhoodRadius);

		m_Neighbors = GetNeighbors();

	}
	else
	{


		// Update the container and neighbor count (resetting the neighbor count and overwriting the variables already in the container)
		auto agentPos = pAgent->GetPosition();
		for (auto agent : m_Agents)
		{
			// position compare
			auto otherAgentPos = agent->GetPosition();
			if (Elite::Distance(agentPos, otherAgentPos) < m_NeighborhoodRadius)
			{
				m_Neighbors.push_back(agent);
				m_NrOfNeighbors++;
			}
		}
	}
}

int Flock::GetNrOfNeighbors() const
{
	if (m_UsePartitioning)
	{
		return m_pPartitionedSpace->GetNrOfNeighbors();
	}
	else
	{
		return m_NrOfNeighbors;
	}

}

const vector<SteeringAgent*>& Flock::GetNeighbors() const
{
	if (m_UsePartitioning)
	{
		return m_pPartitionedSpace->GetNeighbors();
	}
	else
	{
		return m_Neighbors;
	}
}

Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	Elite::Vector2 averagePos{}, totalPos{};
	for (auto neighbor : GetNeighbors())
	{
		totalPos += neighbor->GetPosition();
	}
	averagePos = totalPos / static_cast<float>(GetNrOfNeighbors());
	return averagePos;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	Elite::Vector2 averageVel{}, totalVel{};
	for (auto neighbor : GetNeighbors())
	{
		totalVel += neighbor->GetLinearVelocity();
	}
	averageVel = totalVel / static_cast<float>(GetNrOfNeighbors());
	return averageVel;
}

void Flock::SetSeekTarget(TargetData target)
{
	// TODO: set target for Seek behavior
	m_pSeekBehavior->SetTarget(target);

}

float* Flock::GetWeight(ISteeringBehavior* pBehavior)
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if (it != weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}
