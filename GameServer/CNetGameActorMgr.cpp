#include "NetLogicHead.h"



CNetGameActorMgr::CNetGameActorMgr()
{
}


CNetGameActorMgr::~CNetGameActorMgr()
{
}

CNetGameActor* CNetGameActorMgr::AllocNetGameActor(const int iGameObjType)
{
	switch (iGameObjType)
	{
	case EGAMEOBJ::EGAMEOBJ_NORMAL:
		return new CNetGameActor;

	case EGAMEOBJ::EGAMEOBJ_UNIT:
		return new CNetUnitActor;

	case EGAMEOBJ::EGAMEOBJ_BULLET:
		return new CNetBulletActor;

	default:
		break;
	}

	return nullptr;
}

void CNetGameActorMgr::OnFrameMove(float fElapsedTime)
{
	std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.begin();
	for (mit; mit != m_mapNetGameObjData.end(); ++mit)
	{
		if (mit->second == nullptr)
			continue;

		if (*mit->second->IsDestroy() == true)
			m_vecTrashCan.push_back(mit->second);

		mit->second->OnFrameMove(fElapsedTime);
	}

	if (m_vecTrashCan.empty() == false)
	{
		for (int i = m_vecTrashCan.size() - 1; i >= 0; --i)
		{
			if (m_vecTrashCan[i] == nullptr)
				continue;

			DestroyNetGameObjData(m_vecTrashCan[i]->GetNetObjID());
		}

		m_vecTrashCan.clear();
	}

	// 이부분 위치 바꿔야함.
	D_COLLIDERMGR->OnFrameMove(fElapsedTime);
}