#pragma once

class CGameObj;

class CGameObjContainer
{
private:
	static CGameObjContainer* m_pGameObjContainer;

protected:
	map< int, vector< CGameObj* >> m_mapObj;
	map< int, vector< CGameObj* >> m_mapReserveObj;

	map< int, CGameObj* > m_mapNetObjContainer;

public:
	CGameObjContainer();
	virtual ~CGameObjContainer();

protected:
	virtual CGameObj* Alloc(int iType) {
		return nullptr;
	}

	void PostProcess();
	const int InsertNetObjContainer(CGameObj* pObj);

public:
	CGameObj* CreateObj(int iType, wstring wsID, wstring wsName, D3DXVECTOR2 vPos, float fRotation, bool bCamera, int iNetObjID = -1, bool bNetObj = false);
	CGameObj* CreateObj(CNetGameActor* pNetObjData, bool bNetObj = false);

	void ClearContainer();

	CGameObj* FindNetObj(int iNetObjID);

};

