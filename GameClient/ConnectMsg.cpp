#include "DXUT.h"

CConnectMsg::CConnectMsg()
{
	m_pConnectMsgList[SNET_HEAD_SERVER_JOIN_PACKET_FB] = &CConnectMsg::GAME_SERVER_MSG_JOIN_FB;
	m_pConnectMsgList[SNET_HEAD_CREATE_PLAYER_FB] = &CConnectMsg::GAME_SERVER_MSG_CREATE_PLAYER_FB;
	m_pConnectMsgList[SNET_HEAD_CREATE_OBJ_FB] = &CConnectMsg::GAME_SERVER_MSG_CREATE_OBJ_FB;
	m_pConnectMsgList[SNET_HEAD_CHANGE_POS_FB] = &CConnectMsg::GAME_SERVER_MSG_CHANGE_POS_FB;
	m_pConnectMsgList[SNET_HEAD_CHANGE_ROTATION_FB] = &CConnectMsg::GAME_SERVER_MSG_CHANGE_ROTATION_FB;
	m_pConnectMsgList[SNET_HEAD_CHANGE_STATE_FB] = &CConnectMsg::GAME_SERVER_MSG_CHANGE_STATE_FB;
	m_pConnectMsgList[SNET_HEAD_DESTROY_NETOBJ_FB] = &CConnectMsg::GAME_SERVER_MSG_DESTROY_NETOBJ_FB;
	m_pConnectMsgList[SNET_HEAD_DO_HEARTBEAT_FB] = &CConnectMsg::GAME_SERVER_MSG_HEART_BEAT_FB;
	m_pConnectMsgList[SNET_HEAD_HEARTBEAT_FB] = &CConnectMsg::GAME_SERVER_MSG_HEART_BEAT_FB;
	m_pConnectMsgList[SNET_HEAD_NETOBJSYNC_FB] = &CConnectMsg::GAME_SERVER_MSG_NETOBJ_SYNC_FB;
	m_pConnectMsgList[SNET_HEAD_NETOBJSYNC_END_FB] = &CConnectMsg::GAME_SERVER_MSG_NETOBJ_SYNC_END_FB;
	m_pConnectMsgList[SNET_HEAD_CHANGE_HP_FB] = &CConnectMsg::GAME_SERVER_MSG_CHANGE_HP_FB;
	m_pConnectMsgList[SNET_HEAD_CHANGE_KILLSCORE_FB] = &CConnectMsg::GAME_SERVER_MSG_CHANGE_KILLSCORE_FB;
	m_pConnectMsgList[SNET_HEAD_RESPAWN_PLAYER_FB] = &CConnectMsg::GAME_SERVER_MSG_RESPAWN_PLAYER_FB;
}

CConnectMsg::~CConnectMsg()
{
}

void CConnectMsg::ProcessPakcet(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	if (pHeader == nullptr)
		return;

	(this->*m_pConnectMsgList[pHeader->m_iHeadID])(iSessionID, pHeader);
}

void CConnectMsg::GAME_SERVER_MSG_JOIN_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	D_CONNECTMGR->LoginOk();

	SNET_REQ_SERVER_JOIN_FB* pNetPacket = (SNET_REQ_SERVER_JOIN_FB*)pHeader;
	D_CONNECTMGR->SetSessionID(pNetPacket->m_iSessionID);

	SNET_REQ_CREATE_PLAYER* pCreatePacket = new SNET_REQ_CREATE_PLAYER;
	pCreatePacket->Init();
	pCreatePacket->m_iCreatePlayerID = EGAMEOBJDATA_UNIT;
	D_CONNECTMGR->PostSend(true, pCreatePacket->m_iPacketSize, (char*)pCreatePacket);
}

