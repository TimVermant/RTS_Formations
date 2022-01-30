/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"

//------------
//---STATES---
//------------
// A state that makes the agent go wander
class WanderState : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackBoard) override
	{
		AgarioAgent* pAgent{ nullptr };
		bool succes = pBlackBoard->GetData("Agent", pAgent);

		if (!succes)
			return;

		pAgent->SetToWander();
	};
private:
	float m_Timer = 0.0f;
};



class FleeState : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackBoard) override
	{
		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		AgarioAgent* pEnemyAgent{ nullptr };
		assert(pBlackBoard->GetData("EnemyAgents", pEnemyAgent));


		pAgent->SetToFlee(pEnemyAgent->GetPosition());
	}
private:
	float m_Timer = 0.0f;
};

class SeekAgentState : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackBoard) override
	{
		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		AgarioAgent* pEnemyAgent{ nullptr };
		assert(pBlackBoard->GetData("EnemyAgents", pEnemyAgent));


		pAgent->SetToSeek(pEnemyAgent->GetPosition());
	}
private:
	float m_Timer = 0.0f;
};


class SeekFoodState : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackBoard) override
	{
		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		std::vector<AgarioFood*>* foodVec{ nullptr };
		assert(pBlackBoard->GetData("FoodVec", foodVec));

		float detectRadius{};
		assert(pBlackBoard->GetData("DetectRadius", detectRadius));

		float radiusSquared = detectRadius * detectRadius;
		float shortestDistanceSquared = -1;
		Elite::Vector2 foodPos{};
		bool foodInRange = false;
		for (auto food : *foodVec)
		{
			Elite::Vector2 distanceVector = pAgent->GetPosition() - food->GetPosition();

			float magnitudeSquared = distanceVector.MagnitudeSquared();
			if (magnitudeSquared <= radiusSquared)
			{
				if (shortestDistanceSquared == -1)
				{
					shortestDistanceSquared = magnitudeSquared;
					foodPos = food->GetPosition();
					foodInRange = true;
					continue;
				}
				// Check if distance squared is closer then previous food in radius 
				if (shortestDistanceSquared > magnitudeSquared)
				{
					shortestDistanceSquared = magnitudeSquared;
					foodPos = food->GetPosition();
					foodInRange = true;
				}
			}
		}
		if (!foodInRange)
			return;


		pAgent->SetToSeek(foodPos);
	}
private:
	float m_Timer = 0.0f;
};


class EvadeBorderState : public Elite::FSMState
{
public:
	virtual void OnEnter(Elite::Blackboard* pBlackBoard) override
	{
		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		float trimWorldSize{};
		assert(pBlackBoard->GetData("trimWorldSize", trimWorldSize));

	/*	float distanceFromBorder{};
		assert(pBlackBoard->GetData("distanceFromBorder", distanceFromBorder));*/

		float distanceFromBorder = pAgent->GetDetectRadius();
		Elite::Vector2 agentPos = pAgent->GetPosition();
		Elite::Vector2 fleePos{agentPos};
		bool isFleeing = false;
		//Right border 
		if (trimWorldSize - agentPos.x < distanceFromBorder)
		{
			fleePos.x = trimWorldSize;
			isFleeing = true;
		}
		//Left border
		else if (-trimWorldSize - agentPos.x < distanceFromBorder)
		{
			fleePos.x = -trimWorldSize;
			isFleeing = true;

		}
		//Top border
		if (trimWorldSize - agentPos.y < distanceFromBorder)
		{
			fleePos.y = trimWorldSize;
			isFleeing = true;
		}
		else if (-trimWorldSize - agentPos.y < distanceFromBorder)
		{
			fleePos.y = -trimWorldSize;
			isFleeing = true;
		}


		if (!isFleeing)
			return;
		pAgent->SetToFlee(fleePos);

	}
private:
	float m_Timer = 0.0f;
};

//-----------------
//---TRANSITIONS---
//-----------------


class BorderNearbyTransition : public Elite::FSMTransition
{
public:

