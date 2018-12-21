#pragma once

class CMouseMgr
{
private:
	static CMouseMgr* m_pMouseMgr;

	D3DXVECTOR2 m_vMousePos;
	D3DXVECTOR2 m_vCameraMousePos;

	bool m_bClick;

	vector<CGameObj*>m_vecMouseCollisionObj;
public:
	CMouseMgr();
	virtual ~CMouseMgr();

	static CMouseMgr* GetMouseMgr() { if (m_pMouseMgr == nullptr)m_pMouseMgr = new CMouseMgr; return m_pMouseMgr; }

	void AddMouseCollisionObj(CGameObj* pObj) { m_vecMouseCollisionObj.push_back(pObj); }
	void ClearMouseCollisionObj() { m_vecMouseCollisionObj.clear(); }

	bool IsClick() { return m_bClick; }

	const D3DXVECTOR2* GetMousePos() { return &m_vMousePos; }
	const D3DXVECTOR2* GetCameraMousePos() { return &m_vCameraMousePos; }

	void OnFrameMove(float fElapsedTime);
};