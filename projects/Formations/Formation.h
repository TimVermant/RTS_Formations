#pragma once
#include <vector>

class BattleUnitAgent;

class Formation
{
public:

	Formation() = default;
	Formation(BattleUnitAgent* pLeader) :
		m_pLeaderUnit{ pLeader }
	{

	}



	void CreateFormation();


private:

	BattleUnitAgent* m_pLeaderUnit = nullptr;
	std::vector<BattleUnitAgent*> m_pUnits;
};