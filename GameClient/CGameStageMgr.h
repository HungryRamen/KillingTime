#pragma once

enum ESTAGE
{
	ESTAGE_MENU,
	ESTAGE_MAIN_STAGE_1,
	ESTAGE_LOBY_STAGE,
};

class CGameStage;

class CGameStageMgr
{
private:
	static CGameStageMgr* m_pGameStageMgr;

private:
	int m_iCurrentStage;
	int m_iNextStage;

	CGameStage* m_pCurrentStage;

public:
	static CGameStageMgr* GetGameStageMgr()
	{
		if (m_pGameStageMgr == nullptr)
			m_pGameStageMgr = new CGameStageMgr();

		return m_pGameStageMgr;
	}

public:
	CGameStageMgr();
	virtual ~CGameStageMgr();

	CGameStage* Alloc(int const iStage);
	void OnChangeStage(int const iStage);

	virtual void Load(const wstring wsFile);

	virtual void OnFrameMove(float fElapsedTime);
	virtual void OnFrameRender();
};

