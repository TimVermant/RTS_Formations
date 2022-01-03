#include "stdafx.h"
#include "ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

using namespace Elite;

Elite::NavGraph::NavGraph(const Polygon& contourMesh, float playerRadius = 1.0f) :
	Graph2D(false),
	m_pNavMeshPolygon(nullptr)
{
	//Create the navigation mesh (polygon of navigatable area= Contour - Static Shapes)
	m_pNavMeshPolygon = new Polygon(contourMesh); // Create copy on heap

	//Get all shapes from all static rigidbodies with NavigationCollider flag
	auto vShapes = PHYSICSWORLD->GetAllStaticShapesInWorld(PhysicsFlags::NavigationCollider);

	//Store all children
	for (auto shape : vShapes)
	{
		shape.ExpandShape(playerRadius);
		m_pNavMeshPolygon->AddChild(shape);
	}

	//Triangulate
	m_pNavMeshPolygon->Triangulate();

	//Create the actual graph (nodes & connections) from the navigation mesh
	CreateNavigationGraph();
}

Elite::NavGraph::~NavGraph()
{
	delete m_pNavMeshPolygon;
	m_pNavMeshPolygon = nullptr;
}

int Elite::NavGraph::GetNodeIdxFromLineIdx(int lineIdx) const
{
	auto nodeIt = std::find_if(m_Nodes.begin(), m_Nodes.end(), [lineIdx](const NavGraphNode* n) { return n->GetLineIndex() == lineIdx; });
	if (nodeIt != m_Nodes.end())
	{
		return (*nodeIt)->GetIndex();
	}

	return invalid_node_index;
}

Elite::Polygon* Elite::NavGraph::GetNavMeshPolygon() const
{
	return m_pNavMeshPolygon;
}

void Elite::NavGraph::CreateNavigationGraph()
{
	//1. Go over all the edges of the navigationmesh and create nodes

	//Loop over all the lines
	for (auto line : m_pNavMeshPolygon->GetLines())
	{
		auto LITriangles = m_pNavMeshPolygon->GetTrianglesFromLineIndex(line->index);
		if (LITriangles.size() <= 1) // Check if line is connected to anoter triangle
			continue;
		auto middleLine = (line->p1 + line->p2) / 2.f;
		NavGraphNode* navGraphNode = new NavGraphNode{ GetNextFreeNodeIndex(), line->index,middleLine };
		AddNode(navGraphNode);

	}

	//Loop over all the triangles
	for (auto triangle : m_pNavMeshPolygon->GetTriangles())
	{

		std::vector<int> validNodeIdxs;
		for (auto lineIdx : triangle->metaData.IndexLines)
		{
			int nodeIdx = GetNodeIdxFromLineIdx(lineIdx);
		//	auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&](NavGraphNode* node) { return lineIdx == node->GetIndex(); });
			if (IsNodeValid(nodeIdx))
				validNodeIdxs.push_back(nodeIdx);

		}
		// No connections possible
		if (validNodeIdxs.size() < 2)
			continue;

		std::list<Elite::GraphConnection2D*> connectionList;
		if (validNodeIdxs.size() == 2)
		{

			auto connection = new Elite::GraphConnection2D{ validNodeIdxs[0],validNodeIdxs[1] };
			AddConnection(connection);
			//connectionList.push_back(connection);
		}
		else if (validNodeIdxs.size() == 3)
		{
			auto connection1 = new Elite::GraphConnection2D{ validNodeIdxs[0],validNodeIdxs[1] };
			auto connection2 = new Elite::GraphConnection2D{ validNodeIdxs[1],validNodeIdxs[2] };
			auto connection3 = new Elite::GraphConnection2D{ validNodeIdxs[2],validNodeIdxs[0] };
			AddConnection(connection1);
			AddConnection(connection2);
			AddConnection(connection3);
			/*	connectionList.push_back(connection1);
				connectionList.push_back(connection2);
				connectionList.push_back(connection3);*/
		}
		else
		{
			continue;
		}
		//m_Connections.push_back(connectionList);
	}
	SetConnectionCostsToDistance();



	//2. Create connections now that every node is created

	//3. Set the connections cost to the actual distance
}