	virtual bool ToTransition(Elite::Blackboard* pBlackBoard) const override
	{

		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		float trimWorldSize{};
		assert(pBlackBoard->GetData("TrimWorldSize", trimWorldSize));


		float distanceFromBorder = pAgent->GetDetectRadius();
		Elite::Vector2 agentPos = pAgent->GetPosition();
		Elite::Vector2 fleePos{ agentPos };
		bool isFleeing = false;
		//Right border 
		if (trimWorldSize - agentPos.x < distanceFromBorder)
		{
			fleePos.x = trimWorldSize;
			return true;
		}
		//Left border
		else if (-trimWorldSize - agentPos.x < distanceFromBorder)
		{
			fleePos.x = -trimWorldSize;
			return true;

		}
		//Top border
		if (trimWorldSize - agentPos.y < distanceFromBorder)
		{
			fleePos.y = trimWorldSize;
			return true;
		}
		else if (-trimWorldSize - agentPos.y < distanceFromBorder)
		{
			fleePos.y = -trimWorldSize;
			return true;
		}
		return false;
	}

private:

};



class SmallEnemyNearbyTransition : public Elite::FSMTransition
{
public:

	virtual bool ToTransition(Elite::Blackboard* pBlackBoard) const override
	{

		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		std::vector<AgarioAgent*>* pEnemyAgents{ nullptr };
		assert(pBlackBoard->GetData("EnemyAgents", pEnemyAgents));

	

		Elite::Vector2 agentPosition = pAgent->GetPosition();
		float radiusSquared = pAgent->GetDetectRadius() * pAgent->GetDetectRadius();
		for (auto enemy : *pEnemyAgents)
		{

			if (enemy->GetRadius() > pAgent->GetRadius()) // Check if enemy is bigger then pAgent
				continue;

			float distanceSquared = (enemy->GetPosition() - agentPosition).MagnitudeSquared();
			float enemyRadiusSquared = enemy->GetRadius() * enemy->GetRadius();
			if (distanceSquared < radiusSquared + enemyRadiusSquared) // Check with squared distance/radius if enemy inside player radius
				return true;
		}

		return false;
	}

private:

};


class BigEnemyNearbyTransition : public Elite::FSMTransition
{
public:

	virtual bool ToTransition(Elite::Blackboard* pBlackBoard) const override
	{

		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		std::vector<AgarioAgent*>* pEnemyAgents{ nullptr };
		assert(pBlackBoard->GetData("EnemyAgents", pEnemyAgents));

		

		Elite::Vector2 agentPosition = pAgent->GetPosition();
		float radiusSquared = pAgent->GetDetectRadius() * pAgent->GetDetectRadius();
	
		for (auto enemy : *pEnemyAgents)
		{

			if (enemy->GetRadius() < pAgent->GetRadius()) // Check if enemy is bigger then pAgent
				continue;

			float distanceSquared = (enemy->GetPosition() - agentPosition).MagnitudeSquared();
			float enemyRadiusSquared = enemy->GetRadius() * enemy->GetRadius();
			if (distanceSquared < radiusSquared + enemyRadiusSquared) // Check with squared distance/radius if enemy inside player radius
				return true;
		}

		return false;
	}

private:

};

class FoodNearbyTransition : public Elite::FSMTransition
{
public:

	virtual bool ToTransition(Elite::Blackboard* pBlackBoard) const override
	{

		AgarioAgent* pAgent{ nullptr };
		assert(pBlackBoard->GetData("Agent", pAgent));

		std::vector<AgarioFood*>* foodVec{ nullptr };
		assert(pBlackBoard->GetData("FoodVec", foodVec));




		float radiusSquared = pAgent->GetDetectRadius() * pAgent->GetDetectRadius();

		for (auto food : *foodVec)
		{
			Elite::Vector2 distanceVector = pAgent->GetPosition() - food->GetPosition();
			float distanceSquared = distanceVector.MagnitudeSquared();
			if (distanceSquared <= radiusSquared)
				return true;
		}



		return false;
	}

private:

};

#endif