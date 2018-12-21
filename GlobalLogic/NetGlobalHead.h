
#pragma once

#include <string>
#include "d3dx9math.h"

#include "../GlobalLogic/GlobalLogicHeader.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif  


enum EMSG_SYSTEM
{
	EMSG_SYSTEM_FAIL = -1,
	EMSG_SYSTEM_SUCCESS = 1,
};

class CNetActor
{
protected:
	int m_iNetObjID;
	int m_iMasterSessionID;	// 이 넷게임 오브젝트 데이터를 수정가능한 세션의 아이디 
	bool m_bStartSync;		// 게임 시작시 동기화해야하는 오브젝트인가 ( 총알은 굳이 시작할때 모두 동기화 할필요없다. )

public:
	CNetActor() :
		m_iNetObjID(-1),
		m_iMasterSessionID(-1),
		m_bStartSync(false)
	{
	}

	virtual ~CNetActor() {}

	int const GetNetObjID() {
		return m_iNetObjID;
	}

	int const GetMasterSessionID()
	{
		return m_iMasterSessionID;
	}

	void SetNetObjID(const int iNetObjID)
	{
		m_iNetObjID = iNetObjID;
	}

	void SetMasterSessionID(int const iMasterSessionID)
	{
		m_iMasterSessionID = iMasterSessionID;
	}

	void SetStartSync(bool const bStartSync)
	{
		m_bStartSync = bStartSync;
	}

	bool const IsStartSync() {
		return m_bStartSync;
	}

};

class CActor;

class CNetGameActor : public CActor, public  CNetActor
{
public:
	CNetGameActor() {}
	virtual ~CNetGameActor() {}

public:
	virtual void OnFrameMove(float fElaspedTime) {}
};

class CUnit
{
protected:
	float m_fSpeed;
	int m_iMaxHp;
	int m_iHp;
	int m_iDamage;

public:

	CUnit() :
		m_fSpeed(0.0f),
		m_iMaxHp(0),
		m_iHp(0),
		m_iDamage(0)
	{
	}

	virtual ~CUnit() {}

	float const GetSpeed() {
		return m_fSpeed;
	}

	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	void SetMaxHP(int iMaxHP)
	{
		m_iMaxHp = iMaxHP;
	}

	void SetHP(int iHP)
	{
		m_iHp = iHP;
	}

	const int GetMaxHP() {
		return m_iMaxHp;
	}

	const int GetHP() {
		return m_iHp;
	}

	const int GetDamage() {
		return m_iDamage;
	}

	void SetDamage(const int iDamage)
	{
		m_iDamage = iDamage;
	}
};

class CBullet
{
protected:
	float m_fLifeTime;

public:

	CBullet() :
		m_fLifeTime(0.0f)
	{
	}

	virtual ~CBullet() {}

	const float GetLifeTime() {
		return m_fLifeTime;
	}

	void SetLifeTime(float fLifeTime) {
		m_fLifeTime = fLifeTime;
	}

};

class CNetUnitActor : public CNetGameActor, public CUnit
{
public:
	CNetUnitActor()
	{
	}

	virtual ~CNetUnitActor() {}

	virtual void OnDamage(int const iDamage) {
		m_iHp -= iDamage;

		//if (m_iHp <= 0)
		//	Destroy();
	}
};

const int PORT_NUMBER = 31450;
const int MAX_RECEIVE_BUFFER_LEN = 1024;
const int MAX_SESSION_COUNT = 256;


enum ENET_HEAD
{
	SNET_HEAD_SERVER_JOIN_PACKET,
	SNET_HEAD_SERVER_JOIN_PACKET_FB,
	SNET_HEAD_TEST_PACKET,
	SNET_HEAD_CREATE_OBJ,
	SNET_HEAD_CREATE_OBJ_FB,
	SNET_HEAD_CREATE_PLAYER,
	SNET_HEAD_CREATE_PLAYER_FB,
	SNET_HEAD_CHANGE_POS,
	SNET_HEAD_CHANGE_POS_FB,
	SNET_HEAD_CHANGE_ROTATION,
	SNET_HEAD_CHANGE_ROTATION_FB,
	SNET_HEAD_CHANGE_STATE,
	SNET_HEAD_CHANGE_STATE_FB,
	SNET_HEAD_DESTROY_NETOBJ,
	SNET_HEAD_DESTROY_NETOBJ_FB,
	SNET_HEAD_CREATE_BULLET,
	SNET_HEAD_CREATE_BULLET_FB,
	SNET_HEAD_HEARTBEAT,
	SNET_HEAD_HEARTBEAT_FB,
	SNET_HEAD_DO_HEARTBEAT_FB,
	SNET_HEAD_NETOBJSYNC,
	SNET_HEAD_NETOBJSYNC_FB,
	SNET_HEAD_NETOBJSYNC_END_FB,
	SNET_HEAD_CHANGE_HP,
	SNET_HEAD_CHANGE_HP_FB,
	SNET_HEAD_CHANGE_KILLSCORE,
	SNET_HEAD_CHANGE_KILLSCORE_FB,
	SNET_HEAD_RESPAWN_PLAYER,
	SNET_HEAD_RESPAWN_PLAYER_FB,
};

