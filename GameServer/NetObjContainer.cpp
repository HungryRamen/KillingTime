#include "NetLogicHead.h"

CNetGameActorContainer::CNetGameActorContainer() :
	m_iNetObjID(0)
{
}


CNetGameActorContainer::~CNetGameActorContainer()
{
}

CNetGameActor* const CNetGameActorContainer::CreateNetGameObjdata(SGameObjData* pData, D3DXVECTOR2 vPos,
	D3DXVECTOR2 vScl, float fRotation, int iRenderTurn, bool bCamera, int iMasterSessionID, bool bStartSync)
{
	CNetGameActor* pNetGameActor = AllocNetGameActor(pData->m_iGameDataType);
	if (pNetGameActor == nullptr)
		return nullptr;

	pNetGameActor->SetPos(vPos);
	pNetGameActor->SetScl(vScl);
	pNetGameActor->SetRotation(fRotation);
	pNetGameActor->SetRenderTurn(iRenderTurn);
	pNetGameActor->SetCamera(bCamera);
	pNetGameActor->SetNetObjID(m_iNetObjID);
	pNetGameActor->SetMasterSessionID(iMasterSessionID);
	pNetGameActor->SetStartSync(bStartSync);
	pNetGameActor->SetTeam(iMasterSessionID);

	vector<SColliderData*>::iterator vitAtk = pData->m_vecATKCollider.begin();
	for (vitAtk; vitAtk != pData->m_vecATKCollider.end(); ++vitAtk)
		D_COLLIDERMGR->CreateCollider(pNetGameActor, (*vitAtk));

	vector<SColliderData*>::iterator vitDef = pData->m_vecDEFCollider.begin();
	for (vitDef; vitDef != pData->m_vecDEFCollider.end(); ++vitDef)
		D_COLLIDERMGR->CreateCollider(pNetGameActor, (*vitDef));

	if (pData != nullptr)
	{
		pNetGameActor->SetGameDataID(pData->m_wsID);
		pNetGameActor->SetRenderID(pData->m_wsRenderDataID);
		pNetGameActor->SetGameObjType(pData->m_iGameDataType);
		switch (pData->m_iGameDataType)
		{
		case EGAMEOBJDATA_UNIT:
			((CNetUnitActor*)pNetGameActor)->SetSpeed(((SUnitObjData*)pData)->m_fSpeed);
			((CNetUnitActor*)pNetGameActor)->SetHP(((SUnitObjData*)pData)->m_iHp);
			break;

		case EGAMEOBJDATA_BULLET:
		{
			((CNetBulletActor*)pNetGameActor)->SetSpeed(((SBulletObjData*)pData)->m_fSpeed);
			((CNetBulletActor*)pNetGameActor)->SetHP(((SBulletObjData*)pData)->m_iHp);
			((CNetBulletActor*)pNetGameActor)->SetLifeTime(((SBulletObjData*)pData)->m_fLifeTime);
			((CNetBulletActor*)pNetGameActor)->SetDamage(((SBulletObjData*)pData)->m_iDamage);
			pNetGameActor->SetMasterSessionID(-1);
		}

		}
	}

	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.find(m_iNetObjID);
	if (mit != m_mapNetGameObjData.end())
		return nullptr;

	m_mapNetGameObjData.insert(std::make_pair(pNetGameActor->GetNetObjID(), pNetGameActor));
	++m_iNetObjID;

	SNET_REQ_CREATE_OBJ_FB* pNetPacket = new SNET_REQ_CREATE_OBJ_FB;
	pNetPacket->Init();
	pNetPacket->m_NetGameActor = *pNetGameActor;
	if(pData->m_iGameDataType == EGAMEOBJDATA_UNIT)
		pNetPacket->m_wsUserName = D_GAMESERVERMGR->GetGameServer()->wsSerchSessionUserName(iMasterSessionID);
	D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(false, pNetPacket->m_iPacketSize, (char*)pNetPacket);

	cout << " NetObjContainer Size : " << m_mapNetGameObjData.size() << endl;

	return pNetGameActor;
}

