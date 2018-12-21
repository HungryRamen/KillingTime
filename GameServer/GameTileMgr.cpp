#include "NetLogicHead.h"

CGameTileMgr* CGameTileMgr::m_pGameTileMgr = nullptr;

CGameTileMgr::CGameTileMgr()
{
}

CGameTileMgr::~CGameTileMgr()
{
}

CActor * CGameTileMgr::CreateTile(D3DXVECTOR2 vPos, wstring wsRenderName)
{
	SGameObjData* pObjData = D_GAMEDATAMGR->GetGameObjData(wsRenderName);
	CActor* pActor = new CActor;
	pActor->SetPos(vPos);
	pActor->SetScl(D3DXVECTOR2(1.0f, 1.0f));
	pActor->SetRotation(0.0f);
	pActor->SetRenderTurn(0);
	pActor->SetCamera(true);
	pActor->SetTeam(-1);

	vector<SColliderData*>::iterator vitDef = pObjData->m_vecDEFCollider.begin();
	for (vitDef; vitDef != pObjData->m_vecDEFCollider.end(); ++vitDef)
		D_COLLIDERMGR->CreateCollider(pActor, (*vitDef));

	pActor->SetGameDataID(pObjData->m_wsID);
	pActor->SetRenderID(pObjData->m_wsRenderDataID);
	pActor->SetGameObjType(pObjData->m_iGameDataType);
	return pActor;
}

void CGameTileMgr::Destroy()
{
	vector< CActor* >::iterator vit = m_vecTileList.begin();
	for (vit; vit != m_vecTileList.end(); ++vit)
		SAFE_DELETE(*vit);

	m_vecTileList.clear();
}

D3DXVECTOR2 CGameTileMgr::ChangeTileIndexToPos(const int iTileIndex)
{
	int iX = -1;
	int iY = -1;

	iY = iTileIndex / m_iWidth;
	iX = iTileIndex % m_iWidth;

	return D3DXVECTOR2(iX * 50.0f, iY * 50.0f);
}

wstring CGameTileMgr::ChangeTileDataIDToTileRenderName(const int iTileDataID)
{
	switch (iTileDataID)
	{
	case 0:
		return L"Tile1";
	case 1:
		return L"Tile2";
	case 2:
		return L"Tile3";
	case 3:
		return L"Tile4";
	case 4:
		return L"Tile5";
	case 5:
		return L"Tile6";
	case 6:
		return L"Tile7";
	case 7:
		return L"Tile8";
	case 8:
		return L"Tile9";
	case 9:
		return L"Tile10";
	}
	return L"";
}


void CGameTileMgr::LoadTileMap(const wstring wsFile)
{
	if (wsFile.empty() == true)
		return;

	Destroy();

	const STileMapData* pTileData = D_GAMEDATAMGR->LoadTileMap(wsFile);
	m_iWidth = pTileData->m_iWidth;
	m_iHeight = pTileData->m_iHeight;
	m_iOffSetX = 50;	// 타일 사이즈 현재는 고정값으로 사용하자.
	m_iOffSetY = 50;

	for each(vector< STileObjData >::const_reference vr in  pTileData->m_vecTileObjData)
	{
		if (ChangeTileDataIDToTileRenderName(vr.m_iTileDataID) == L"Tile3" || ChangeTileDataIDToTileRenderName(vr.m_iTileDataID) == L"Tile6")
		{
			CActor* pActor = CreateTile(ChangeTileIndexToPos(vr.m_iTileIndex), ChangeTileDataIDToTileRenderName(vr.m_iTileDataID));
			m_vecTileList.push_back(pActor);
		}
	}
}