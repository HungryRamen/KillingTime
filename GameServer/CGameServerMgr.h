#pragma once

class CGameServer;

class CGameServerMgr
{
private:
	static CGameServerMgr* m_pGameServerMgr;

private:
	CGameServer* m_pGameServer;		// 차후에 동적 배열로 수정.
	CNetGameActorMgr* m_pNetGameActorMgr;

private:
	boost::posix_time::ptime m_mstStart;
	boost::posix_time::time_duration m_mstDiff;

private:
	int m_iHeartBeatCount;
	bool m_bCheckTickTime;

public:
	static CGameServerMgr* GetGameServerMgr() {
		if (m_pGameServerMgr == nullptr) m_pGameServerMgr = new CGameServerMgr;
		return m_pGameServerMgr;
	}

public:
	CGameServerMgr();
	virtual ~CGameServerMgr();

	CGameServer* GetGameServer() {
		return m_pGameServer;
	}

	void SetGameserver(CGameServer* pGameServer) {
		m_pGameServer = pGameServer;
	}

	CNetGameActorMgr* GetNetGameActorMgr() {

		if (m_pNetGameActorMgr == nullptr)
			m_pNetGameActorMgr = new CNetGameActorMgr;

		return m_pNetGameActorMgr;
	}

	void OnNetFrameMove();

};