const int CNetGameActorContainer::DisconnectSession(const int iSessionID)
{
	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.begin();
	while (mit != m_mapNetGameObjData.end())
	{
		if (mit->second->GetMasterSessionID() == -1)
		{
			mit++;
			continue;
		}

		if (mit->second->GetMasterSessionID() == iSessionID)
		{
			SNET_REQ_DESTROY_OBJ_FB* pSendPacket = new SNET_REQ_DESTROY_OBJ_FB;
			pSendPacket->Init();
			pSendPacket->m_iNetObjID = mit->second->GetNetObjID();
			D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(false, pSendPacket->m_iPacketSize, (char*)pSendPacket);

			SAFE_DELETE(mit->second);
			m_mapNetGameObjData.erase(mit++);
		}
		else
			mit++;
	}

	return 1;
}

int CNetGameActorContainer::SendAllNetGameObjDataToSession(int iSessionID)
{
	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.begin();
	for (mit; mit != m_mapNetGameObjData.end(); ++mit)
	{
		if (mit->second == nullptr)
			continue;

		if (mit->second->IsStartSync() == false)
			continue;

		SNET_REQ_CREATE_OBJ_FB* pPacket = new SNET_REQ_CREATE_OBJ_FB();
		pPacket->Init();
		pPacket->m_NetGameActor = *mit->second;
		pPacket->m_wsUserName = D_GAMESERVERMGR->GetGameServer()->wsSerchSessionUserName(mit->second->GetMasterSessionID());
		pPacket->m_iKillCount = D_GAMESERVERMGR->GetGameServer()->iSerchSessionUserKillCount(mit->second->GetMasterSessionID());

		D_GAMESERVERMGR->GetGameServer()->PostSendSession(iSessionID, false, pPacket->m_iPacketSize, (char*)pPacket);
	}

	return 1;
}

const int CNetGameActorContainer::RemoveNetGameObjData(int iNetObjID)
{
	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.find(iNetObjID);
	if (mit == m_mapNetGameObjData.end())
		return -1;

	SAFE_DELETE(mit->second);
	m_mapNetGameObjData.erase(iNetObjID);
	return 1;
}

const int CNetGameActorContainer::DestroyNetGameObjData(int iNetObjID)
{
	if (iNetObjID == -1)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	SNET_REQ_DESTROY_OBJ_FB* pSendPacket = new SNET_REQ_DESTROY_OBJ_FB;
	pSendPacket->Init();
	pSendPacket->m_iNetObjID = iNetObjID;
	D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(false, pSendPacket->m_iPacketSize, (char*)pSendPacket);

	RemoveNetGameObjData(iNetObjID);
	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

int CNetGameActorContainer::BulletUserSerch(int iTeam)
{
	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.begin();
	for (mit; mit != m_mapNetGameObjData.end(); ++mit)
	{
		if (mit->second == nullptr)
			continue;

		if (mit->second->GetGameObjType() != EGAMEOBJDATA_UNIT)
			continue;

		if (iTeam == mit->second->GetTeam())
			return mit->second->GetNetObjID();
	}

	return -1;
}

const int CNetGameActorContainer::SyncNetObjContainer(int iSessionID)
{
	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.begin();
	for (mit; mit != m_mapNetGameObjData.end(); ++mit)
	{
		if (mit->second == nullptr)
			continue;

		SNET_REQ_NETOBJSYNC_FB* pSyncPacket = new SNET_REQ_NETOBJSYNC_FB;
		pSyncPacket->Init();
		pSyncPacket->m_iNetObjID = mit->second->GetNetObjID();
		pSyncPacket->m_vPos = *mit->second->GetPos();
		pSyncPacket->m_fRotation = *mit->second->GetRotation();
		pSyncPacket->m_eState = mit->second->GetState();

		D_GAMESERVERMGR->GetGameServer()->PostSendSession(iSessionID, false, pSyncPacket->m_iPacketSize, (char*)pSyncPacket);
	}

	return EMSG_SYSTEM_SUCCESS;
}
