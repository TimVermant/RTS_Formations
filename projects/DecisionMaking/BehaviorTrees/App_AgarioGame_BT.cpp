#include "stdafx.h"
#include "App_AgarioGame_BT.h"
#include "Behaviors.h"

#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioContactListener.h"
#include "projects/Shared/NavigationColliderElement.h"

using namespace Elite;
App_AgarioGame_BT::App_AgarioGame_BT()
{
}

App_AgarioGame_BT::~App_AgarioGame_BT()
{
	for (auto& f : m_pFoodVec)
	{
		SAFE_DELETE(f);
	}
	m_pFoodVec.clear();

	for (auto& a : m_pAgentVec)
	{
		SAFE_DELETE(a);
	}
	m_pAgentVec.clear();

	SAFE_DELETE(m_pContactListener);
	SAFE_DELETE(m_pUberAgent);

	for (auto pNC : m_vNavigationColliders)
		SAFE_DELETE(pNC);
	m_vNavigationColliders.clear();
}

void App_AgarioGame_BT::Start()
{
	//Create Boundaries
	const float blockSize{ 2.0f };
	const float hBlockSize{ blockSize / 2.0f };
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(-m_TrimWorldSize - hBlockSize, 0.f), blockSize, (m_TrimWorldSize + blockSize) * 2.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(m_TrimWorldSize + hBlockSize, 0.f), blockSize, (m_TrimWorldSize + blockSize) * 2.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(0.0f, m_TrimWorldSize + hBlockSize), m_TrimWorldSize * 2.0f, blockSize));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(0.0f, -m_TrimWorldSize - hBlockSize), m_TrimWorldSize * 2.0f, blockSize));


	//Creating the world contact listener that informs us of collisions
	m_pContactListener = new AgarioContactListener();

	//Create food items
	m_pFoodVec.reserve(m_AmountOfFood);
	for (int i = 0; i < m_AmountOfFood; i++)
	{
		Elite::Vector2 randomPos = randomVector2(-m_TrimWorldSize, m_TrimWorldSize);
		m_pFoodVec.push_back(new AgarioFood(randomPos));
	}

	//Create default agents
	m_pAgentVec.reserve(m_AmountOfAgents);
	for (int i = 0; i < m_AmountOfAgents; i++)
	{
		Vector2 randomPos = randomVector2(-m_TrimWorldSize, m_TrimWorldSize);
		AgarioAgent* newAgent = new AgarioAgent(randomPos);

		//1. Create Blackboard
		Elite::Blackboard* pBlackBoard = CreateBlackboard(newAgent); // Creates blackboard and adds some default values to it

		//2. Create BehaviorTree
		Elite::BehaviorTree* pBehaviorTree = new Elite::BehaviorTree(pBlackBoard,
			new BehaviorAction(ChangeToWander));


		//3. Set the BehaviorTree active on the agent 
		newAgent->SetDecisionMaking(pBehaviorTree);



		m_pAgentVec.push_back(newAgent);
	}


	//-------------------
	//Create The Uber Agent
	//-------------------
	Elite::Vector2 randomPos = randomVector2(-m_TrimWorldSize, m_TrimWorldSize);
	Color customColor = Color{ randomFloat(), randomFloat(), randomFloat() };
	m_pUberAgent = new AgarioAgent(randomPos, customColor);

	//Create and add the necessary blackboard data
	Elite::Blackboard* pBlackBoard = CreateBlackboard(m_pUberAgent); // Creates blackboard and adds some default values to it

		//2. Create BehaviorTree

	Elite::BehaviorTree* pBehaviorTree = new Elite::BehaviorTree(pBlackBoard,
		new BehaviorSelector(
			{
new BehaviorSequence(

					{
						new BehaviorConditional(IsBiggerEnemyClose),
new BehaviorAction(ChangeToFlee)
					}),
				new BehaviorSequence(

					{
						new BehaviorConditional(IsFoodCloseBy),
new BehaviorAction(ChangeToSeek)
					}),
				new BehaviorAction(ChangeToWander)
			}
	));


	//3. Set the BehaviorTree active on the agent 
	m_pUberAgent->SetDecisionMaking(pBehaviorTree);


}

void App_AgarioGame_BT::Update(float deltaTime)
{
	UpdateImGui();

	//Check if agent is still alive
	if (m_pUberAgent->CanBeDestroyed())
	{
		m_GameOver = true;
		return;
	}
	//Update the custom agent
	m_pUberAgent->Update(deltaTime);

	//Update the other agents and food
	UpdateAgarioEntities(m_pFoodVec, deltaTime);
	UpdateAgarioEntities(m_pAgentVec, deltaTime);


	//Check if we need to spawn new food
	m_TimeSinceLastFoodSpawn += deltaTime;
	if (m_TimeSinceLastFoodSpawn > m_FoodSpawnDelay)
	{
		m_TimeSinceLastFoodSpawn = 0.f;
		m_pFoodVec.push_back(new AgarioFood(randomVector2(-m_TrimWorldSize, m_TrimWorldSize)));
	}
}

void App_AgarioGame_BT::Render(float deltaTime) const
{
	std::vector<Elite::Vector2> points =
	{
		{ -m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, -m_TrimWorldSize },
		{ -m_TrimWorldSize, -m_TrimWorldSize }
	};
	DEBUGRENDERER2D->DrawPolygon(&points[0], 4, { 1,0,0,1 }, 0.4f);

	for (AgarioFood* f : m_pFoodVec)
	{
		f->Render(deltaTime);
	}

	for (AgarioAgent* a : m_pAgentVec)
	{
		a->Render(deltaTime);
	}

	m_pUberAgent->Render(deltaTime);
}

Blackboard* App_AgarioGame_BT::CreateBlackboard(AgarioAgent* a)
{
	Elite::Blackboard* pBlackboard = new Elite::Blackboard();
	pBlackboard->AddData("Agent", a);
	pBlackboard->AddData("AgentsVec", &m_pAgentVec);
	pBlackboard->AddData("FoodVec", &m_pFoodVec);
	pBlackboard->AddData("WorldSize", m_TrimWorldSize);
	pBlackboard->AddData("Target", Elite::Vector2{});
	pBlackboard->AddData("AgentFleeTarget", static_cast<AgarioAgent*>(nullptr)); // Needs the cast for the type
	pBlackboard->AddData("Time", 0.0f);

	return pBlackboard;
}

void App_AgarioGame_BT::UpdateImGui()
{
	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		//Setup
		int menuWidth = 150;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
		ImGui::Begin("Agario", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);
		ImGui::SetWindowFocus();
		ImGui::PushItemWidth(70);
		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
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
		ImGui::Spacing();

		ImGui::Text("Agent Info");
		ImGui::Text("Radius: %.1f", m_pUberAgent->GetRadius());
		ImGui::Text("Survive Time: %.1f", TIMER->GetTotal());

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
	if (m_GameOver)
	{
		//Setup
		int menuWidth = 300;
		int menuHeight = 100;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2(width / 2.0f - menuWidth, height / 2.0f - menuHeight));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
		ImGui::Begin("Game Over", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::Text("Final Agent Info");
		ImGui::Text("Radius: %.1f", m_pUberAgent->GetRadius());
		ImGui::Text("Survive Time: %.1f", TIMER->GetTotal());
		ImGui::End();
	}
#pragma endregion
#endif

}