void CConnectMsg::GAME_SERVER_MSG_CREATE_OBJ_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CREATE_OBJ_FB* pNetPacket = (SNET_REQ_CREATE_OBJ_FB*)pHeader;
	CUnitObj* pObj = (CUnitObj*)D_GAMEOBJMGR->CreateObj(&pNetPacket->m_NetGameActor, true);
	if (pNetPacket->m_NetGameActor.GetGameObjType() == EGAMEOBJ_UNIT)
	{
		wstringstream wsFont;
		wsFont << pObj->GetNetObjID();
		D_FONT->UserNameInsert(pObj->GetNetObjID(), pNetPacket->m_wsUserName);
		D_FONT->UserKillInsert(pObj->GetNetObjID(), pNetPacket->m_iKillCount);
		D_FONT->CreateFont(wsFont.str(), pNetPacket->m_wsUserName, D3DXVECTOR2(0.0f, 0.0f), DT_LEFT, 15, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (pNetPacket->m_NetGameActor.GetGameObjType() == EGAMEOBJ_UNIT || pNetPacket->m_NetGameActor.GetGameObjType() == EGAMEOBJ_BULLET)
		pObj->SetHP(((CUnit*)&pNetPacket->m_NetGameActor)->GetHP());

	if (pNetPacket->m_NetGameActor.GetGameObjType() == EGAMEOBJ_BULLET)
	{
		SBulletObjData* pBulletData = (SBulletObjData*)D_GAMEDATAMGR->GetGameObjData(pNetPacket->m_NetGameActor.GetGameDataID());
		((CBulletObj*)pObj)->SetLifeTime(pBulletData->m_fLifeTime);
		((CBulletObj*)pObj)->SetSpeed(pBulletData->m_fSpeed);
	}

	SUnitObjData* pPlayerData = (SUnitObjData*)D_GAMEDATAMGR->GetGameObjData(pNetPacket->m_NetGameActor.GetGameDataID());
	if (pPlayerData != nullptr)
		(pObj)->SetMaxHP(pPlayerData->m_iHp);
}

void CConnectMsg::GAME_SERVER_MSG_CREATE_PLAYER_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CREATE_PLAYER_FB* pNetPacket = (SNET_REQ_CREATE_PLAYER_FB*)pHeader;
	CUnitObj* pObj = (CUnitObj*)D_GAMEOBJMGR->FindNetObj(pNetPacket->m_iPlayerNetObjID);
	if (pObj != nullptr)
	{
		pObj->SetGameObjController(new CPlayerController(pObj));
		D_GAMEOBJMGR->SetPlayerObj(pObj);

		pObj->SetSpeed(pNetPacket->m_sPlayerData.m_fSpeed);
		pObj->SetHP(pNetPacket->m_sPlayerData.m_iHp);
		pObj->SetMaxHP(pNetPacket->m_sPlayerData.m_iHp);
		pObj->ChangeState(ESTATE::AT_NORMAL);
	}

	SNET_REQ_NETOBJSYNC* pPacket = new SNET_REQ_NETOBJSYNC;
	pPacket->Init();
	D_CONNECTMGR->PostSend(false, pPacket->m_iPacketSize, (char*)pPacket);
}

void CConnectMsg::GAME_SERVER_MSG_CHANGE_POS_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CHANGE_POS_FB* pNetPacket = (SNET_REQ_CHANGE_POS_FB*)pHeader;
	CGameObj* pObj = D_GAMEOBJMGR->FindNetObj(pNetPacket->m_iNetObjID);

	if (pObj != nullptr)
		pObj->SetPos(pNetPacket->m_vPos);
}

void CConnectMsg::GAME_SERVER_MSG_CHANGE_ROTATION_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CHANGE_ROTATION_FB* pNetPacket = (SNET_REQ_CHANGE_ROTATION_FB*)pHeader;
	CGameObj* pObj = D_GAMEOBJMGR->FindNetObj(pNetPacket->m_iNetObjID);
	if (pObj != nullptr)
		pObj->SetRotation(pNetPacket->m_fRotation);
}

void CConnectMsg::GAME_SERVER_MSG_CHANGE_STATE_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CHANGE_STATE_FB* pNetPacket = (SNET_REQ_CHANGE_STATE_FB*)pHeader;

	CGameObj* pObj = D_GAMEOBJMGR->FindNetObj(pNetPacket->m_iNetObjID);
	if (pObj != nullptr)
		pObj->ChangeState(pNetPacket->m_eState);
}

