#pragma once

struct SFSMState
{
	typedef void(*PFBEGIN)(DWORD);
	typedef void(*PFUPDATE)(DWORD, float);
	typedef void(*PFEND)(DWORD);

	PFBEGIN m_pfBegin;
	PFUPDATE m_pfUpdate;
	PFEND m_pfEnd;

	DWORD m_dwArgs;

	SFSMState(DWORD dwArgs, PFBEGIN pfBegin, PFUPDATE pfUpdate, PFEND pfEnd) :
		m_dwArgs(dwArgs),
		m_pfBegin(pfBegin),
		m_pfUpdate(pfUpdate),
		m_pfEnd(pfEnd)
	{
	}
};

class CFSM
{
private:
	map< int, SFSMState > m_mapState;

protected:
	SFSMState * m_pCurState;
	SFSMState* m_pNextState;

public:
	CFSM(void);
	virtual ~CFSM(void);

	void AddState(int iState, const SFSMState& pState);
	SFSMState* GetState(int iState);

	bool OnChangeFSMState(int iState, bool bReserve = true);
	void OnChangeReserveState();

	void OnUpdateState(float fElapsedTime);
};

