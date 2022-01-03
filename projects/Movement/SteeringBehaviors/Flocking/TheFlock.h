#pragma once
#include "../SteeringHelpers.h"
#include "FlockingSteeringBehaviors.h"
#include "../SpacePartitioning/SpacePartitioning.h"

class ISteeringBehavior;
class SteeringAgent;
class BlendedSteering;
class PrioritySteering;




class Flock
{
public:
	Flock(
		int flockSize = 50, 
		float worldSize = 100.f, 
		SteeringAgent* pAgentToEvade = nullptr, 
		bool trimWorld = false);

	~Flock();

	void Update(float deltaT);
	void UpdateAndRenderUI() ;
	void Render(float deltaT);


	//const vector<SteeringAgent*>& GetNeighbors() const { return m_pPartitionedSpace->GetNeighbors(); }
	//int GetNrOfNeighbors() const { return m_pPartitionedSpace->GetNrOfNeighbors(); }


	void RegisterNeighbors(SteeringAgent* pAgent);
	int GetNrOfNeighbors() const;
	const vector<SteeringAgent*>& GetNeighbors() const;


	Elite::Vector2 GetAverageNeighborPos() const;
	Elite::Vector2 GetAverageNeighborVelocity() const;

	void SetSeekTarget(TargetData target);
	void SetWorldTrimSize(float size) { m_WorldSize = size; }

	bool IsUsingPartitioning() const { return m_UsePartitioning; };
	CellSpace* GetPartitionedSpace() const { return m_pPartitionedSpace; };

private:
	//Datamembers
	int m_FlockSize = 0;
	vector<SteeringAgent*> m_Agents;
	

	CellSpace* m_pPartitionedSpace = nullptr;
	int m_NrOfCellsX{ 25 };

	vector<Elite::Vector2> m_OldPositions = {};


	vector<SteeringAgent*> m_Neighbors;


	bool m_TrimWorld = false;
	float m_WorldSize = 0.f, m_MaxWorldSize = 100.f;

	float m_NeighborhoodRadius = 10.f;
	int m_NrOfNeighbors = 0;

	SteeringAgent* m_pAgentToEvade = nullptr;
	
	//Steering Behaviors
	Seperation* m_pSeparationBehavior = nullptr;
	Cohesion* m_pCohesionBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Wander* m_pWanderBehavior = nullptr;
	Evade* m_pEvadeBehavior = nullptr;

	BlendedSteering* m_pBlendedSteering = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;

	//weight variables
	float m_SeperationWeight;
	float m_AlignmentWeight;
	float m_CohesionWeight;
	

	//debug rendering booleans
	bool m_EnableDebugRendering;
	bool m_DrawNeighbourRadius;
	bool m_DrawNeighbours;

	bool m_UsePartitioning;

	float* GetWeight(ISteeringBehavior* pBehaviour);

private:
	Flock(const Flock& other);
	Flock& operator=(const Flock& other);
};