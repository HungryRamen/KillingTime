
#include "../GlobalLogic/GlobalLogicHeader.h"

CColliderMgr* CColliderMgr::m_pColliderMgr = nullptr;

CColliderMgr::CColliderMgr()
{
}


CColliderMgr::~CColliderMgr()
{
}

CCollider* CColliderMgr::AllocCollider(int iColliderModel)
{
	switch (iColliderModel)
	{
	case ECOLLIDER_CIRCLE:
		return new CCircleCollider;

	case ECOLLIDER_SQUARE:
		return new CSquareCollider;
	}

	return nullptr;
}

const CCollider* CColliderMgr::CreateCollider(CActor* pParent, SColliderData* pColliderData)
{
	if (pParent == nullptr || pColliderData == nullptr)
		return nullptr;

	CCollider* pCollider = AllocCollider(pColliderData->m_iColliderModel);
	if (pCollider == nullptr)
		return nullptr;

	pCollider->SetParent(pParent);
	pCollider->SetColliderModel(pColliderData->m_iColliderModel);
	pCollider->SetCollierType(pColliderData->m_iColliderType);
	pCollider->SetPos(pColliderData->m_vPos);

	pParent->PushCollider(pCollider);

	switch (pColliderData->m_iColliderModel)
	{
	case ECOLLIDER_CIRCLE:
		((CCircleCollider*)pCollider)->SetRaidus(((SCircleColliderData*)pColliderData)->m_iRadius);
		break;

	case ECOLLIDER_SQUARE:
		((CSquareCollider*)pCollider)->SetWidth(((SSquareColliderData*)pColliderData)->m_iWidth);
		((CSquareCollider*)pCollider)->SetHeight(((SSquareColliderData*)pColliderData)->m_iHeight);
		break;
	}

	if (pCollider->GetColliderType() == ECOLLIDER_ATTACK)
	{
		m_vecAtkCollider.push_back(pCollider);
	}
	else if (pCollider->GetColliderType() == ECOLLIDER_DEFENCE)
	{
		m_vecDefCollider.push_back(pCollider);
	}

	return pCollider;
}

void CColliderMgr::OnFrameMove(float fElaspedTime)
{
	ProcessCollider();
}

void CColliderMgr::ProcessCollider()
{
	for (int iAtk = m_vecAtkCollider.size() - 1; iAtk >= 0; --iAtk)
	{
		if (*m_vecAtkCollider[iAtk]->IsDestroy() == true)
		{
			m_vecAtkCollider.erase(m_vecAtkCollider.begin() + iAtk);
			continue;
		}

		for (int iDef = m_vecDefCollider.size() - 1; iDef >= 0; --iDef)
		{
			if (*m_vecDefCollider[iDef]->IsDestroy() == true)
			{
				m_vecDefCollider.erase(m_vecDefCollider.begin() + iDef);
				continue;
			}

			if (m_vecAtkCollider[iAtk]->GetParent() == m_vecDefCollider[iDef]->GetParent())
				continue;

			if (m_vecAtkCollider[iAtk]->GetParent()->GetTeam() == m_vecDefCollider[iDef]->GetParent()->GetTeam())
				continue;

			if (m_vecAtkCollider[iAtk]->GetGolliderModel() == ECOLLIDER_CIRCLE &&
				m_vecDefCollider[iDef]->GetGolliderModel() == ECOLLIDER_CIRCLE)
			{
				if (IsCircleCollisionCheck(m_vecAtkCollider[iAtk], m_vecDefCollider[iDef]) == false)
					continue;

				m_vecAtkCollider[iAtk]->GetParent()->OnAtkCollider(m_vecDefCollider[iDef]->GetParent());
				m_vecDefCollider[iDef]->GetParent()->OnDefCollider(m_vecAtkCollider[iAtk]->GetParent());
			}
			if (m_vecAtkCollider[iAtk]->GetGolliderModel() == ECOLLIDER_CIRCLE &&
				m_vecDefCollider[iDef]->GetGolliderModel() == ECOLLIDER_SQUARE)
			{
				if (IsCircleSqureCollisionCheck(m_vecAtkCollider[iAtk], m_vecDefCollider[iDef]) == false)
					continue;

				m_vecAtkCollider[iAtk]->GetParent()->OnAtkCollider(m_vecDefCollider[iDef]->GetParent());
			}
		}
	}
}

bool CColliderMgr::IsCircleCollisionCheck(CCollider* pAtk, CCollider* pDef)
{
	if (CUtil::CheckCollision((CCircleCollider*)pAtk, (CCircleCollider*)pDef) == true)
		return true;

	return false;
}

bool CColliderMgr::IsCircleSqureCollisionCheck(CCollider * pAtk, CCollider * pDef)
{
	if (CUtil::CheckCircleSqureCollision((CCircleCollider*)pAtk, (CSquareCollider*)pDef) == true)
		return true;

	return false;
}
