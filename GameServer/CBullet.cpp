#include "NetLogicHead.h"



CNetBulletActor::CNetBulletActor()
{
}


CNetBulletActor::~CNetBulletActor()
{
}

void CNetBulletActor::SetBulletData(SBulletObjData* pData)
{
	if (pData == nullptr)
		return;

	m_fLifeTime = pData->m_fLifeTime;
	m_fSpeed = pData->m_fSpeed;
	m_iDamage = pData->m_iDamage;
}

void CNetBulletActor::OnFrameMove(float fElapsedTime)
{
	m_fLifeTime -= fElapsedTime;
	if (m_fLifeTime <= 0.0f)
	{
		m_bDestroy = true;
		return;
	}

	D3DXVECTOR2 vAnglePos(sin(D3DXToRadian(m_fRotation)), -cos(D3DXToRadian(m_fRotation)));
	m_vPos -= vAnglePos * (m_fSpeed * fElapsedTime);
}

void CNetBulletActor::OnAtkCollider(CActor* pDefTarget)
{
	if (pDefTarget->GetGameObjType() == EGAMEOBJ_UNIT)
	{
		if (pDefTarget->GetState() == AT_DIE)
			return;
		((CNetUnitActor*)pDefTarget)->OnDamage(this->GetDamage());
		if (((CNetUnitActor*)pDefTarget)->GetHP() <= 0)
		{
			SNET_REQ_CHANGE_KILLSCORE_FB* pChangePacket = new SNET_REQ_CHANGE_KILLSCORE_FB;
			pChangePacket->Init();
			pChangePacket->m_iNetObjID = D_GAMESERVERMGR->GetNetGameActorMgr()->BulletUserSerch(this->GetTeam());
			D_GAMESERVERMGR->GetGameServer()->iPlusSessionUserKillCount(this->GetMasterSessionID());
			D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(true, pChangePacket->m_iPacketSize, (char*)pChangePacket);
		}
		SNET_REQ_CHANGE_HP_FB* pSendPacket = new SNET_REQ_CHANGE_HP_FB;
		pSendPacket->Init();
		pSendPacket->m_iNetObjID = ((CNetUnitActor*)pDefTarget)->GetNetObjID();
		pSendPacket->m_iHp = ((CNetUnitActor*)pDefTarget)->GetHP();
		D_GAMESERVERMGR->GetGameServer()->PostSendAllSession(true, pSendPacket->m_iPacketSize, (char*)pSendPacket);
		Destroy();
	}
	else if (pDefTarget->GetGameObjType() == EGAMEOBJ_NORMAL)
	{
		Destroy();
	}
}