#ifndef STEERINGBEHAVIORS_APPLICATION_H
#define STEERINGBEHAVIORS_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
#include "SteeringBehaviors.h"
class SteeringAgent;
class Obstacle;

//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_SteeringBehaviors final : public IApp
{
public:
	//Constructor & Destructor
	App_SteeringBehaviors() = default;
	virtual ~App_SteeringBehaviors();

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:

	enum class BehaviorTypes :int
	{
		Seek,
		Flee,
		Arrive,
		Face,
		Wander,
		Evade,
		Pursuit,

		//@end
		Count
	};

	struct ImGui_Agent
	{
		SteeringAgent* pAgent = nullptr;
		ISteeringBehavior* pBehavior = nullptr;
		int SelectedBehavior = int(BehaviorTypes::Wander);
		int SelectedTarget = -1;
	};

	//Datamembers
	std::vector<ImGui_Agent> m_AgentVec = {};
	std::vector<string> m_TargetLabelsVec = {};
	TargetData m_Target = {};
	bool m_VisualizeTarget = true;
	bool m_IsInitialized = false;
	bool m_TrimWorld = true;
	float m_TrimWorldSize = 50.f;
	int m_AgentToRemove = -1;

	std::vector<Obstacle*> m_Obstacles;
	const float m_MaxObstacleRadius = 5.f;
	const float m_MinObstacleRadius = 1.f;
	const float m_MinObstacleDistance = 10.f;

	//Interface Functions
	void RemoveAgent(UINT index);
	ImGui_Agent App_SteeringBehaviors::AddAgent(BehaviorTypes behaviorType = BehaviorTypes::Wander, int targetId = -1, bool autoOrient = true, float mass = 1.f, float maxSpd = 7.f);
	void SetAgentBehavior(ImGui_Agent& a);
	void UpdateTarget(ImGui_Agent& a);
	void UpdateTargetLabel();

	void AddObstacle();
	Elite::Vector2 GetRandomObstaclePosition(float obstacleRadius, bool& positionFound);

	//C++ make the class non-copyable
	App_SteeringBehaviors(const App_SteeringBehaviors&) = delete;
	App_SteeringBehaviors& operator=(const App_SteeringBehaviors&) = delete;
};
#endif