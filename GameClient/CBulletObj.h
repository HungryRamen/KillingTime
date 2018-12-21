#pragma once
class CBulletObj : public CUnitObj, public CBullet
{
public:
	CBulletObj();
	virtual ~CBulletObj();

	virtual void OnFrameMove(float fElaspedTime);
};