struct SNET_PACKET_HEAD
{
	int m_iHeadID;
	int m_iSendID;

	int m_iPacketSize;

	SNET_PACKET_HEAD() :
		m_iHeadID(-1),
		m_iSendID(-1),
		m_iPacketSize(-1)
	{
	}

	SNET_PACKET_HEAD(int iHeadID, int iSendID, int iPaceketSize) :
		m_iHeadID(iHeadID),
		m_iSendID(iSendID),
		m_iPacketSize(iPaceketSize)
	{
	}
};

struct SNET_REQ_SERVER_JOIN : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_SERVER_JOIN_PACKET;
		m_iPacketSize = sizeof(SNET_REQ_SERVER_JOIN);
	}
	wstring m_wsUserName;
};


struct SNET_REQ_SERVER_JOIN_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_SERVER_JOIN_PACKET_FB;
		m_iPacketSize = sizeof(SNET_REQ_SERVER_JOIN_FB);
		m_iSessionID = -1;
	}

	int m_iSessionID;
};

struct SNET_REQ_TEST_PACK : SNET_PACKET_HEAD
{
	char m_szMsg[MAX_PATH];


};

struct SNET_REQ_CREATE_OBJ : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CREATE_OBJ;
		m_iPacketSize = sizeof(SNET_REQ_CREATE_OBJ);
	}

	wstring m_wsID;
};

struct SNET_REQ_CREATE_OBJ_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CREATE_OBJ_FB;
		m_iPacketSize = sizeof(SNET_REQ_CREATE_OBJ_FB);
		m_iKillCount = 0;
	}

	CNetGameActor m_NetGameActor;
	wstring m_wsUserName;
	int m_iKillCount;
};

struct SNET_REQ_CREATE_PLAYER : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CREATE_PLAYER;
		m_iPacketSize = sizeof(SNET_REQ_CREATE_PLAYER);
		m_iCreatePlayerID = -1;
	}

	int m_iCreatePlayerID;
};

struct SNET_REQ_CREATE_PLAYER_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CREATE_PLAYER_FB;
		m_iPacketSize = sizeof(SNET_REQ_CREATE_PLAYER_FB);
		m_iPlayerNetObjID = -1;
	}

	int m_iPlayerNetObjID;
	SUnitObjData m_sPlayerData;
};

struct SNET_REQ_CHANGE_POS : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_POS;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_POS);
		m_iNetObjID = -1;
	}

	int m_iNetObjID;
	D3DXVECTOR2 m_vPos;
};

struct SNET_REQ_CHANGE_POS_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_POS_FB;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_POS_FB);
		m_iNetObjID = -1;
	}

	int m_iNetObjID;
	D3DXVECTOR2 m_vPos;
};

struct SNET_REQ_CHANGE_ROTATION : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_ROTATION;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_ROTATION);
		m_iNetObjID = -1;
		m_fRotation = -1.0f;
	}

	int m_iNetObjID;
	float m_fRotation;
};

struct SNET_REQ_CHANGE_ROTATION_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_ROTATION_FB;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_ROTATION_FB);
		m_iNetObjID = -1;
		m_fRotation = -1.0f;
	}

	int m_iNetObjID;
	float m_fRotation;
};

struct SNET_REQ_CHANGE_STATE :SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_STATE;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_STATE);
		m_iNetObjID = -1;
		m_eState = AT_NORMAL;
	}
	int m_iNetObjID;
	ESTATE m_eState;
};

