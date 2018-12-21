#pragma once

class CRender;
class CGameObjController;

class CGameObj : public CActor
{
protected:
	CRender * m_pRender;
	CGameObjController* m_pController;
	map<ESTATE, CRender*>m_mapRender;
protected:

	int m_iNetObjID;
	D3DXCOLOR m_d3dColor;
	wstring m_wsName;

	RECT* m_prcRect;

public:
	CGameObj();
	virtual ~CGameObj();

	virtual CRender* const GetRender() {
		return m_pRender;
	}

	virtual void ChangeState(const ESTATE eState, bool bNet = false);

	const int GetNetObjID() {
		return m_iNetObjID;
	}

	void SetNetObjID(int iID)
	{
		m_iNetObjID = iID;
	}

	const D3DXCOLOR GetColor() {
		return m_d3dColor;
	}

	void SetColor(D3DCOLOR d3dColor)
	{
		m_d3dColor = d3dColor;
	}

	void SetName(wstring wsName) {
		m_wsName = wsName;
	}

	const wstring GetName() {
		return m_wsName;
	}

	const RECT* GetRect() {
		return m_prcRect;
	}

	void SetRect(int iWidth, int iHeight)
	{
		if (m_prcRect == nullptr)
			m_prcRect = new RECT;

		m_prcRect->left = 0;
		m_prcRect->top = 0;
		m_prcRect->right = iWidth;
		m_prcRect->bottom = iHeight;
	}


public:
	void SetGameObjController(CGameObjController* pController) {
		m_pController = pController;
	}

public:

	CRender * RenderAlloc(int iRenderType);
	virtual const int Load(const wstring wsID);

	virtual void OnFrameMove(float fElapsedTime);
	virtual void OnFrameRender();

};

