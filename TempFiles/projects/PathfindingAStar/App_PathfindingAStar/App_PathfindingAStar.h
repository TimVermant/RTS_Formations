#ifndef ASTAR_APPLICATION_H
#define ASTAR_APPLICATION_H
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
#include "framework\EliteAI\EliteGraphs\EGridGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphEditor.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphUtilities\EGraphRenderer.h"


//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_PathfindingAStar final : public IApp
{
public:
	//Constructor & Destructor
	App_PathfindingAStar() = default;
	virtual ~App_PathfindingAStar();

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	//Datamembers
	const bool ALLOW_DIAGONAL_MOVEMENT = true;
	Elite::Vector2 m_StartPosition = Elite::ZeroVector2;
	Elite::Vector2 m_TargetPosition = Elite::ZeroVector2;

	//Grid datamembers
	static const int COLUMNS = 20;
	static const int ROWS = 10;
	unsigned int m_SizeCell = 15;
	Elite::GridGraph<Elite::GridTerrainNode, Elite::GraphConnection>* m_pGridGraph;


	//Pathfinding datamembers
	int startPathIdx = invalid_node_index;
	int endPathIdx = invalid_node_index;
	std::vector<Elite::GridTerrainNode*> m_vPath;

	//Editor and Visualisation
	Elite::GraphEditor m_GraphEditor{};
	Elite::GraphRenderer m_GraphRenderer{};

	//Debug rendering information
	bool m_bDrawGrid = true;
	bool m_bDrawNodeNumbers = false;
	bool m_bDrawConnections = false;
	bool m_bDrawConnectionsCosts = false;
	bool m_StartSelected = true;
	int m_SelectedHeuristic = 4;
	Elite::Heuristic m_pHeuristicFunction = Elite::HeuristicFunctions::Chebyshev;

	//Functions
	void MakeGridGraph();
	void UpdateImGui();
	void CalculatePath();

	//C++ make the class non-copyable
	App_PathfindingAStar(const App_PathfindingAStar&) = delete;
	App_PathfindingAStar& operator=(const App_PathfindingAStar&) = delete;
};
#endif