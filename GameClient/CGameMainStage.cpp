#include "DXUT.h"


CGameMainStage::CGameMainStage()
{
}


CGameMainStage::~CGameMainStage()
{
}

void CGameMainStage::OnBegin()
{
	D_GAMEDATAMGR->LoadData();
	D_CONNECTMGR->LoginServer(L"");

	D_GAMETILEMGR->LoadTileMap(L"../Data/0.2Map");
	D_FONT->CreateFont(L"Score", L"", D3DXVECTOR2(0.0f, 0.0f), DT_LEFT, 30, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
}

void CGameMainStage::OnEnd()
{
	D_FONT->DestroyAll();
}

void CGameMainStage::OnFrameMove(float fElaspedTime)
{
	D_FONT->ViewKillScore();
}

//void CGameMainStage::OnFrameRender()
//{
//}

