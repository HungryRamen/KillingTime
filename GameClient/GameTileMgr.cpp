#include "DXUT.h"

CGameTileMgr* CGameTileMgr::m_pGameTileMgr = nullptr;

CGameTileMgr::CGameTileMgr()
{
}

CGameTileMgr::~CGameTileMgr()
{
}

CGameObj * CGameTileMgr::CreateTile(D3DXVECTOR2 vPos, wstring wsRenderName)
{
	CGameObj* pGameObj = D_GAMEOBJMGR->CreateObj(EGAMEOBJ_NORMAL, wsRenderName, wsRenderName, vPos, 0.0f, true);
	return pGameObj;
}

void CGameTileMgr::Destroy()
{
	vector< CGameObj* >::iterator vit = m_vecTileList.begin();
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

bool CGameTileMgr::TilePlayerCollision(CGameObj * pObj)
{
	bool bCheck = false;
	for (int i = 0; i < (int)m_vecTileList.size(); ++i)
	{
		if(RectPushCollisionCheck(pObj, m_vecTileList[i]))
			bCheck = true;
	}
	return bCheck;
}

bool CGameTileMgr::RectPushCollisionCheck(CGameObj * pObj1, CGameObj * pObj2)
{
	RECT rcRect1 = CreateRect(pObj1,true);   //�о ������Ʈ
	RECT rcRect2 = CreateRect(pObj2,false);   //������ ������Ʈ

	RECT rcTemp;                        //����ũ��

	if (IntersectRect(&rcTemp, &rcRect1, &rcRect2) == TRUE)  //�簢���浹Ȯ��
	{
		float fWidth = float(rcTemp.right - rcTemp.left);  //���ΰ�
		float fHeight = float(rcTemp.bottom - rcTemp.top); //���ΰ�

		if (fWidth > fHeight)  //�����Ʒ�
		{
			if (rcTemp.top == rcRect2.top)  //���¹����� ���ϰ��
			{
				pObj1->SetPos(D3DXVECTOR2(pObj1->GetPos()->x, pObj1->GetPos()->y - fHeight));
			}
			else if (rcTemp.bottom == rcRect2.bottom) //���¹����� �Ʒ��ϰ��
			{
				pObj1->SetPos(D3DXVECTOR2(pObj1->GetPos()->x, pObj1->GetPos()->y + fHeight));
			}
		}
		else                 //�¿�
		{
			if (rcTemp.left == rcRect2.left) //���¹����� �����ϰ��
			{
				pObj1->SetPos(D3DXVECTOR2(pObj1->GetPos()->x - fWidth, pObj1->GetPos()->y));
			}
			else if (rcTemp.right == rcRect2.right) //���¹����� �������ϰ��
			{
				pObj1->SetPos(D3DXVECTOR2(pObj1->GetPos()->x + fWidth, pObj1->GetPos()->y));
			}
		}
		return true;
	}
	return false;
}

void CGameTileMgr::LoadTileMap(const wstring wsFile)
{
	if (wsFile.empty() == true)
		return;

	Destroy();

	const STileMapData* pTileData = D_GAMEDATAMGR->LoadTileMap(wsFile);
	m_iWidth = pTileData->m_iWidth;
	m_iHeight = pTileData->m_iHeight;
	m_iOffSetX = 50;	// Ÿ�� ������ ����� ���������� �������.
	m_iOffSetY = 50;

	for each(vector< STileObjData >::const_reference vr in  pTileData->m_vecTileObjData)
	{
		//if (ChangeTileDataIDToTileRenderName(vr.m_iTileDataID) == L"Tile3" || ChangeTileDataIDToTileRenderName(vr.m_iTileDataID) == L"Tile6")
		//{

		CGameObj* pObj = CreateTile(ChangeTileIndexToPos(vr.m_iTileIndex), ChangeTileDataIDToTileRenderName(vr.m_iTileDataID));
		if (pObj->GetName() == L"Tile3" || pObj->GetName() == L"Tile6")
			m_vecTileList.push_back(pObj);
		//}	
	}
}

RECT CGameTileMgr::CreateRect(CGameObj * pObj,bool bTP)
{
	D3DXVECTOR2 vPos = *pObj->GetPos();

	RECT rcRect = { (LONG)(vPos.x)
		, (LONG)(vPos.y)
		, (LONG)(vPos.x + m_iOffSetX)
		, (LONG)(vPos.y + m_iOffSetY) };
	
	if (bTP)
	{
		rcRect = { (LONG)(vPos.x - 20.0f)
		, (LONG)(vPos.y - 20.0f)
		, (LONG)(vPos.x + 20.0f)
		, (LONG)(vPos.y + 20.0f) };
	}
	return rcRect;
}