void CConnectMsg::GAME_SERVER_MSG_DESTROY_NETOBJ_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_DESTROY_OBJ_FB* pNetPacket = (SNET_REQ_DESTROY_OBJ_FB*)pHeader;
	CGameObj* pObj = D_GAMEOBJMGR->FindNetObj(pNetPacket->m_iNetObjID);
	if (pObj != nullptr)
	{
		if (pObj->GetName() == L"Bullet1")
		{
			pObj->ChangeState(AT_DIE);
			return;
		}
		wstringstream wsNetObjID;
		wsNetObjID << pObj->GetNetObjID();
		D_FONT->Destroy(wsNetObjID.str());
		D_FONT->UserNameDelete(pObj->GetNetObjID());
		D_FONT->UserKillDelete(pObj->GetNetObjID());
		pObj->Destroy();
	}
}

void CConnectMsg::GAME_SERVER_MSG_HEART_BEAT_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
}

void CConnectMsg::GAME_SERVER_MSG_NETOBJ_SYNC_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_NETOBJSYNC_FB* pNetPacket = (SNET_REQ_NETOBJSYNC_FB*)pHeader;
	CUnitObj* pObj = (CUnitObj*)D_GAMEOBJMGR->FindNetObj(pNetPacket->m_iNetObjID);
	if (pObj != nullptr)
	{
		float fCheck = (pObj->GetSpeed() * DXUTGetElapsedTime()) * 5;
		float fDist = sqrt(pow(pNetPacket->m_vPos.x - pObj->GetPos()->x, 2)
			+ pow(pNetPacket->m_vPos.y - pObj->GetPos()->y, 2));
		
		if (fDist >= fCheck)
			pObj->SetPos(pNetPacket->m_vPos);

		float fRoatationDist = abs(*pObj->GetRotation() - pNetPacket->m_fRotation);
		if (fRoatationDist >= fCheck)
		pObj->SetRotation(pNetPacket->m_fRotation);

		if (pNetPacket->m_eState != pObj->GetState())
			pObj->ChangeState(pNetPacket->m_eState, true);
	}
}

void CConnectMsg::GAME_SERVER_MSG_NETOBJ_SYNC_END_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_NETOBJSYNC* pPacket = new SNET_REQ_NETOBJSYNC;
	pPacket->Init();
	D_CONNECTMGR->PostSend(false, pPacket->m_iPacketSize, (char*)pPacket);
}

void CConnectMsg::GAME_SERVER_MSG_CHANGE_HP_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CHANGE_HP_FB* pPacket = (SNET_REQ_CHANGE_HP_FB*)pHeader;
	CUnitObj* pObj = (CUnitObj*)D_GAMEOBJMGR->FindNetObj(pPacket->m_iNetObjID);
	if (pObj != nullptr)
	{
		pObj->SetHP(pPacket->m_iHp);
	}
}

void CConnectMsg::GAME_SERVER_MSG_CHANGE_KILLSCORE_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_CHANGE_KILLSCORE_FB* pPacket = (SNET_REQ_CHANGE_KILLSCORE_FB*)pHeader;
	D_FONT->UserKillPlus(pPacket->m_iNetObjID);
}

void CConnectMsg::GAME_SERVER_MSG_RESPAWN_PLAYER_FB(const int iSessionID, SNET_PACKET_HEAD * pHeader)
{
	SNET_REQ_RESPAWN_PLAYER_FB* pPacket = (SNET_REQ_RESPAWN_PLAYER_FB*)pHeader;
	CUnitObj* pObj = (CUnitObj*)D_GAMEOBJMGR->FindNetObj(pPacket->m_iNetObjID);

	if (pObj != nullptr)
	{
		pObj->SetPos(pPacket->m_vPos);
		pObj->ChangeState(pPacket->m_eState);
		pObj->SetHP(pPacket->m_iHp);
	}
}
