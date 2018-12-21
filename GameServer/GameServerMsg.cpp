#include "NetLogicHead.h"


CGameServerMsg::CGameServerMsg()
{
	m_pGameServerMsgList[SNET_HEAD_SERVER_JOIN_PACKET] = &CGameServerMsg::GAME_SERVER_MSG_JOIN;
	m_pGameServerMsgList[SNET_HEAD_CREATE_OBJ] = &CGameServerMsg::GAME_SERVER_MSG_CREATE_OBJ;
	m_pGameServerMsgList[SNET_HEAD_CREATE_PLAYER] = &CGameServerMsg::GAME_SERVER_MSG_CREATE_PLAYER;
	m_pGameServerMsgList[SNET_HEAD_CHANGE_POS] = &CGameServerMsg::GAME_SERVER_MSG_CHANGE_POS;
	m_pGameServerMsgList[SNET_HEAD_CHANGE_ROTATION] = &CGameServerMsg::GAME_SERVER_MSG_CHANGE_ROTATION;
	m_pGameServerMsgList[SNET_HEAD_CHANGE_STATE] = &CGameServerMsg::GAME_SERVER_MSG_CHANGE_STATE;
	m_pGameServerMsgList[SNET_HEAD_DESTROY_NETOBJ] = &CGameServerMsg::GAME_SERVER_MSG_DESTROY_NETOBJ;
	m_pGameServerMsgList[SNET_HEAD_CREATE_BULLET] = &CGameServerMsg::GAME_SERVER_MSG_CREATE_BULLET;
	m_pGameServerMsgList[SNET_HEAD_HEARTBEAT] = &CGameServerMsg::GAME_SERVER_MSG_HEART_BEAT;
	m_pGameServerMsgList[SNET_HEAD_NETOBJSYNC] = &CGameServerMsg::GAME_SERVER_MSG_NETOBJSYNC;
	m_pGameServerMsgList[SNET_HEAD_CHANGE_KILLSCORE] = &CGameServerMsg::GAME_SERVER_MSG_CHANGE_KILLSCORE;
	m_pGameServerMsgList[SNET_HEAD_RESPAWN_PLAYER] = &CGameServerMsg::GAME_SERVER_MSG_RESPAWN_PLAYER;
}


CGameServerMsg::~CGameServerMsg()
{
}

void CGameServerMsg::ProcessPacket(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	if (pHead == nullptr)
		return;

	(this->*m_pGameServerMsgList[pHead->m_iHeadID])(iSessionID, pHead);
}

void CGameServerMsg::GAME_SERVER_MSG_JOIN(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_SERVER_JOIN* pRecvPacket = (SNET_REQ_SERVER_JOIN*)pHead;
	
	SNET_REQ_SERVER_JOIN_FB* pNetPacekt = new SNET_REQ_SERVER_JOIN_FB;
	pNetPacekt->Init();
	pNetPacekt->m_iSessionID = iSessionID;
	D_GAMESERVERMGR->GetGameServer()->PostSendSession(iSessionID, false, pNetPacekt->m_iPacketSize, (char*)pNetPacekt);
	D_GAMESERVERMGR->GetGameServer()->InsertConnectSession(iSessionID,pRecvPacket->m_wsUserName);

	cout << "SNET_REQ_SERVER_JOIN : " << iSessionID << endl;
	D_GAMESERVERMGR->GetNetGameActorMgr()->SendAllNetGameObjDataToSession(iSessionID);

	SNET_REQ_DO_HEARTBEAT_FB* pHeartBeat = new SNET_REQ_DO_HEARTBEAT_FB;
	pHeartBeat->Init();
	D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(false, pHeartBeat->m_iPacketSize, (char*)pHeartBeat);
}

void CGameServerMsg::GAME_SERVER_MSG_CREATE_OBJ(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_CREATE_OBJ* pPacket = (SNET_REQ_CREATE_OBJ*)pHead;
	D_GAMESERVERMGR->GetGameServer()->CreateObjFunction(pPacket->m_wsID, D3DXVECTOR2(0.0f, 0.0f), 0.0f, iSessionID);
	cout << "SNET_HEAD_CREATE_OBJ" << endl;
}

void CGameServerMsg::GAME_SERVER_MSG_CREATE_PLAYER(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_CREATE_PLAYER* pPacket = (SNET_REQ_CREATE_PLAYER*)pHead;
	const int iNetObjID = D_GAMESERVERMGR->GetGameServer()->CreateObjFunction(L"Player", D3DXVECTOR2(0.0f, 0.0f), 0.0f, iSessionID);

	SNET_REQ_CREATE_PLAYER_FB* pCreatePlayerFB = new SNET_REQ_CREATE_PLAYER_FB();
	cout << "SNET_REQ_CREATE_PLAYER_FB" << endl;
	pCreatePlayerFB->Init();
	pCreatePlayerFB->m_iPlayerNetObjID = iNetObjID;
	pCreatePlayerFB->m_sPlayerData = *(SUnitObjData*)D_GAMEDATAMGR->GetGameObjData(L"Player");
	D_GAMESERVERMGR->GetGameServer()->PostSendSession(iSessionID, false, pCreatePlayerFB->m_iPacketSize, (char*)pCreatePlayerFB);
}

void CGameServerMsg::GAME_SERVER_MSG_CHANGE_POS(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_CHANGE_POS* pPacket = (SNET_REQ_CHANGE_POS*)pHead;
	if (pPacket->m_iNetObjID == -1)
		return;

	CNetGameActor* pData = D_GAMESERVERMGR->GetNetGameActorMgr()->GetNetGameObjData(pPacket->m_iNetObjID);
	if (pData == nullptr)
		return;

	if (pData->GetMasterSessionID() != iSessionID)
		return;

	if (pData != nullptr)
		pData->SetPos(pPacket->m_vPos);
}

