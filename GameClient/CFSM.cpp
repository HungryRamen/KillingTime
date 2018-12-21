#include "DXUT.h"

CFSM::CFSM(void) :
	m_pCurState(NULL),
	m_pNextState(NULL)
{
}


CFSM::~CFSM(void)
{
}

void CFSM::AddState(int iState, const SFSMState& pState)
{
	map< int, SFSMState >::iterator mit = m_mapState.find(iState);
	if (mit != m_mapState.end())
		return;

	m_mapState.insert(make_pair(iState, pState));
}

SFSMState* CFSM::GetState(int iState)
{
	map< int, SFSMState >::iterator mit = m_mapState.find(iState);
	if (mit == m_mapState.end())
		return NULL;

	return &mit->second;
}

bool CFSM::OnChangeFSMState(int iState, bool bReserve)
{
	m_pNextState = GetState(iState);
	if (m_pNextState == NULL)
		return false;

	if (bReserve == false)
		OnChangeReserveState();

	return true;
}

void CFSM::OnChangeReserveState()
{
	if (m_pNextState == NULL)
		return;

	if (m_pCurState != NULL && m_pCurState->m_pfEnd != NULL)
		(m_pCurState->m_pfEnd)(m_pCurState->m_dwArgs);

	m_pCurState = m_pNextState;
	m_pNextState = NULL;

	if (m_pCurState->m_pfBegin != NULL)
		(m_pCurState->m_pfBegin)(m_pCurState->m_dwArgs);
}

void CFSM::OnUpdateState(float fElapsedTime)
{
	if (m_pCurState != NULL && m_pCurState->m_pfUpdate != NULL)
		(m_pCurState->m_pfUpdate)(m_pCurState->m_dwArgs, fElapsedTime);
}