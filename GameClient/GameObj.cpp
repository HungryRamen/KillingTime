#include "DXUT.h"

CGameObj::CGameObj() :
	m_pController(nullptr),
	m_pRender(nullptr),
	m_iNetObjID(-1),
	m_prcRect(nullptr)
{
	m_d3dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}


CGameObj::~CGameObj()
{
}

CRender* CGameObj::RenderAlloc(int iRenderType)
{
	switch (iRenderType)
	{
	case ERENDERDATA_IMGRENDER:
		return new CImgRender(this);
	case ERENDERDATA_ANIRENDER:
		return new CAniRender(this);
	case ERENDERDATA_CUTIMGRENDER:
		return new CCutImgRender(this);
	default:
		break;
	}

	return nullptr;
}

const int CGameObj::Load(const wstring wsID)
{
	SGameObjData* pData = D_GAMEDATAMGR->GetGameObjData(wsID);

	SRenderData* pRenderData = D_GAMEDATAMGR->GetRenderData(pData->m_wsRenderDataID);
	if (pRenderData != nullptr)
	{
		CRender* pRender = RenderAlloc(pRenderData->m_iRenderType);
		pRender->Load(pRenderData);

		m_pRender = pRender;
	}

	if (pData->m_iGameDataType == EGAMEOBJ_UNIT || pData->m_iGameDataType == EGAMEOBJ_BULLET)
	{
		if (!((SStateGameObjData*)pData)->m_vecRenderDataID.empty())
		{
			for (int i = 0; i < (int)((SStateGameObjData*)pData)->m_vecRenderDataID.size(); ++i)
			{
				SRenderData* pStateRenderData = D_GAMEDATAMGR->GetRenderData(((SStateGameObjData*)pData)->m_vecRenderDataID[i]);
				if (pStateRenderData != nullptr)
				{
					CRender* pStateRender = RenderAlloc(pStateRenderData->m_iRenderType);
					pStateRender->Load(pStateRenderData);
					m_mapRender.insert(make_pair(((SAniRenderData*)pStateRenderData)->m_eAniState, pStateRender));
				}
			}
			m_mapRender.insert(make_pair(AT_NORMAL, m_pRender));
		}
	}

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

void CGameObj::OnFrameMove(float fElapsedTime)
{
	if (m_pController != nullptr)
		m_pController->OnFrameMove(fElapsedTime);

	if (this->GetRender() != nullptr)
		this->GetRender()->OnFrameMove(fElapsedTime);
}

void CGameObj::OnFrameRender()
{
	if (this->GetRender() != nullptr)
	{
		if (this->GetName() == L"Player")
		{
			wstringstream wsFont;
			wsFont << this->GetNetObjID();
			D_CAMERAMGR->PlayerNamePos(this,wsFont.str());
		}
		this->GetRender()->OnFrameRender();
	}
}

void CGameObj::ChangeState(const ESTATE eState, bool bNet)
{
	__super::ChangeState(eState);
	map<ESTATE, CRender*>::iterator mit = m_mapRender.find(eState);
	if (mit != m_mapRender.end())
	{
		m_pRender = mit->second;
		m_eState = eState;
	}
	if (m_pController != nullptr)
		m_pController->OnChangeState(eState, bNet);
}
