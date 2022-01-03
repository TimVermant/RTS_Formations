#include "stdafx.h"
#include "BattleUnitAgent.h"

BattleUnitAgent::BattleUnitAgent(Elite::Vector2 pos) :BattleUnitAgent{ pos, { 0.8f , 0.8f , 0.8f} }
{
}

BattleUnitAgent::BattleUnitAgent(Elite::Vector2 pos, Elite::Color color)
{
	m_BodyColor = color;

	SetPosition(pos);
	SetMass(0.f);
}

BattleUnitAgent::~BattleUnitAgent()
{

}

void BattleUnitAgent::Update(float dt)
{
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
