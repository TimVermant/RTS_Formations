#pragma once
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"

//--- Includes and Forward Declarations ---
#include "projects/Movement/SteeringBehaviors/SteeringHelpers.h"
#include "framework\EliteAI\EliteGraphs\EGraph2D.h"
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphEditor.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphRenderer.h"
#include "projects/Formations/BattleUnitAgent.h"
#include "projects/Formations/Formation.h"

class App_RTSFormations :public IApp
{
public:

	App_RTSFormations() = default;
	virtual ~App_RTSFormations() final;

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void UpdateUI();
	void Render(float deltaTime) const override;


private:
	float m_TrimWorldSize = 70.f;

	Formation* m_pFormation;

	std::vector<BattleUnitAgent*> m_pUnits;

	// Formation parameters
	int m_NrOfUnits = 10;

	float m_HorizontalDistanceUnits, m_VerticalDistanceUnits;
	float m_VerticalLineDistance;
};