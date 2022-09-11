#include "stdafx.h"
#include "BattleUnitAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"

BattleUnitAgent::BattleUnitAgent(Elite::Vector2 pos) :BattleUnitAgent{ pos, { 0.8f , 0.8f , 0.8f} }
{
	m_pSeek = new Seek{};
	m_pFace = new Face{};
}

BattleUnitAgent::BattleUnitAgent(Elite::Vector2 pos, Elite::Color color)
{
	m_BodyColor = color;
	m_DefaultColor = color;
	m_pSeek = new Seek{};
	m_pFace = new Face{};
	SetPosition(pos);
	SetMass(0.f);

	m_pRigidBody->RemoveAllShapes();

}

BattleUnitAgent::~BattleUnitAgent()
{
	SAFE_DELETE(m_pSeek);
	SAFE_DELETE(m_pFace);
}

void BattleUnitAgent::Update(float dt)
{

	SteeringAgent::Update(dt);
	//if (Elite::Distance(GetPosition(), m_pSeek->GetTarget().Position) < 0.2f)
	//{
	//	SetLinearVelocity(GetLinearVelocity() * Elite::Vector2{ 0.5f,0.5f });
	//	m_pFace->SetTarget(m_pSeek->GetTarget().Position);
	//	SetSteeringBehavior(m_pFace);
	//}
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

	m_pSeek->SetTarget(pos);
	SetSteeringBehavior(m_pSeek);
}


void BattleUnitAgent::RotateTowards(Elite::Vector2 pos)
{

	m_pFace->SetTarget(pos);
	SetSteeringBehavior(m_pFace);
}


void BattleUnitAgent::ResetColor()
{
	SetBodyColor(m_DefaultColor);
}
