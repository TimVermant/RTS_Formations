#include "stdafx.h"
#include "SandboxAgent.h"

using namespace Elite;

SandboxAgent::SandboxAgent() : BaseAgent()
{
	m_Target = GetPosition();
}

void SandboxAgent::Update(float dt)
{
	//Orientation
	const auto angle = Orientation(GetRotation(), -GetLinearVelocity());
	m_pRigidBody->SetTransform(Transform(GetPosition(), Elite::Vector2(angle, angle)));

	// Variables
	const float maxSpeed = 50.f;
	const float arrivalRadius = 1.f;
	const float slowRadius = 15.f;

	Vector2 toTarget = m_Target - GetPosition();
	const float distance = toTarget.Magnitude();
	if (distance < arrivalRadius)
	{
		SetLinearVelocity(ZeroVector2);
		return;
	}

	Vector2 velocity = toTarget;
	velocity.Normalize();
	if (distance < slowRadius)
	{
		velocity *= maxSpeed * distance / slowRadius;
	}
	else
	{
		velocity *= maxSpeed;
	}
	SetLinearVelocity(velocity);
}

void SandboxAgent::Render(float dt)
{
	BaseAgent::Render(dt); //Default Agent Rendering
}

float SandboxAgent::Orientation(float currentOrientation, Elite::Vector2 velocity) const
{
	//Determine angle based on direction
	if (velocity.Magnitude() > 0)
	{
		velocity.Normalize();
		return atan2(-velocity.x, velocity.y);
	}
	return currentOrientation;
}