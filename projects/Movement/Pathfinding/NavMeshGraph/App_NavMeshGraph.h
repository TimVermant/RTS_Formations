#ifndef INFLUENCE_MAP_APPLICATION_H
#define INFLUENCE_MAP_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/Movement/SteeringBehaviors/SteeringHelpers.h"

#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphRenderer.h"
#include "framework\EliteAI\EliteNavigation\Algorithms\EPathSmoothing.h"

class NavigationColliderElement;
class SteeringAgent;
class Seek;
class Arrive;

namespace Elite
{
	class NavGraph;
}
//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_NavMeshGraph final : public IApp
{
public:
	//Constructor & Destructor
	App_NavMeshGraph() = default;
	virtual ~App_NavMeshGraph();

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	//Datamembers
	// --Agents--
	SteeringAgent* m_pAgent = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Arrive* m_pArriveBehavior = nullptr;
	TargetData m_Target = {};
	float m_AgentRadius = 1.0f;
	float m_AgentSpeed = 16.0f;

	// --Level--
	std::vector<NavigationColliderElement*> m_vNavigationColliders = {};

	// --Pathfinder--
	std::vector<Elite::Vector2> m_vPath;

	// --Graph--
	Elite::NavGraph* m_pNavGraph = nullptr;
	Elite::GraphRenderer m_GraphRenderer{};

	// --Debug drawing information--
	std::vector<Elite::Portal> m_Portals;
	std::vector<Elite::Vector2> m_DebugNodePositions;
	static bool sShowPolygon;
	static bool sShowGraph;
	static bool sDrawPortals;
	static bool sDrawFinalPath;
	static bool sDrawNonOptimisedPath;

	void UpdateImGui();
private:
	//C++ make the class non-copyable
	App_NavMeshGraph(const App_NavMeshGraph&) = delete;
	App_NavMeshGraph& operator=(const App_NavMeshGraph&) = delete;
};
#endif