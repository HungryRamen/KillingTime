#pragma once

class CGameServerMsg
{
private:
	void  (CGameServerMsg::*m_pGameServerMsgList[1000])(const int iSessionID, SNET_PACKET_HEAD*);

public:
	CGameServerMsg();
	~CGameServerMsg();

public:
	void ProcessPacket(const int iSessionID, SNET_PACKET_HEAD* pHead);

public:
	void GAME_SERVER_MSG_JOIN(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CREATE_OBJ(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CREATE_PLAYER(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CHANGE_POS(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CHANGE_ROTATION(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CHANGE_STATE(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_DESTROY_NETOBJ(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CREATE_BULLET(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_HEART_BEAT(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_NETOBJSYNC(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_CHANGE_KILLSCORE(const int iSessionID, SNET_PACKET_HEAD* pHead);
	void GAME_SERVER_MSG_RESPAWN_PLAYER(const int iSessionID, SNET_PACKET_HEAD* pHead);
};

