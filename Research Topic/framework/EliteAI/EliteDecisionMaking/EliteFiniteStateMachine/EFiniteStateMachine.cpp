//=== General Includes ===
#include "stdafx.h"
#include "EFiniteStateMachine.h"

Elite::FiniteStateMachine::FiniteStateMachine(Elite::FSMState* startState, Elite::Blackboard* pBlackboard)
	: m_pCurrentState(nullptr),
	m_pBlackboard(pBlackboard)
{
	ChangeState(startState);
}

Elite::FiniteStateMachine::~FiniteStateMachine()
{
	SAFE_DELETE(m_pBlackboard);
}

void Elite::FiniteStateMachine::AddTransition(Elite::FSMState* startState, Elite::FSMState* toState, Elite::FSMTransition* transition)
{
	auto it = m_Transitions.find(startState);
	if (it == m_Transitions.end())
	{
		m_Transitions[startState] = Transitions();
	}

	m_Transitions[startState].push_back(std::make_pair(transition, toState));
}

void Elite::FiniteStateMachine::Update(float deltaTime)
{
	//TODO 4: Look if 1 or more transition exists for the current state that we are in
	//Tip: Check the transitions map for a TransitionState pair
	auto it = m_Transitions.find(m_pCurrentState);
	if (it != m_Transitions.end())
	{
		for (TransitionStatePair& transPair : it->second)
		{
			if (transPair.first->ToTransition(m_pBlackboard))
			{
				ChangeState(transPair.second);
				break;
			}
		 }
	}

	if (m_pCurrentState)
		m_pCurrentState->Update(m_pBlackboard,deltaTime);

	//TODO 5: if a TransitionState exists

		//TODO 6: Loop over all the TransitionState pairs 
		//TODO 7: If a ToTransition returns true => transition to the new corresponding state


	//TODO 8: Update the current state (if one exists ;-))

}

Elite::Blackboard* Elite::FiniteStateMachine::GetBlackboard() const
{
	return m_pBlackboard;
}

void Elite::FiniteStateMachine::ChangeState(FSMState* newState)
{
	//TODO 1. If currently in a state => make sure the OnExit of that state gets called
	if (m_pCurrentState)
	{
		m_pCurrentState->OnExit(m_pBlackboard);
	}

	//TODO 2. Change the current state to the new state
	m_pCurrentState = newState;

	//TODO 3. Call the OnEnter of the new state
	if (m_pCurrentState)
	{
		m_pCurrentState->OnEnter(m_pBlackboard);
	}

}
