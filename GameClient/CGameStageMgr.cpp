#include "DXUT.h"

CGameStageMgr* CGameStageMgr::m_pGameStageMgr = nullptr;

CGameStageMgr::CGameStageMgr() :
	m_iCurrentStage(-1),
	m_pCurrentStage(nullptr),
	m_iNextStage(-1)
{
}


CGameStageMgr::~CGameStageMgr()
{
}

void CGameStageMgr::Load(const wstring wsFile)
{
	OnChangeStage(ESTAGE_MAIN_STAGE_1);
}

void CGameStageMgr::OnFrameMove(float fElapsedTime)
{
	if (m_pCurrentStage == nullptr)
		return;

	m_pCurrentStage->OnFrameMove(fElapsedTime);
	D_GAMEOBJMGR->OnFrameMove(fElapsedTime);
}

void CGameStageMgr::OnFrameRender()
{
	D_GAMEOBJMGR->OnFrameRender();
}

CGameStage* CGameStageMgr::Alloc(int const iStage)
{
	switch (iStage)
	{
	case ESTAGE_MENU:
		break;

	case ESTAGE_MAIN_STAGE_1:
		return new CGameMainStage();

	case ESTAGE_LOBY_STAGE:
		return new CGameLobyStage();
	}

	return nullptr;
}

void CGameStageMgr::OnChangeStage(const int iStage)
{
	if (m_pCurrentStage != nullptr)
	{
		m_pCurrentStage->OnEnd();
		SAFE_DELETE(m_pCurrentStage);
	}

	m_pCurrentStage = Alloc(iStage);

	if (m_pCurrentStage != nullptr)
		m_pCurrentStage->OnBegin();
}

