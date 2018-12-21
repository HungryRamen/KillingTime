#pragma once
class CGameStage
{
private:
	int m_iStage;

public:
	CGameStage();
	virtual ~CGameStage();

	virtual void Load(const wstring wsFile) {}
	virtual void OnBegin() {}
	virtual void OnFrameMove(float fElapsedTime) {}
	virtual void OnEnd() {}
	virtual void OnFraemRender() {}
};

