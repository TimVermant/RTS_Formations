/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../SteeringHelpers.h"
class SteeringAgent;
class Obstacle;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

/////////////////////////
//FLEE
//****
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Flee Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Arrive behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetSlowRadius(const float slowRadius) { m_SlowRadius = slowRadius; };
	void SetTargetRadius(const float targetRadius) { m_TargetRadius = targetRadius; };

protected:
	float m_SlowRadius = 3.f;
	float m_TargetRadius = 1.f;
};

class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() = default;

	//Face behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};


class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetWanderAngle(float angle) { m_WanderAngle = angle; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; };

protected:
	float m_OffsetDistance = 6.f; // Offset (Agent Direction)
	float m_Radius = 4.f; //WanderRadius
	float m_MaxAngleChange = Elite::ToRadians(45); //Max WanderAngle change per frame
	float m_WanderAngle = 0.f; //Internal
};

class Pursuit : public Seek
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

};


class Evade : public Seek
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

};
#endif


