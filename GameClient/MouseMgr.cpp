#include "DXUT.h"

CMouseMgr* CMouseMgr::m_pMouseMgr = nullptr;

CMouseMgr::CMouseMgr() :
	m_bClick(false),
	m_vMousePos(0.0f, 0.0f),
	m_vCameraMousePos(0.0f, 0.0f)
{
}

CMouseMgr::~CMouseMgr()
{
}

void CMouseMgr::OnFrameMove(float fElapsedTime)
{
	m_bClick = false;

	if (DXUTIsKeyDown(VK_LBUTTON) == true)
		m_bClick = true;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(DXUTGetHWND(), &ptMouse);

	m_vMousePos = D3DXVECTOR2((float)ptMouse.x, (float)ptMouse.y);
	m_vCameraMousePos = D_CAMERAMGR->ChangeCameraPos(m_vMousePos);
}