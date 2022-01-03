#pragma once
#include <vector>
#include <iostream>
#include "framework/EliteMath/EMath.h"
#include "framework\EliteAI\EliteGraphs\ENavGraph.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EAStar.h"

namespace Elite
{
	class NavMeshPathfinding
	{
	public:
		static std::vector<Elite::Vector2> FindPath(Elite::Vector2 startPos, Elite::Vector2 endPos, Elite::NavGraph* pNavGraph, std::vector<Elite::Vector2>& debugNodePositions, std::vector<Elite::Portal>& debugPortals)
		{
			//Create the path to return
			std::vector<Elite::Vector2> finalPath{};

			//Get the start and endTriangle
			auto startTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(startPos);
			auto endTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(endPos);
			if (startTriangle == nullptr || endTriangle == nullptr)
				return finalPath;
			if (startTriangle == endTriangle)
			{
				finalPath.push_back(endPos);
				return finalPath;
			}
			//We have valid start/end triangles and they are not the same
			//=> Start looking for a path
			//Copy the graph
			auto graph = pNavGraph->Clone();

			//Create extra node for the Start Node (Agent's position
			auto startNode = new NavGraphNode{ graph->GetNextFreeNodeIndex(), -1,startPos };
			graph->AddNode(startNode);
			int nodeIndex = -1;
			for (auto edge : startTriangle->metaData.IndexLines)
			{
				nodeIndex = pNavGraph->GetNodeIdxFromLineIdx(edge);
				if (pNavGraph->IsNodeValid(nodeIndex))
				{

					graph->AddConnection(new GraphConnection2D{ startNode->GetIndex(),nodeIndex,
						Distance(startNode->GetPosition(),graph->GetNode(nodeIndex)->GetPosition()) });
				}
			}
			//Create extra node for the endNode
			auto endNode = new NavGraphNode{ graph->GetNextFreeNodeIndex(), -1, endPos };
			graph->AddNode(endNode);
			nodeIndex = -1;
			for (auto edge : endTriangle->metaData.IndexLines)
			{
				nodeIndex = pNavGraph->GetNodeIdxFromLineIdx(edge);
				if (pNavGraph->IsNodeValid(nodeIndex))
				{

					graph->AddConnection(new GraphConnection2D{ endNode->GetIndex(),nodeIndex,
						Distance(endNode->GetPosition(),graph->GetNode(nodeIndex)->GetPosition()) });
				}
			}
			//Run A star on new graph
			Elite::Heuristic heuristic = Elite::HeuristicFunctions::Chebyshev;
			auto pathfinder = Elite::AStar<Elite::NavGraphNode, Elite::GraphConnection2D>(graph.get(), heuristic);

			auto path = pathfinder.FindPath(startNode, endNode);
			for (auto node : path)
			{
				finalPath.push_back(node->GetPosition());
			}

			//OPTIONAL BUT ADVICED: Debug Visualisation

			//Run optimiser on new graph, MAKE SURE the A star path is working properly before starting this section and uncommenting this!!!
			
			debugPortals = SSFA::FindPortals(path, pNavGraph->GetNavMeshPolygon());
			finalPath = SSFA::OptimizePortals(debugPortals);

			return finalPath;
		}
	};
}
