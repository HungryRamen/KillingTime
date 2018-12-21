#include "NetLogicHead.h"

CGameServerMgr* CGameServerMgr::m_pGameServerMgr = nullptr;

CGameServerMgr::CGameServerMgr() :
	m_pGameServer(nullptr),
	m_pNetGameActorMgr(nullptr),
	m_iHeartBeatCount(0),
	m_bCheckTickTime(false)
{
}


CGameServerMgr::~CGameServerMgr()
{
}

void CGameServerMgr::OnNetFrameMove()
{
	if (m_bCheckTickTime == false)
	{
		m_mstStart = boost::posix_time::microsec_clock::local_time();
		m_bCheckTickTime = true;

		if (m_pNetGameActorMgr != nullptr)
			m_pNetGameActorMgr->OnFrameMove(0.001f);
	}
	else
	{
		boost::posix_time::ptime mstNow = boost::posix_time::microsec_clock::local_time();
		m_mstDiff = mstNow - m_mstStart;
		int64_t imili = m_mstDiff.total_milliseconds();
		float fElapsedTime = m_mstDiff.total_milliseconds() * 0.001f;
		m_mstStart = mstNow;

		if (m_pNetGameActorMgr != nullptr)
			m_pNetGameActorMgr->OnFrameMove(fElapsedTime);
	}
}