struct SNET_REQ_CHANGE_STATE_FB :SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_STATE_FB;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_STATE_FB);
		m_iNetObjID = -1;
		m_eState = AT_NORMAL;
	}
	int m_iNetObjID;
	ESTATE m_eState;
};

struct SNET_REQ_DESTROY_OBJ : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_DESTROY_NETOBJ;
		m_iPacketSize = sizeof(SNET_REQ_DESTROY_OBJ);
		m_iNetObjID = -1;
	}

	int m_iNetObjID;
};

struct SNET_REQ_DESTROY_OBJ_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_DESTROY_NETOBJ_FB;
		m_iPacketSize = sizeof(SNET_REQ_DESTROY_OBJ_FB);
		m_iNetObjID = -1;
	}

	int m_iNetObjID;
};

struct SNET_REQ_CREATE_BULLET : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CREATE_BULLET;
		m_iPacketSize = sizeof(SNET_REQ_CREATE_BULLET);
		m_iNetObjID = -1;
	}

	int m_iNetObjID;
};

struct SNET_REQ_CREATE_BULLET_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CREATE_BULLET_FB;
		m_iPacketSize = sizeof(SNET_REQ_CREATE_BULLET);
	}

	CNetGameActor m_sNetGameObjData;
};

struct SNET_REQ_HEARTBEAT : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_HEARTBEAT;
		m_iPacketSize = sizeof(SNET_REQ_HEARTBEAT);
	}
};

struct SNET_REQ_HEARTBEAT_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_HEARTBEAT_FB;
		m_iPacketSize = sizeof(SNET_REQ_HEARTBEAT_FB);
	}
};

struct SNET_REQ_DO_HEARTBEAT_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_DO_HEARTBEAT_FB;
		m_iPacketSize = sizeof(SNET_REQ_DO_HEARTBEAT_FB);
	}
};

struct SNET_REQ_NETOBJSYNC : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_NETOBJSYNC;
		m_iPacketSize = sizeof(SNET_REQ_NETOBJSYNC);
	}
};

struct SNET_REQ_NETOBJSYNC_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_NETOBJSYNC_FB;
		m_iPacketSize = sizeof(SNET_REQ_NETOBJSYNC_FB);
		m_vPos = D3DXVECTOR2(0.0f, 0.0f);
		m_fRotation = 0.0f;
		m_eState = AT_NORMAL;
		m_iNetObjID = -1;

	}

	int m_iNetObjID;
	D3DXVECTOR2 m_vPos;
	float		m_fRotation;
	ESTATE      m_eState;
};

struct SNET_REQ_NETOBJSYNC_END_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_NETOBJSYNC_END_FB;
		m_iPacketSize = sizeof(SNET_REQ_NETOBJSYNC_END_FB);
	}
};

struct SNET_REQ_CHANGE_HP_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_HP_FB;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_HP_FB);
		m_iNetObjID = -1;
		m_iHp = -1;
	}

	int m_iNetObjID;
	int m_iHp;
};

struct SNET_REQ_CHANGE_KILLSCORE : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_KILLSCORE;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_KILLSCORE);
		m_iNetObjID = -1;
	}

	int m_iNetObjID;
};

struct SNET_REQ_CHANGE_KILLSCORE_FB : SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_CHANGE_KILLSCORE_FB;
		m_iPacketSize = sizeof(SNET_REQ_CHANGE_KILLSCORE_FB);
		m_iNetObjID = -1;
		m_iKillScore = -1;
	}

	int m_iNetObjID;
	int m_iKillScore;
};

struct SNET_REQ_RESPAWN_PLAYER :SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_RESPAWN_PLAYER;
		m_iPacketSize = sizeof(SNET_REQ_RESPAWN_PLAYER);
		m_iNetObjID = -1;
	}
	int m_iNetObjID;
};

struct SNET_REQ_RESPAWN_PLAYER_FB :SNET_PACKET_HEAD
{
	void Init()
	{
		m_iHeadID = SNET_HEAD_RESPAWN_PLAYER_FB;
		m_iPacketSize = sizeof(SNET_REQ_RESPAWN_PLAYER_FB);
		int m_iNetObjID = -1;
		m_eState = AT_NONE;
		m_iHp = 0;
		m_vPos = D3DXVECTOR2(0.0f, 0.0f);
	}

	int m_iNetObjID;
	ESTATE m_eState;
	int m_iHp;
	D3DXVECTOR2 m_vPos;
};