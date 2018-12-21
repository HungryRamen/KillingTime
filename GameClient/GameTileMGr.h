#pragma once

class CGameObj;

class CGameTileMgr
{
private:
	int m_iWidth;
	int m_iHeight;

	int m_iOffSetX;
	int m_iOffSetY;

private:
	int m_iViewXID;
	int m_iViewYID;
	int m_iMaxView;

private:
	static CGameTileMgr* m_pGameTileMgr;

private:
	vector<CGameObj*>m_vecTileList;
public:
	CGameTileMgr();
	virtual ~CGameTileMgr();

	static CGameTileMgr* GetGameTileMgr() { if (m_pGameTileMgr == nullptr)m_pGameTileMgr = new CGameTileMgr; return m_pGameTileMgr; }


	int GetTileWidth() { return m_iWidth; }
	int GetTileHeight() { return m_iHeight; }
	int GetOffSetX() { return m_iOffSetX; }
	int GetOffSetY() { return m_iOffSetY; }

	CGameObj* CreateTile(D3DXVECTOR2 vPos, wstring wsRenderName);
	void Destroy();

	D3DXVECTOR2 ChangeTileIndexToPos(const int iTileIndex);
	wstring ChangeTileDataIDToTileRenderName(const int iTileDataID);

	bool TilePlayerCollision(CGameObj* pObj);

	bool RectPushCollisionCheck(CGameObj * pObj1, CGameObj * pObj2);

	void LoadTileMap(const wstring wsFile);

	RECT CreateRect(CGameObj* pObj,bool bTP);
};
