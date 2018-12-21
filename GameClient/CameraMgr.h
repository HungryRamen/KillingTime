#pragma once

class CGameObj;

enum ECAMERA
{
	ECAMERA_CENTER,
	ECAMERA_LEFT,
	ECAMERA_RIGHT,
	ECAMERA_UP,
	ECAMERA_DOWN,
};

class CCameraMgr
{
private:
	static CCameraMgr* m_pCameraMgr;

private:
	D3DXVECTOR2 m_vCameraPos;
	int m_iWidth;
	int m_iHeight;

	ECAMERA m_eCameraLR;
	ECAMERA m_eCameraUD;
private:
	CGameObj* m_pTargetObj;

public:
	CCameraMgr();
	virtual ~CCameraMgr();

	static CCameraMgr* GetCameraMgr() {
		if (m_pCameraMgr == nullptr)
			m_pCameraMgr = new CCameraMgr;

		return m_pCameraMgr;
	}

	void SetTargetObj(CGameObj* pGameObj) {
		m_pTargetObj = pGameObj;
	}

	void InitSize(const int  iWidth, const int iHeight);

	const int GetWidth() {
		return m_iWidth;
	}

	const int GetHeight() {
		return m_iHeight;
	}

	D3DXVECTOR2 ChangeCameraPos(D3DXVECTOR2 vPos);
	void OnFrameMove(const float fElapsedTime);
	void GetCameraViewTrans(D3DXMATRIX* pmatTrans);

	void PlayerNamePos(CGameObj* pObj,wstring wsFontName);
};

