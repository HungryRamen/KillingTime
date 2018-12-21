#pragma once

class CHPBar;

class CUnitObj : public CGameObj, public CUnit
{
private:
	CHPBar * m_pHPBar;


public:
	CUnitObj();
	virtual ~CUnitObj();


public:
	virtual const int Load(const wstring wsFile);
	virtual void OnFrameMove(float fElapsedTime);
	virtual void OnFrameRender();

};