void CGameServerMsg::GAME_SERVER_MSG_CHANGE_ROTATION(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_CHANGE_ROTATION* pPacket = (SNET_REQ_CHANGE_ROTATION*)pHead;
	if (pPacket->m_iNetObjID == -1)
		return;

	CNetGameActor* pData = D_GAMESERVERMGR->GetNetGameActorMgr()->GetNetGameObjData(pPacket->m_iNetObjID);
	if (pData == nullptr)
		return;

	pData->SetRotation(pPacket->m_fRotation);
}

void CGameServerMsg::GAME_SERVER_MSG_CHANGE_STATE(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_CHANGE_STATE* pPacket = (SNET_REQ_CHANGE_STATE*)pHead;
	if (pPacket->m_iNetObjID == -1)
		return;

	CNetGameActor* pData = D_GAMESERVERMGR->GetNetGameActorMgr()->GetNetGameObjData(pPacket->m_iNetObjID);

	if (pData == nullptr)
		return;

	if (pData->GetMasterSessionID() != iSessionID)
		return;

	if (pData != nullptr)
		pData->ChangeState(pPacket->m_eState);
}

void CGameServerMsg::GAME_SERVER_MSG_DESTROY_NETOBJ(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_DESTROY_OBJ* pPacket = (SNET_REQ_DESTROY_OBJ*)pHead;
	if (pPacket->m_iNetObjID == -1)
		return;

	CNetGameActor* pData = D_GAMESERVERMGR->GetNetGameActorMgr()->GetNetGameObjData(pPacket->m_iNetObjID);
	if (pData == nullptr)
		return;

	if (pData->GetMasterSessionID() != iSessionID)
		return;

	D_GAMESERVERMGR->GetNetGameActorMgr()->RemoveNetGameObjData(pData->GetNetObjID());
	SNET_REQ_DESTROY_OBJ_FB* pSendPacket = new SNET_REQ_DESTROY_OBJ_FB;
	pSendPacket->Init();
	pSendPacket->m_iNetObjID = pData->GetNetObjID();
	D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(true, pSendPacket->m_iPacketSize, (char*)pSendPacket);
}

void CGameServerMsg::GAME_SERVER_MSG_CREATE_BULLET(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	SNET_REQ_CREATE_BULLET* pPacket = (SNET_REQ_CREATE_BULLET*)pHead;
	CNetGameActor* pData = D_GAMESERVERMGR->GetNetGameActorMgr()->GetNetGameObjData(pPacket->m_iNetObjID);
	if (pData == nullptr)
		return;

	const int iNetObjID = D_GAMESERVERMGR->GetGameServer()->CreateObjFunction(L"Bullet1", *pData->GetPos(), *pData->GetRotation(), iSessionID);
}

void CGameServerMsg::GAME_SERVER_MSG_HEART_BEAT(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
}

void CGameServerMsg::GAME_SERVER_MSG_NETOBJSYNC(const int iSessionID, SNET_PACKET_HEAD* pHead)
{
	D_GAMESERVERMGR->GetNetGameActorMgr()->SyncNetObjContainer(iSessionID);

	SNET_REQ_NETOBJSYNC_END_FB* pSyncEndPacket = new SNET_REQ_NETOBJSYNC_END_FB;
	pSyncEndPacket->Init();
	D_GAMESERVERMGR->GetGameServer()->PostSendSession(iSessionID, false, pSyncEndPacket->m_iPacketSize, (char*)pSyncEndPacket);
}

void CGameServerMsg::GAME_SERVER_MSG_CHANGE_KILLSCORE(const int iSessionID, SNET_PACKET_HEAD * pHead)
{
	SNET_REQ_CHANGE_KILLSCORE_FB* pSendPacket = new SNET_REQ_CHANGE_KILLSCORE_FB;
	pSendPacket->Init();
	pSendPacket->m_iKillScore = 0;
	D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(true, pSendPacket->m_iPacketSize, (char*)pSendPacket);
}

void CGameServerMsg::GAME_SERVER_MSG_RESPAWN_PLAYER(const int iSessionID, SNET_PACKET_HEAD * pHead)
{
	SNET_REQ_RESPAWN_PLAYER* pPacket = (SNET_REQ_RESPAWN_PLAYER*)pHead;
	CNetGameActor* pData = D_GAMESERVERMGR->GetNetGameActorMgr()->GetNetGameObjData(pPacket->m_iNetObjID);
	if (pData == nullptr)
		return;

	if (pData->GetMasterSessionID() != iSessionID)
		return;
	pData->SetPos(D_GAMESERVERMGR->GetGameServer()->GetSpawnPos(D_GAMESERVERMGR->GetGameServer()->GetSpawnCount() % D_GAMESERVERMGR->GetGameServer()->GetSpawnPointMax()));
	pData->ChangeState(AT_NORMAL);
	SGameObjData* pObjData = D_GAMEDATAMGR->GetGameObjData(L"Player");
	((CNetUnitActor*)pData)->SetHP(((SUnitObjData*)pObjData)->m_iHp);

	SNET_REQ_RESPAWN_PLAYER_FB* pSendPacket = new SNET_REQ_RESPAWN_PLAYER_FB;
	pSendPacket->Init();
	pSendPacket->m_eState = pData->GetState();
	pSendPacket->m_iHp = ((CNetUnitActor*)pData)->GetHP();
	pSendPacket->m_iNetObjID = pPacket->m_iNetObjID;
	pSendPacket->m_vPos = *pData->GetPos();
	D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(true, pSendPacket->m_iPacketSize, (char*)pSendPacket);
}
