#pragma once
#include <vector>


class BattleUnitAgent;

class Formation final
{
public:

	Formation(size_t nrOfUnits);
	~Formation();


	void Render(float dt);
	void Update(float dt);


	void CreateFormation(float trimWorldSize);
	void CreateFormation(float trimWorldSize, Elite::Vector2 startPosition);


	void AddUnit(BattleUnitAgent* pUnit);



	//Get reference for IMGUI
	int& GetUnitsPerLine();
	float& GetDistanceBetweenUnitsOnLine();
	float& GetVerticalDistanceBetweenUnitsOnLine();
	float& GetDistanceBetweenLines();
	bool& GetRetainLeader();

private:
	size_t m_FormationMaxSize = 36;

	BattleUnitAgent* m_pLeaderUnit = nullptr;
	std::vector<BattleUnitAgent*> m_pUnits;
	
	Elite::Vector2 m_StartPosition;
	bool m_bRetainLeaderUnit = false;

	// Helpers
	BattleUnitAgent* GetClosestUnit(float trimWorldSize);
	BattleUnitAgent* GetClosestUnit(float trimWorldSize, Elite::Vector2 startPosition);
	void CalculateDesiredFormationPositions();
	

	float AngleBetweenVectors(Elite::Vector2 A, Elite::Vector2 B);
	

	// Formation math variables

	int m_UnitsPerLine;
	float m_DistanceBetweenUnitsOnLine;
	float m_VerticalDistanceBetweenUnitsOnLine;
	float m_DistanceBetweenLines;
};