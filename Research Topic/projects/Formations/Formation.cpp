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

	// Resets/recalculates the closest unit
	if (m_pLeaderUnit)
		m_pLeaderUnit->ResetColor();
	// Calculates unit that will become the leader unit
	m_pLeaderUnit = GetClosestUnit(trimWorldSize);


	auto it = std::find(m_pUnits.begin(), m_pUnits.end(), m_pLeaderUnit);
	std::iter_swap(m_pUnits.begin(), it);
	m_pUnits[0]->SetBodyColor(Elite::Color{ 1.0f,0.f,0.f,1.f });

	CalculateDesiredFormationPositions();



}

void Formation::CreateFormation(float trimWorldSize, Elite::Vector2 startPosition)
{

	// Resets/recalculates the closest unit
	if (m_pLeaderUnit)
		m_pLeaderUnit->ResetColor();
	m_pLeaderUnit = GetClosestUnit(trimWorldSize, startPosition);


	auto it = std::find(m_pUnits.begin(), m_pUnits.end(), m_pLeaderUnit);
	std::iter_swap(m_pUnits.begin(), it);
	m_pUnits[0]->SetBodyColor(Elite::Color{ 1.0f,0.f,0.f,1.f });

	CalculateDesiredFormationPositions();



}

void Formation::AddUnit(BattleUnitAgent* pUnit)
{
	m_pUnits.push_back(pUnit);

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

bool& Formation::GetRetainLeader()
{
	return m_bRetainLeaderUnit;
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
	m_StartPosition = { lowestX + highestX,highestY };
	if (m_bRetainLeaderUnit)
		return m_pLeaderUnit;
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
	if (pLeader == nullptr)
		return m_pLeaderUnit;

	return pLeader;
}

BattleUnitAgent* Formation::GetClosestUnit(float trimWorldSize, Elite::Vector2 startPosition)
{

	m_StartPosition = startPosition;
	if (m_bRetainLeaderUnit)
		return m_pLeaderUnit;
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
	if (pLeader == nullptr)
		return m_pLeaderUnit;

	return pLeader;
}






void Formation::CalculateDesiredFormationPositions()
{
	Elite::Vector2 position{};
	Elite::Vector2 startPosition = m_StartPosition;
	int offsetMultiplier;
	size_t index = 0;

	//RotateLeadUnit();
	for (size_t i{ 0 }; i < m_FormationMaxSize / m_UnitsPerLine; i++)
	{

		//// Rotation


		//auto orientation = m_pLeaderUnit->GetOrientation();
		//auto linearVelocity = m_pLeaderUnit->GetLinearVelocity().GetNormalized();
		//Elite::Vector2 orientationVector = Elite::Vector2{ std::cosf(orientation),std::sinf(orientation) };

		////Calculate angle between orientation/linearvelocity and add future angle
		//float angle = std::acosf(Elite::Dot(orientationVector, linearVelocity) / (orientationVector.Magnitude() * linearVelocity.Magnitude())) + orientation;


		//float newX = orientationVector.x * cosf(angle) - orientationVector.y * sinf(angle);
		//float newY = orientationVector.x * sinf(angle) + orientationVector.y * cosf(angle);

		//std::cout << newX << " - " << newY << std::endl;







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
			Elite::Vector2 tempPos{ position.x,position.y};
			// Because rotation isnt around origin
			tempPos -= m_StartPosition;

			auto leadOrientation = m_pLeaderUnit->GetOrientation();
			auto unitOrientation = m_pUnits[index]->GetOrientation();
			
			float angle = AngleBetweenVectors(
				Elite::Vector2{ std::cosf(leadOrientation),std::sinf(leadOrientation) }, 
				Elite::Vector2{ std::cosf(unitOrientation),std::sinf(unitOrientation) } 
			);

		

			//// Rotate it
			float newX = tempPos.x * cosf(angle) - tempPos.y * sinf(angle);
			float newY = tempPos.x * sinf(angle) + tempPos.y * cosf(angle);
			tempPos = { newX,newY };

			// Translate it back 
			tempPos += m_StartPosition;

			m_pUnits[index]->MoveTowards(tempPos);
			index++;
			
		}
	 }
}

void Formation::RotateLeadUnit()
{
	Elite::Vector2 desiredDirection = m_StartPosition - m_pLeaderUnit->GetPosition();
	Elite::Vector2 currentDirection = Elite::Vector2{ std::cosf(m_pLeaderUnit->GetOrientation()),std::sinf(m_pLeaderUnit->GetOrientation()) };
	float angle = AngleBetweenVectors(desiredDirection, currentDirection);
	std::cout << angle << " - " << m_pLeaderUnit->GetRotation() << std::endl;
	m_pLeaderUnit->SetRotation(angle);
	
}

float Formation::AngleBetweenVectors(Elite::Vector2 A, Elite::Vector2 B)
{
	float angle = Elite::Dot(A, B) / (A.Magnitude() * B.Magnitude());
	return acosf(angle);
}



