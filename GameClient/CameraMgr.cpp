#include "DXUT.h"

CCameraMgr* CCameraMgr::m_pCameraMgr = nullptr;

CCameraMgr::CCameraMgr() :
	m_iWidth(0),
	m_iHeight(0),
	m_vCameraPos(0.0f, 0.0f),
	m_pTargetObj(nullptr),
	m_eCameraLR(ECAMERA_CENTER),
	m_eCameraUD(ECAMERA_CENTER)
{
}


CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::InitSize(const int  iWidth, const int  iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_vCameraPos.x = m_iWidth* 0.5f;
	m_vCameraPos.y = m_iHeight * 0.5f;
}

D3DXVECTOR2 CCameraMgr::ChangeCameraPos(D3DXVECTOR2 vPos)
{
	D3DXVECTOR2 vCameraPos;
	vCameraPos.x = vPos.x + m_vCameraPos.x - (m_iWidth * 0.5f);
	vCameraPos.y = vPos.y + m_vCameraPos.y - (m_iHeight * 0.5f);

	return vCameraPos;
}

void CCameraMgr::OnFrameMove(const float fElapsedTime)
{
	if (m_pTargetObj != nullptr)
	{
		m_vCameraPos = *m_pTargetObj->GetPos();
		m_eCameraLR = ECAMERA_CENTER;
		m_eCameraUD = ECAMERA_CENTER;
		if (m_vCameraPos.x - m_iWidth* 0.5f <= 0.0f)
		{
			m_vCameraPos.x = m_iWidth* 0.5f;
			m_eCameraLR = ECAMERA_LEFT;
		}
		else if (m_vCameraPos.x + m_iWidth* 0.5f >= D_GAMETILEMGR->GetTileWidth() * D_GAMETILEMGR->GetOffSetX())
		{
			m_vCameraPos.x = D_GAMETILEMGR->GetTileWidth() * D_GAMETILEMGR->GetOffSetX() - m_iWidth * 0.5f;
			m_eCameraLR = ECAMERA_RIGHT;
		}
		if (m_vCameraPos.y - m_iHeight * 0.5f <= 0.0f)
		{
			m_vCameraPos.y = m_iHeight* 0.5f;
			m_eCameraUD = ECAMERA_UP;
		}
		else if (m_vCameraPos.y + m_iHeight* 0.5f >= D_GAMETILEMGR->GetTileHeight() * D_GAMETILEMGR->GetOffSetY())
		{
			m_vCameraPos.y = D_GAMETILEMGR->GetTileHeight() * D_GAMETILEMGR->GetOffSetY() - m_iHeight* 0.5f;
			m_eCameraUD = ECAMERA_DOWN;
		}
	}
}

void CCameraMgr::GetCameraViewTrans(D3DXMATRIX* pmatTrans)
{
	pmatTrans->_41 = (float)m_iWidth * 0.5f - m_vCameraPos.x;
	pmatTrans->_42 = (float)m_iHeight * 0.5f - m_vCameraPos.y;
	pmatTrans->_43 = 0.0f;
}

void CCameraMgr::PlayerNamePos(CGameObj* pObj,wstring wsFontName)
{
	bool bMyUnit = false;
	if (m_pTargetObj == pObj)
		bMyUnit = true;
	float fX = D_CAMERAMGR->GetWidth() * 0.5f - 50.0f;
	float fY = D_CAMERAMGR->GetHeight() * 0.5f - 60.0f;
	if (bMyUnit)
	{
		switch (m_eCameraLR)
		{
		case ECAMERA_LEFT:
			fX = pObj->GetPos()->x - 50.0f;
			break;
		case ECAMERA_RIGHT:
			fX += pObj->GetPos()->x - (D_GAMETILEMGR->GetTileWidth() * D_GAMETILEMGR->GetOffSetX() - m_iWidth * 0.5f);
			break;
		}
		switch (m_eCameraUD)
		{
		case ECAMERA_UP:
			fY = pObj->GetPos()->y - 60.0f;
			break;
		case ECAMERA_DOWN:
			fY += pObj->GetPos()->y - (D_GAMETILEMGR->GetTileHeight() * D_GAMETILEMGR->GetOffSetY() - m_iHeight* 0.5f);
			break;
		}
	}
	else
	{
		fX += pObj->GetPos()->x - m_vCameraPos.x;
		fY += pObj->GetPos()->y - m_vCameraPos.y;
	}
	D_FONT->ChangePos(wsFontName, D3DXVECTOR2(fX, fY));

}
