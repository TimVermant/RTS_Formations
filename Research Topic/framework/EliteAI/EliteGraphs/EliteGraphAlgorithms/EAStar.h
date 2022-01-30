#pragma once

namespace Elite
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType* pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(IGraph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction)
		: m_pGraph(pGraph)
		, m_HeuristicFunction(hFunction)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		std::vector<T_NodeType*> path;
		std::vector<NodeRecord> openList;
		std::vector<NodeRecord> closedList;
		NodeRecord currentRecord;

		//currentRecord.pNode = pStartNode;
		//currentRecord.pConnection = nullptr;
		//currentRecord.costSoFar = 0.f;
		//currentRecord.estimatedTotalCost = GetHeuristicCost(pStartNode, pGoalNode);

		NodeRecord startNode{ pStartNode,nullptr,0.f,GetHeuristicCost(pStartNode,pGoalNode) };

		openList.push_back(startNode);
		while (!openList.empty())
		{


			//Get connection with lowest F score
			currentRecord = *std::min_element(openList.begin(), openList.end());

			// Leads to end node
			if (currentRecord.pNode == pGoalNode)
			{
				break; // If yes exit while loop
			}

			// get all connections from currentRecord and loop over them
			for (auto connection : m_pGraph->GetNodeConnections(currentRecord.pNode->GetIndex()))
			{
				// Get total cost
				float totalCostSoFar = currentRecord.costSoFar + connection->GetCost();


				auto connectionPredicate = [&](NodeRecord node)
				{
					return node.pNode == m_pGraph->GetNode(connection->GetTo());
				};
				auto iterClosed = std::find_if(closedList.begin(), closedList.end(), connectionPredicate);
				auto iterOpen = std::find_if(openList.begin(), openList.end(), connectionPredicate);

				if (iterClosed != closedList.end()) // Check if any of the connections lead to a node already on the closed list
				{
					if (iterClosed->costSoFar <= totalCostSoFar) // Check if alreaedy existing connection is cheaper
					{
						continue;
					}
					else
					{
						closedList.erase(std::remove(closedList.begin(), closedList.end(), *iterClosed));
					}
				}

				if (iterOpen != openList.end()) // Check if any of the connections lead to a node already on the closed list
				{
					if (iterOpen->costSoFar <= totalCostSoFar) // Check if already existing connection is cheaper
					{
						continue;
					}
					else
					{
						openList.erase(std::remove(openList.begin(), openList.end(), *iterOpen));
					}
				}

				NodeRecord newNode;
				newNode.pNode = m_pGraph->GetNode(connection->GetTo());
				newNode.pConnection = connection;
				newNode.costSoFar = totalCostSoFar;
				newNode.estimatedTotalCost = totalCostSoFar + GetHeuristicCost(newNode.pNode, pGoalNode);
				openList.push_back(newNode);

			}

			//remove noderecord from the openlist
			openList.erase(std::remove(openList.begin(), openList.end(), currentRecord));


			closedList.push_back(currentRecord);


		}
		//for (auto node : closedList)
		//{
		//	if (node.pConnection != nullptr)
		//		std::cout << node.pConnection->GetFrom() << " - " << node.pNode->GetIndex()  << " - " << node.pConnection->GetTo() << std::endl;
		//}
		//Find the node right before end of path in the closed list
		while (currentRecord.pNode != pStartNode)
		{
			path.push_back(currentRecord.pNode);
			// find previous connection
			auto it = std::find_if(closedList.begin(), closedList.end(),
				[&currentRecord](const NodeRecord& node) { return node.pNode->GetIndex() == currentRecord.pConnection->GetFrom(); });

			if (it != closedList.end())
			{
				currentRecord = *it;
			}
			else
			{
				std::cout << "end iterator";
			}

		}
		path.push_back(pStartNode);
		std::reverse(path.begin(), path.end());


		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float Elite::AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const
	{
		Vector2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}