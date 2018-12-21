#pragma once
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
	vector<CActor*>m_vecTileList;
public:
	CGameTileMgr();
	virtual ~CGameTileMgr();

	static CGameTileMgr* GetGameTileMgr() { if (m_pGameTileMgr == nullptr)m_pGameTileMgr = new CGameTileMgr; return m_pGameTileMgr; }

	CActor* CreateTile(D3DXVECTOR2 vPos, wstring wsRenderName);
	void Destroy();

	D3DXVECTOR2 ChangeTileIndexToPos(const int iTileIndex);
	wstring ChangeTileDataIDToTileRenderName(const int iTileDataID);

	void LoadTileMap(const wstring wsFile);

};