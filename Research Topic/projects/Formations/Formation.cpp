#include "stdafx.h"
#include "Formation.h"
#include "BattleUnitAgent.h"

Formation::Formation(size_t nrOfUnits) :
	m_FormationMaxSize{ nrOfUnits },
	m_UnitsPerLine{ 9 },
	m_DistanceBetweenUnitsOnLine{ 4.f },
	m_VerticalDistanceBetweenUnitsOnLine{ 3.f },
	m_DistanceBetweenLines{ 5.f }
{



}

Formation::~Formation()
{
	for (auto it : m_pUnits)
	{

		SAFE_DELETE(it);
	}
	m_pLeaderUnit = nullptr;
}

void Formation::Update(float dt)
{
	for (auto pUnit : m_pUnits)
	{
		pUnit->Update(dt);
	}
}

void Formation::Render(float dt)
{
	for (auto unit : m_pUnits)
	{
		unit->Render(dt);
	}
}

void Formation::CreateFormation(float trimWorldSize)
{


	if (m_pLeaderUnit)
		m_pLeaderUnit->ResetColor();
	// Calculates unit that will become the leader unit
	m_pLeaderUnit = GetClosestUnit(trimWorldSize);
	// Put the leader unit in the front of the list
	auto it = std::find(m_pUnits.begin(), m_pUnits.end(), m_pLeaderUnit);
	std::iter_swap(m_pUnits.begin(), it);
	m_pUnits[0]->SetBodyColor(Elite::Color{ 1.0f,0.f,0.f,1.f });

	CalculateDesiredFormationPositions();

	/*for (size_t i{ 0 }; i < m_pUnits.size(); i++)
	{
		m_pUnits[i]->SetPosition(m_FormationPositions[i]);
	}*/

}

void Formation::AddUnit(BattleUnitAgent* pUnit)
{
	m_pUnits.push_back(pUnit);

}

void Formation::SetStartPosition(const Elite::Vector2& startPos)
{
	m_StartPosition = startPos;
	m_UseMouseStartPos = true;
}




int& Formation::GetUnitsPerLine()
{
	return m_UnitsPerLine;
}

float& Formation::GetDistanceBetweenUnitsOnLine()
{
	return m_DistanceBetweenUnitsOnLine;
}

float& Formation::GetVerticalDistanceBetweenUnitsOnLine()
{
	return m_VerticalDistanceBetweenUnitsOnLine;
}

float& Formation::GetDistanceBetweenLines()
{
	return m_DistanceBetweenLines;
}

BattleUnitAgent* Formation::GetClosestUnit(float trimWorldSize)
{
	// Calculate desired leader position
	float lowestX{ trimWorldSize }, highestX{ -trimWorldSize }, highestY{ -trimWorldSize };
	for (auto pUnit : m_pUnits)
	{
		auto pos = pUnit->GetPosition();
		if (pos.x < lowestX)
		{
			lowestX = pos.x;
		}
		else if (pos.x > highestX)
		{
			highestX = pos.x;
		}

		if (pos.y > highestY)
		{
			highestY = pos.y;
		}
	}
	// Find unit closest to leaderpos
	if(!m_UseMouseStartPos)
		m_StartPosition = { lowestX + highestX,highestY };
	float distance = trimWorldSize * trimWorldSize;
	BattleUnitAgent* pLeader = nullptr;
	for (auto pUnit : m_pUnits)
	{
		// Check squared distance to find closest point
		if (distance > pUnit->GetPosition().DistanceSquared(m_StartPosition))
		{
			distance = pUnit->GetPosition().DistanceSquared(m_StartPosition);
			pLeader = pUnit;
		}

	}

	return pLeader;
}

void Formation::CalculateDesiredFormationPositions()
{
	Elite::Vector2 position{};
	Elite::Vector2 startPosition = m_StartPosition;
	int offsetMultiplier;
	size_t index = 0;
	for (size_t i{ 0 }; i < m_FormationMaxSize / m_UnitsPerLine; i++)
	{
		auto orientation = m_pLeaderUnit->GetOrientation();
		auto linearVelocity = m_pLeaderUnit->GetLinearVelocity().GetNormalized();
		Elite::Vector2 orientationVector = Elite::Vector2{std::cosf(orientation),std::sinf(orientation)};

		//Calculate angle between orientation/linearvelocity and add future angle
		float angle = std::acosf(Elite::Dot(orientationVector, linearVelocity) / (orientationVector.Magnitude()* linearVelocity.Magnitude())) + orientation;



		Elite::Vector3 forward{ std::cos(angle),std::sin(angle), 0.f };
		Elite::Vector3 up{ 0.f,0.f,1.f };
		Elite::Vector3 right = Elite::Cross(forward, up);

		//// To put units behind leader
		forward *= -1.f;

		// Get middle front position of unit on line
		startPosition = m_StartPosition;
		startPosition.y -= m_DistanceBetweenLines * i;
		m_pUnits[index]->MoveTowards(startPosition);
		index++;
		offsetMultiplier = 1;
		for (size_t j{ 1 }; j < (size_t)m_UnitsPerLine; j++) // start at index 1 because we already calculated the first position
		{
			// Reset to start position on line
			position = startPosition;
			float xOffset = offsetMultiplier * m_DistanceBetweenUnitsOnLine;
			float yOffset = offsetMultiplier * m_VerticalDistanceBetweenUnitsOnLine;
			if (j % 2 != 0) // Odd number
			{
				position.x -= xOffset;
				position.y -= yOffset;

			}
			else // Even number
			{
				position.x += xOffset;
				position.y -= yOffset;
				offsetMultiplier++; // After odd - even we increase the offsetmultiplier

			}


			// Doesnt work right now
			/*Elite::Vector3 tempPos{ position.x,position.y,0.f };
			tempPos = tempPos - forward * yOffset + right * xOffset;
			position = { tempPos.x,tempPos.y };*/



			m_pUnits[index]->MoveTowards(position);
			index++;
			//m_FormationPositions.push_back(position);
		}
	}
}

