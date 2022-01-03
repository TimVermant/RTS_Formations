/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteMath/EMath.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteBehaviorTree/EBehaviorTree.h"
#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------


// ACTIONS
// **********
Elite::BehaviorState ChangeToWander(Elite::Blackboard* pBlackboard)
{
	AgarioAgent* pAgent{ nullptr };
	pBlackboard->GetData("Agent", pAgent);


	if (!pAgent)
		return Elite::BehaviorState::Failure;

	pAgent->SetToWander();
	return Elite::BehaviorState::Success;
}


Elite::BehaviorState ChangeToSeek(Elite::Blackboard* pBlackboard)
{
	AgarioAgent* pAgent{ nullptr };
	pBlackboard->GetData("Agent", pAgent);
	Elite::Vector2 target = Elite::Vector2();
	pBlackboard->GetData("Target", target); // Remember to set the target in your condition
	if (!pAgent)
		return Elite::BehaviorState::Failure;

	pAgent->SetToSeek(target);
	return Elite::BehaviorState::Success;
}

Elite::BehaviorState ChangeToFlee(Elite::Blackboard* pBlackboard)
{
	AgarioAgent* pAgent{ nullptr };
	pBlackboard->GetData("Agent", pAgent);
	Elite::Vector2 target = Elite::Vector2();
	pBlackboard->GetData("Target", target); // Remember to set the target in your condition
	if (!pAgent)
		return Elite::BehaviorState::Failure;

	pAgent->SetToFlee(target);
	return Elite::BehaviorState::Success;
}


// CONDITIONS
// **********


bool IsFoodCloseBy(Elite::Blackboard* pBlackboard)
{
	AgarioAgent* pAgent{ nullptr };
	pBlackboard->GetData("Agent", pAgent);

	std::vector<AgarioFood*>* foods;
	pBlackboard->GetData("FoodVec", foods);

	//OPTIONAL: CHECK IF DATA IS OKAY
	if (foods->size() < 1)
	{
		return false;
	}


	//1. Get the closest food
	AgarioFood* closestFood = (*foods)[0];
	float closestDistance{ DistanceSquared(pAgent->GetPosition(),(*foods)[0]->GetPosition()) };
	for (size_t i = 1; i < foods->size(); i++)
	{
		// calculate the distance squared between agent and the current food
		float distanceSquared = DistanceSquared(pAgent->GetPosition(), (*foods)[i]->GetPosition());
		if (distanceSquared < closestDistance) // if it is smaller than my previous closest food 
		{
			closestFood = (*foods)[i];
		}
	}

	//2. Check if closest food is in a certain range
	const float range{ 200.0f };
	if (closestDistance < range * range)
	{
		pBlackboard->ChangeData("Target", closestFood->GetPosition());
		return true;
	}

	return false;
}


bool IsBiggerEnemyClose(Elite::Blackboard* pBlackboard)
{
	AgarioAgent* pAgent{ nullptr };
	pBlackboard->GetData("Agent", pAgent);

	std::vector<AgarioAgent*>* pEnemies;
	pBlackboard->GetData("AgentsVec", pEnemies);


	if (pEnemies->size() < 1)
		return false;

	
	AgarioAgent* pClosestAgent = nullptr;
	float closestDistanceSquared = DistanceSquared(pAgent->GetPosition(), (*pEnemies)[0]->GetPosition());
	for (size_t i = 1; i < pEnemies->size(); i++)
	{
		// calculate the distance squared between agent and the current food
		float distanceSquared = DistanceSquared(pAgent->GetPosition(), (*pEnemies)[i]->GetPosition());
		if (distanceSquared < closestDistanceSquared && (*pEnemies)[i]->GetRadius() < pAgent->GetRadius()) // if it is smaller than my previous closest agent and the agent is not smaller
		{
			pClosestAgent = (*pEnemies)[i];
		}

	}
	if (pClosestAgent == nullptr)
		return false;
	//2. Check if closest food is in a certain range
	const float range{ 50.f };
	if (closestDistanceSquared < range * range)
	{
		pBlackboard->ChangeData("Target", pClosestAgent->GetPosition());
		return true;
	}

	return false;
}





#endif