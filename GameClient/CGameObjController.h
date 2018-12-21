#pragma once

class CGameObj;

class CGameObjController : public CFSM
{
protected:
	CGameObj * m_pGameObj;

public:
	CGameObjController(CGameObj* pObj);
	virtual ~CGameObjController();

	virtual void OnFrameMove(float fElpasedTime) {}
	virtual void OnEndAni(int iAniCategory) {}
	virtual void OnChangeState(ESTATE const eState, bool bNet = false) { OnChangeFSMState(eState); }

	CGameObj* GetGameObj() { return m_pGameObj; }
};


