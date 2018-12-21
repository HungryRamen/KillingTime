
#include "../GlobalLogic/GlobalLogicHeader.h"

float CUtil::CalcDist(CActor* pObj1, CActor* pObj2)
{
	D3DXVECTOR2 vLen = *pObj2->GetPos() - *pObj1->GetPos();

	return D3DXVec2Length(&vLen);
}

float CUtil::CalcDist(CActor* pObj, const D3DXVECTOR2* pPos)
{
	D3DXVECTOR2 vLen = *pObj->GetPos() - *pPos;
	return D3DXVec2Length(&vLen);
}

float CUtil::CalcDist(D3DXVECTOR2 pPos1, D3DXVECTOR2 pPos2)
{
	D3DXVECTOR2 vLen = pPos1 - pPos2;
	return D3DXVec2Length(&vLen);
}


bool CUtil::CheckCollision(CCircleCollider* pAtkCollider, CCircleCollider* pDefCollider)
{
	return (CalcDist(pAtkCollider->GetCollisionPos(), pDefCollider->GetCollisionPos()) < pAtkCollider->GetRadius() + pDefCollider->GetRadius());
}

bool CUtil::CheckCircleSqureCollision(CCircleCollider * pAtkCollider, CSquareCollider * pDefCollider)
{
	int iCircleX = (int)pAtkCollider->GetCollisionPos().x;
	int iCircleY = (int)pAtkCollider->GetCollisionPos().y;
	int iRadius = pAtkCollider->GetRadius();
	RECT rcRect = CreateRect(pDefCollider);
	if ((rcRect.left <= iCircleX && iCircleX <= rcRect.right) ||
		(rcRect.top <= iCircleY && iCircleY <= rcRect.bottom))
	{
		RECT rcDist = {
			rcRect.left - iRadius,
			rcRect.top - iRadius,
			rcRect.right + iRadius,
			rcRect.bottom + iRadius
		};

		if ((rcDist.left < iCircleX && iCircleX < rcDist.right) &&
			(rcDist.top < iCircleY && iCircleY < rcDist.bottom))
		{
			return true;
		}
	}
	else
	{
		if (CheckPointInCircle(pAtkCollider->GetCollisionPos(),(float)rcRect.left,(float)rcRect.top, iRadius)) //좌상단
			return true;
		if (CheckPointInCircle(pAtkCollider->GetCollisionPos(), (float)rcRect.left, (float)rcRect.bottom, iRadius)) //좌하단
			return true;
		if (CheckPointInCircle(pAtkCollider->GetCollisionPos(), (float)rcRect.right, (float)rcRect.top, iRadius)) //우상단
			return true;
		if (CheckPointInCircle(pAtkCollider->GetCollisionPos(), (float)rcRect.right, (float)rcRect.top, iRadius)) //우하단
			return true;
	}
	return false;
}

bool CUtil::CheckPointInCircle(D3DXVECTOR2 vPos1, float fX ,float fY, int iRadius)
{
	D3DXVECTOR2 vPos2(fX, fY);
	return (CalcDist(vPos1, vPos2) < iRadius);
}

RECT CUtil::CreateRect(CSquareCollider * pSqureObj)
{
	D3DXVECTOR2 vPos = pSqureObj->GetCollisionPos();

	float fWidth = (float)pSqureObj->GetWidth();
	float fHeight = (float)pSqureObj->GetHeight();

	RECT rcRect = { (LONG)(vPos.x)
		, (LONG)(vPos.y)
		, (LONG)(vPos.x + fWidth)
		, (LONG)(vPos.y + fHeight) };

	return rcRect;
}
