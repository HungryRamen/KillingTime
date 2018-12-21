#pragma once
class CGameMainStage : public CGameStage
{
public:
	CGameMainStage();
	virtual ~CGameMainStage();

	virtual void OnBegin();
	virtual void OnEnd();

	virtual void OnFrameMove(float fElaspedTime);
	virtual void OnFrameRender() {}
};

