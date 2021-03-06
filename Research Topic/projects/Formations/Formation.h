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

	void AddUnit(BattleUnitAgent* pUnit);

	void SetStartPosition(const Elite::Vector2& startPos);


	//Get reference for IMGUI
	int& GetUnitsPerLine();
	float& GetDistanceBetweenUnitsOnLine();
	float& GetVerticalDistanceBetweenUnitsOnLine();
	float& GetDistanceBetweenLines();

private:
	size_t m_FormationMaxSize = 36;

	BattleUnitAgent* m_pLeaderUnit = nullptr;
	std::vector<BattleUnitAgent*> m_pUnits;
	
	Elite::Vector2 m_StartPosition;

	// Helpers
	BattleUnitAgent* GetClosestUnit(float trimWorldSize);
	void CalculateDesiredFormationPositions();
	
	bool m_UseMouseStartPos = false;
	// Formation math variables

	int m_UnitsPerLine;
	float m_DistanceBetweenUnitsOnLine;
	float m_VerticalDistanceBetweenUnitsOnLine;
	float m_DistanceBetweenLines;
};