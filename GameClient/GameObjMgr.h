#pragma once

class CGameObjMgr : public CGameObjContainer
{
private:
	static CGameObjMgr* m_pGameObjMgr;

private:
	CGameObj* m_pPlayerObj;

private:
	void OnFrameMove(const vector< CGameObj* >& vecObj, float fElapsedTime);
	void OnFrameRender(const vector< CGameObj* >& vecObj);

protected:
	virtual CGameObj* Alloc(int iType);

public:
	CGameObjMgr();
	virtual ~CGameObjMgr();

	static CGameObjMgr* GetGameObjMgr() {
		if (m_pGameObjMgr == nullptr) m_pGameObjMgr = new CGameObjMgr;
		return m_pGameObjMgr;
	}

	void SetPlayerObj(CGameObj* pObj) {
		m_pPlayerObj = pObj;
	}

	CGameObj* GetPlayerObj() {
		return m_pPlayerObj;
	}

	void OnFrameMove(float fElapsedTime);
	void OnFrameRender();
};

