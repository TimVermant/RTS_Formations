#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "TheFlock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"

//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	//Move towards the neighbourhood center
	m_Target = m_pFlock->GetAverageNeighborPos();
	steering = Seek::CalculateSteering(deltaT, pAgent);
	return steering;
}


//*********************
//SEPARATION (FLOCKING)
SteeringOutput Seperation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	// get distance from neighbours as weight 
	// call blended steering flee for every neighbour
	SteeringOutput steering{};
	auto agentPos = pAgent->GetPosition();
	Elite::Vector2 targetPos = agentPos;

	for (auto neighbor : m_pFlock->GetNeighbors())
	{
		auto neighborPos = neighbor->GetPosition();
		Elite::Vector2 toVector = agentPos - neighborPos;
		auto distance = (toVector.MagnitudeSquared());
		if (distance != 0)
		{
			toVector /= distance;
		}
		targetPos += toVector;

	}

	m_Target = targetPos;
	steering = Seek::CalculateSteering(deltaT, pAgent);
	return steering;
}


//*************************
//VELOCITY MATCH (FLOCKING)

SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	Elite::Vector2 avgVelocity{};
	for (auto neighbor : m_pFlock->GetNeighbors())
	{
		avgVelocity += neighbor->GetLinearVelocity();

	}
	avgVelocity /= static_cast<float>(m_pFlock->GetNrOfNeighbors());

	steering.LinearVelocity = avgVelocity;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
	return steering;
}
