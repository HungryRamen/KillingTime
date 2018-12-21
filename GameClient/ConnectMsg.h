#pragma once

class CConnectMsg
{
private:
	void (CConnectMsg::*m_pConnectMsgList[1000])(const int iSessionID, SNET_PACKET_HEAD*);

public:
	CConnectMsg();
	virtual ~CConnectMsg();
	
public:
	void ProcessPakcet(const int iSessionID, SNET_PACKET_HEAD* pHeader);

public:
	void GAME_SERVER_MSG_JOIN_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CREATE_PLAYER_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CREATE_OBJ_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CHANGE_POS_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CHANGE_ROTATION_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CHANGE_STATE_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_DESTROY_NETOBJ_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_HEART_BEAT_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_NETOBJ_SYNC_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_NETOBJ_SYNC_END_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CHANGE_HP_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_CHANGE_KILLSCORE_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
	void GAME_SERVER_MSG_RESPAWN_PLAYER_FB(const int iSessionID, SNET_PACKET_HEAD* pHeader);
};