#include "DXUT.h"


CUnitObj::CUnitObj() :
	m_pHPBar(nullptr)
{
}


CUnitObj::~CUnitObj()
{
	if (m_pHPBar != nullptr)
		m_pHPBar->Destroy();
}

const int CUnitObj::Load(const wstring wsID)
{
	__super::Load(wsID);

	m_pHPBar = (CHPBar*)D_GAMEOBJMGR->CreateObj(EGAMEOBJ_HPBAR, L"HPBar", L"HPBar", D3DXVECTOR2(0.0f, 0.0f), 0.0f, true);
	if (m_pHPBar != nullptr)
		m_pHPBar->SetTargetObj(this);

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;

}

void CUnitObj::OnFrameMove(float fElapsedTime)
{
	__super::OnFrameMove(fElapsedTime);

	if (m_pController == nullptr)
	{
		if (this->GetName() == L"Bullet1")
			return;
		this->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
		this->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

}

void CUnitObj::OnFrameRender()
{
	__super::OnFrameRender();
}