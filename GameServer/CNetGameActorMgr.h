#pragma once
class CNetGameActorMgr : public  CNetGameActorContainer
{
private:
	vector< CNetGameActor* > m_vecTrashCan;


public:
	CNetGameActorMgr();
	virtual ~CNetGameActorMgr();

	virtual CNetGameActor* AllocNetGameActor(const int iGameObjType);
	virtual void OnFrameMove(float fElapsedTime);
};

