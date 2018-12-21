#pragma once

class CGameLobyStage : public CGameStage
{
public:
	CGameLobyStage();
	virtual ~CGameLobyStage();

	virtual void OnBegin();
	virtual void OnEnd();

	virtual void OnFrameMove(float fElapsedTime);
	virtual void OnFrameRender() {}
};