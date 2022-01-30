#include "stdafx.h"
#include "SpacePartitioning.h"
#include "projects\Movement\SteeringBehaviors\SteeringAgent.h"

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
{
	boundingBox.bottomLeft = { left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

bool Cell::IsPosInCell(const Elite::Vector2 pos) const
{
	if (pos.x > boundingBox.bottomLeft.x && pos.x < boundingBox.bottomLeft.x + boundingBox.width
		&& pos.y > boundingBox.bottomLeft.y && pos.y < boundingBox.bottomLeft.y + boundingBox.height)
	{
		return true;
	}
	return false;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_Neighbors(maxEntities)
	, m_NrOfNeighbors(0)
{
	int currRow{ 0 }, currCol{ 0 };
	float cellWidth{ width / rows }, cellHeight{ height / cols };
	for (int i{ 0 }; i < maxEntities; i++)
	{
		float left = 0.f + (currRow * cellWidth);
		float bottom = 0.f + (currCol * cellHeight);
		m_Cells.push_back(Cell{ left,bottom,cellWidth,cellHeight });
		++currRow;
		if (currRow >= rows)
		{
			++currCol;
			currRow = 0;
		}
	}
}

void CellSpace::AddAgent(SteeringAgent* agent)
{
	int index = PositionToIndex(agent->GetPosition());
	if (index != -1)
	{
		m_Cells[index].agents.push_back(AgentStruct{ agent,agent->GetPosition() });
	}
}


void CellSpace::UpdateAllAgentCells()
{

	for (size_t i{ 0 }; i < m_Cells.size(); i++)
	{
		for (auto it{ m_Cells[i].agents.begin() }; it != m_Cells[i].agents.end();)
		{
			int oldIndex = i;
			int newIndex = PositionToIndex(it->pAgent->GetPosition());
			if (oldIndex != newIndex && newIndex != -1 && oldIndex != -1)
			{
				//auto oldAgents = m_Cells[oldIndex].agents;

				AddAgent(it->pAgent);
				it = m_Cells[oldIndex].agents.erase(it);
				//m_Cells[oldIndex].agents.erase(std::remove(m_Cells[oldIndex].agents.begin(), m_Cells[oldIndex].agents.end(), oldAgent));
				//m_Cells[newIndex].agents.push_back(AgentStruct{ it->pAgent,agent->GetPosition() });
			}
			else
			{

				++it;
			}
			//UpdateAgentCell(agent.pAgent, agent.oldPos);


		}
	}
}

void CellSpace::UpdateAgentCell(SteeringAgent* agent, Elite::Vector2 oldPos)
{
	int oldIndex = PositionToIndex(oldPos);
	int newIndex = PositionToIndex(agent->GetPosition());
	if (oldIndex != newIndex && newIndex != -1 && oldIndex != -1)
	{
		//auto oldAgents = m_Cells[oldIndex].agents;
		auto oldAgent = AgentStruct{ agent,oldPos };
		m_Cells[oldIndex].agents.erase(std::find(m_Cells[oldIndex].agents.begin(), m_Cells[oldIndex].agents.end(), oldAgent));
		//m_Cells[oldIndex].agents.erase(std::remove(m_Cells[oldIndex].agents.begin(), m_Cells[oldIndex].agents.end(), oldAgent));
		m_Cells[newIndex].agents.push_back(AgentStruct{ agent,agent->GetPosition() });
	}
}

void CellSpace::RegisterNeighbors(SteeringAgent* agent, float queryRadius)
{
	m_NrOfNeighbors = 0;
	m_Neighbors.resize(0);
	//Find out which cells are in the agent's neighbourhood

	std::vector<AgentStruct> agentsFromCells;
	Elite::Rect agentRect;
	agentRect.width = queryRadius * 2;
	agentRect.height = agentRect.width;
	agentRect.bottomLeft = agent->GetPosition();
	agentRect.bottomLeft.x -= queryRadius;
	agentRect.bottomLeft.y -= queryRadius;
	for (const auto& cell : m_Cells)
	{
		Elite::Rect cellRect{ cell.boundingBox };
		if (Elite::IsOverlapping(cellRect, agentRect))
		{
			for (const auto& cellAgent : cell.agents)
			{
				agentsFromCells.push_back(cellAgent);
			}
		}
	}
	//Get all the agents from those cells

	//From those agents, find out which ones are within our neighborhood radius
	auto agentPos = agent->GetPosition();
	for (const auto& agentsToCheck : agentsFromCells)
	{
		auto otherAgentPos = agentsToCheck.pAgent->GetPosition();
		if (Elite::Distance(agentPos, otherAgentPos) < queryRadius)
		{
			m_Neighbors.push_back(agent);
			m_NrOfNeighbors++;
		}
	}

}

void CellSpace::RenderCells() const
{

	for (const auto& cell : m_Cells)
	{
		Elite::Polygon square = Elite::Polygon{ cell.GetRectPoints() };
		DEBUGRENDERER2D->DrawPolygon(&square, Elite::Color{ 1.f,0.f,0.f,1.f }, 1.f);

		DEBUGRENDERER2D->DrawString(square.GetCenterPoint(), std::to_string(cell.agents.size()).c_str());
	}


}

int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	for (size_t i{ 0 }; i < m_Cells.size(); i++)
	{
		if (m_Cells[i].IsPosInCell(pos))
		{
			return i;
		}

	}
	return -1;
}