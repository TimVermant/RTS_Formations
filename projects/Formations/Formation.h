#pragma once
#include <vector>


class BattleUnitAgent;

class Formation
{
public:

	Formation(size_t nrOfUnits);
	~Formation();


	void Render(float dt);
	void Update(float dt);


	void CreateFormation(float trimWorldSize);

	void AddUnit(BattleUnitAgent* pUnit);

private:
	size_t m_FormationMaxSize = 36;

	BattleUnitAgent* m_pLeaderUnit = nullptr;
	std::vector<BattleUnitAgent*> m_pUnits;
	
	std::vector<Elite::Vector2> m_FormationPositions;
	Elite::Vector2 m_StartPosition;

	// Helpers
	BattleUnitAgent* GetClosestUnit(float trimWorldSize);
	void CalculateDesiredFormationPositions();
	

	// Formation math variables

	int m_UnitsPerLine;
	float m_DistanceBetweenUnitsOnLine;
	float m_VerticalDistanceBetweenUnitsOnLine;
	float m_DistanceBetweenLines;
};