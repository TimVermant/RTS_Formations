#pragma once

#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"

class BattleUnitAgent: public SteeringAgent
{
public:
	//--- Constructor & Destructor ---
	BattleUnitAgent(Elite::Vector2 pos);
	BattleUnitAgent(Elite::Vector2 pos, Elite::Color color);
	virtual ~BattleUnitAgent();

	//--- Agent Functions ---
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;

	//-- Agario Functions --
	void MarkForDestroy();
	bool CanBeDestroyed();

	void MoveTowards(Elite::Vector2 pos);

	void ResetColor();


	const float GetDetectRadius() { return m_DetectRadius; } // Detect radius of agent that gets updated on upgrade

private:
	
	Elite::Color m_DefaultColor;

	bool m_ToDestroy = false;
	float m_SpeedBase = 25.f;
	float m_DetectRadius = 10.0f;

	//Only needs seek
	ISteeringBehavior* m_pSeek = nullptr;

private:
	//C++ make the class non-copyable
	BattleUnitAgent(const BattleUnitAgent&) {};
	BattleUnitAgent& operator=(const BattleUnitAgent&) {};
};