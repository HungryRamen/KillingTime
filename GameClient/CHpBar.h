#pragma once
class CHPBar : public CGameObj
{
private:
	CUnitObj * m_pTargetUnitObj;

	int m_iWIdth;
	int m_iHeight;

public:
	CHPBar();
	virtual ~CHPBar();

	void SetTargetObj(CUnitObj* pObj) {
		m_pTargetUnitObj = pObj;
	}

	virtual const int Load(const wstring wsID);
	virtual void OnFrameMove(float fElapsedTime);
	virtual void OnFrameRender();
};

