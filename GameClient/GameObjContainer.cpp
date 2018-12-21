#include "DXUT.h"

CGameObjContainer::CGameObjContainer()
{
}


CGameObjContainer::~CGameObjContainer()
{
	ClearContainer();
}

void CGameObjContainer::ClearContainer()
{
	for each(map< int, vector< CGameObj* >>::const_reference mr in m_mapObj)
	{
		for each(vector< CGameObj* >::const_reference vr in mr.second)
			delete vr;
	}

	m_mapReserveObj.clear();
	m_mapObj.clear();
}

CGameObj* CGameObjContainer::CreateObj(int iType, wstring wsID, wstring wsName,
	D3DXVECTOR2 vPos, float fRotation, bool bCamera = false, int iNetObjID, bool bNetObj)
{
	CGameObj* pObj = Alloc(iType);
	if (pObj == nullptr)
		return nullptr;

	pObj->SetPos(vPos);
	pObj->SetRotation(fRotation);

	pObj->Load(wsID);
	pObj->SetName(wsName);
	m_mapReserveObj[iType].push_back(pObj);

	if (bNetObj == true)
	{
		pObj->SetNetObjID(iNetObjID);
		InsertNetObjContainer(pObj);
	}

	return pObj;
}

const int CGameObjContainer::InsertNetObjContainer(CGameObj* pNetObj)
{
	if (pNetObj == nullptr)
		return -1;

	map< int, CGameObj* >::iterator mit = m_mapNetObjContainer.find(pNetObj->GetNetObjID());
	if (mit != m_mapNetObjContainer.end())
		return -1;

	m_mapNetObjContainer.insert(make_pair(pNetObj->GetNetObjID(), pNetObj));
	return 1;
}

CGameObj* CGameObjContainer::FindNetObj(int iID)
{
	map< int, CGameObj* >::iterator mit = m_mapNetObjContainer.find(iID);
	if (mit == m_mapNetObjContainer.end())
		return nullptr;

	return mit->second;
}

CGameObj* CGameObjContainer::CreateObj(CNetGameActor* pData, bool bNetObj)
{
	return CreateObj(pData->GetGameObjType(), pData->GetGameDataID(), pData->GetGameDataID(), *pData->GetPos(), *pData->GetRotation(), pData->IsCamera(), pData->GetNetObjID(), bNetObj);
}

void CGameObjContainer::PostProcess()
{
	for each(map< int, vector< CGameObj* >>::const_reference mr in m_mapReserveObj)
	{
		vector< CGameObj* >& vecObj = m_mapObj[mr.first];
		vecObj.insert(vecObj.end(), mr.second.begin(), mr.second.end());
	}

	m_mapReserveObj.clear();

	map< int, vector< CGameObj* >>::iterator mit = m_mapObj.begin();
	while (mit != m_mapObj.end())
	{
		for (int i = (int)mit->second.size() - 1; i >= 0; --i)
		{
			if (*mit->second[i]->IsDestroy() == false)
				continue;


			SAFE_DELETE(mit->second[i]);
			mit->second.erase(mit->second.begin() + i);
		}

		if (mit->second.empty())
			mit = m_mapObj.erase(mit);
		else
			++mit;
	}
}