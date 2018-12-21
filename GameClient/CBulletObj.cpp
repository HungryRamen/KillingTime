#include "DXUT.h"



CBulletObj::CBulletObj()
{

}


CBulletObj::~CBulletObj()
{
}

void CBulletObj::OnFrameMove(float fElapsedTime)
{
	__super::OnFrameMove(fElapsedTime);
	if (m_eState == AT_DIE)
	{
		if (((CAniRender*)GetRender())->IsEndAni())
		{
			Destroy();
		}
		return;
	}

	D3DXVECTOR2 vAnglePos(sin(D3DXToRadian(m_fRotation)), -cos(D3DXToRadian(m_fRotation)));
	m_vPos -= vAnglePos * (m_fSpeed * fElapsedTime);
}