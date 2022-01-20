#include "stdafx.h"
#include "BattleUnitAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

BattleUnitAgent::BattleUnitAgent(Elite::Vector2 pos) :BattleUnitAgent{ pos, { 0.8f , 0.8f , 0.8f} }
{
}

BattleUnitAgent::BattleUnitAgent(Elite::Vector2 pos, Elite::Color color)
{
	m_BodyColor = color;

	SetPosition(pos);
	SetMass(1.f);
	
}

BattleUnitAgent::~BattleUnitAgent()
{
	SAFE_DELETE(m_pSeek);
}

void BattleUnitAgent::Update(float dt)
{
	SteeringAgent::Update(dt);
}

void BattleUnitAgent::Render(float dt)
{
	SteeringAgent::Render(dt);
}

void BattleUnitAgent::MarkForDestroy()
{
	m_ToDestroy = true;
}

bool BattleUnitAgent::CanBeDestroyed()
{
	return m_ToDestroy;
}

void BattleUnitAgent::MoveTowards(Elite::Vector2 pos)
{
	m_pSeek = new Seek{};
	m_pSeek->SetTarget(pos);
	SetSteeringBehavior(m_pSeek);
}
