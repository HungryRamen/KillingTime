#include "DXUT.h"


CHPBar::CHPBar() :
	m_pTargetUnitObj(nullptr)
{
}


CHPBar::~CHPBar()
{
}

const int CHPBar::Load(const wstring wsID)
{
	__super::Load(wsID);

	if (m_pTargetUnitObj != nullptr)
	{
		this->SetPos(
			D3DXVECTOR2(m_pTargetUnitObj->GetPos()->x,
				m_pTargetUnitObj->GetPos()->y - 32.0f)
		);
	}

	if (((CImgRender*)this->GetRender()) != nullptr)
	{
		int iTextureID = ((CImgRender*)this->GetRender())->GetTextureID();
		if (iTextureID != -1)
		{
			const STextureInfo* pTextureInfo = D_TEXTUREMGR->GetTextureInfo(iTextureID);
			if (pTextureInfo != nullptr)
			{
				m_iWIdth = pTextureInfo->m_TextureInfo.Width;
				m_iHeight = pTextureInfo->m_TextureInfo.Height;
			}
		}
	}

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

void CHPBar::OnFrameMove(float fElapsedTime)
{
	__super::OnFrameMove(fElapsedTime);

	if (m_pTargetUnitObj != nullptr)
	{
		this->SetPos(
			D3DXVECTOR2(m_pTargetUnitObj->GetPos()->x,
				m_pTargetUnitObj->GetPos()->y - 32.0f)
		);

		if (m_pTargetUnitObj->GetHP() <= 0 || m_pTargetUnitObj->GetMaxHP() == 0)
		{
			this->SetRect(0, m_iHeight);
		}
		else
		{
			float fHPPercent = (float)m_pTargetUnitObj->GetHP() / (float)m_pTargetUnitObj->GetMaxHP();
			this->SetRect((int)((float)m_iWIdth * fHPPercent), m_iHeight);
		}
	}
}

void CHPBar::OnFrameRender()
{
	if (m_pTargetUnitObj != nullptr)
	{
		if (*m_pTargetUnitObj->GetPos() != m_vPos)
		{
			this->SetPos(
				D3DXVECTOR2(m_pTargetUnitObj->GetPos()->x,
					m_pTargetUnitObj->GetPos()->y - 32.0f)
			);

			if (m_pTargetUnitObj->GetHP() <= 0 || m_pTargetUnitObj->GetMaxHP() == 0)
			{
				this->SetRect(0, m_iHeight);
			}
			else
			{
				float fHPPercent = (float)m_pTargetUnitObj->GetHP() / (float)m_pTargetUnitObj->GetMaxHP();
				this->SetRect((int)((float)m_iWIdth * fHPPercent), m_iHeight);
			}
		}
		__super::OnFrameRender();
	}
}