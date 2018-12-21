
#include "../GlobalLogic/GlobalLogicHeader.h"

CActor::CActor() :
	m_vPos(0.0f, 0.0f),
	m_vScl(1.0f, 1.0f),
	m_fRotation(0.0f),
	m_iRenderTurn(-1),
	m_bCamera(true),
	m_iTeam(-1),
	m_bDestroy(false),
	m_eState(AT_NONE)
{
}

CActor::~CActor()
{
	vector< CCollider* >::iterator vit = m_vecCollider.begin();
	for (vit; vit != m_vecCollider.end(); ++vit)
	{
		(*vit)->Destroy();
		(*vit)->SetParent(nullptr);
	}
}
