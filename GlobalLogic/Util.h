#pragma once

class CActor;

namespace CUtil
{
	float CalcDist(CActor* pObj1, CActor* pObj2);
	float CalcDist(CActor* pObj, const D3DXVECTOR2* vPos);
	float CalcDist(D3DXVECTOR2 pPos1, D3DXVECTOR2 pPos2);

	bool CheckCollision(CCircleCollider* pAtkCollider, CCircleCollider* pDefCollider);
	bool CheckCircleSqureCollision(CCircleCollider* pAtkCollider, CSquareCollider* pDefCollider);
	bool CheckPointInCircle(D3DXVECTOR2 vPos1, float fX,float fY,int iRadius);

	RECT CreateRect(CSquareCollider* pSqureObj);
}
