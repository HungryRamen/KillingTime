#include "DXUT.h"

CSpriteMgr* CSpriteMgr::m_pSpriteMgr = nullptr;

CSpriteMgr::CSpriteMgr() :
	m_pD3DSprite(nullptr)
{
}

CSpriteMgr::~CSpriteMgr()
{
	if (m_pD3DSprite != nullptr)
		SAFE_DELETE(m_pD3DSprite);
}

const int CSpriteMgr::OnResetDevice()
{
	D3DXCreateSprite(DXUTGetD3D9Device(), &m_pD3DSprite);

	if (m_pD3DSprite == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
	else
		return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

void CSpriteMgr::OnLostDevice()
{
	SAFE_RELEASE(m_pD3DSprite);
}


const int CSpriteMgr::OnBegin()
{
	if (m_pD3DSprite == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

const int CSpriteMgr::OnEnd()
{
	if (m_pD3DSprite == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	m_pD3DSprite->End();
	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

const int CSpriteMgr::SetTransform(const D3DXMATRIX* pmatTrans)
{
	if (m_pD3DSprite == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	m_pD3DSprite->SetTransform(pmatTrans);
	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

const int CSpriteMgr::OnDraw(LPDIRECT3DTEXTURE9 pTexture
	, const RECT* pRect
	, const float fX
	, const float fY
)
{
	return	OnDraw(pTexture, pRect, fX, fY, 0xffffffff);
}

const int CSpriteMgr::OnDraw(LPDIRECT3DTEXTURE9 pTexture
	, const RECT* pRect
	, const float fX
	, const float fY
	, D3DXCOLOR pColor)
{
	if (m_pD3DSprite == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	D3DXVECTOR3 vPos(fX, fY, 0.0f);
	m_pD3DSprite->Draw(pTexture, pRect, nullptr, &vPos, pColor);
	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}
