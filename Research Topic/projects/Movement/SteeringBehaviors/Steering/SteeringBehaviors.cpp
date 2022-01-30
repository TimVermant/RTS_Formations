//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"


using namespace Elite;

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};


	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition(); // Vector from pAgent to m_Target
	steering.LinearVelocity.Normalize(); // length = 1
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0.f,1.f,0.f,1.f });
	}

	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};



	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= -pAgent->GetMaxLinearSpeed();


	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0.f,1.f,0.f,1.f });
	}
	return steering;
}

//ARRIVE

SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();

	//const float arrivalDistance = m_SlowRadius;
	//const float distance = steering.LinearVelocity.Magnitude();
	steering.LinearVelocity.Normalize();
	if (m_TargetRadius < m_SlowRadius)
	{
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed() * m_TargetRadius / m_SlowRadius;
	}
	else
	{
		steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
	}
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, m_SlowRadius, Elite::Color{ 0.f,1.f,0.f,1.f });
	}

	return steering;
}


//FACE


SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	float orientation = pAgent->GetOrientation();
	Vector2 directionCurrent = Vector2{ cos(orientation) ,sin(orientation) };
	auto directionDesired = m_Target.Position - pAgent->GetPosition();

	float arrivalRadius = 0.001f;


	float angle = Dot(directionCurrent, directionDesired);
	angle /= directionCurrent.Magnitude();
	angle /= directionDesired.Magnitude();

	pAgent->SetAutoOrient(false);
	if (angle > arrivalRadius || angle < -arrivalRadius)
	{
		steering.AngularVelocity += pAgent->GetMaxAngularSpeed() * angle;
	}
	return steering;
}


// WANDER
SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	//TODO: Fix same direction
	// Give wander angle instead of orientation
//	float orientation = pAgent->GetOrientation() - Elite::ToRadians(90.f);
	float orientation = m_WanderAngle;
	Vector2 directionCurrent = Vector2{ cos(orientation) ,sin(orientation) };

	float distance = 5.f;
	float radius = 2.f;

	// calculate random angle (max angle change thing like 5 degrees)
	Vector2 newPosition = directionCurrent + distance * directionCurrent.GetNormalized() + pAgent->GetPosition();

	m_WanderAngle += Elite::randomFloat(-m_MaxAngleChange, m_MaxAngleChange);

	Vector2 newDirection = Vector2{ cos(m_WanderAngle) ,sin(m_WanderAngle) } *radius + newPosition;

	m_Target = newDirection;
	steering = Seek::CalculateSteering(deltaT, pAgent);

	if (pAgent->CanRenderBehavior())
	{
		/*DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), directionCurrent, distance, Elite::Color{ 0.f,1.f,0.f,1.f });
		DEBUGRENDERER2D->DrawCircle(newPosition, radius, Elite::Color{ 1.0f,0.f,0.f,1.f }, -1.f);
		DEBUGRENDERER2D->DrawPoint(newDirection, 3.f, Elite::Color{ 0.f,0.f,1.f,1.f }, -1.f);*/
	}
	return steering;
}

// PURSUIT
SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	// calculate agent velocity and predict place after few seconds
	float distanceInFrames = 10.f;
	Vector2 predictPos = m_Target.Position + m_Target.LinearVelocity * distanceInFrames;

	steering.LinearVelocity = predictPos - pAgent->GetPosition(); // Vector from pAgent to m_Target
	steering.LinearVelocity.Normalize(); // length = 1
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();



	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(predictPos, 3.f, Elite::Color{ 0.f,1.f,0.f,1.f });
	}
	return steering;
}

// EVADE
SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	// calculate agent velocity and predict place after few seconds
	float distanceInFrames = 10.f;
	float behaviourRadius = 10.f;


	Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	if (behaviourRadius * behaviourRadius < (toTarget.MagnitudeSquared()))
	{
		steering.IsValid = false;
		return steering;
	}
	Vector2 predictPos = m_Target.Position + m_Target.LinearVelocity * distanceInFrames;



	steering.LinearVelocity = predictPos - pAgent->GetPosition(); // Vector from pAgent to m_Target
	steering.LinearVelocity.Normalize(); // length = 1
	steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
	steering.LinearVelocity *= -1.f;


	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawPoint(predictPos, 3.f, Elite::Color{ 0.f,1.f,0.f,1.f });
	}
	return steering;
}
