#pragma once

class CNetBulletActor : public CNetUnitActor, public CBullet
{
public:
	CNetBulletActor();
	virtual ~CNetBulletActor();

public:
	void SetBulletData(SBulletObjData* pBulletData);

public:
	void OnFrameMove(float fElapsedTime);

	virtual void OnAtkCollider(CActor* pDefTarget);
};

