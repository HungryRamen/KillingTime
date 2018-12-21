#include "DXUT.h"

CCutImgRender::CCutImgRender(CGameObj* pObj) :
	CRender(pObj),
	m_iTextureID(-1)
{
}

CCutImgRender::~CCutImgRender()
{
}

void CCutImgRender::Load(SRenderData* pRenderData)
{
	__super::Load(pRenderData);
	if (pRenderData == nullptr)
		return;

	m_iTextureID = D_TEXTUREMGR->Load(((SCutImgRenderData*)pRenderData)->m_wsImgFile);
	m_rcViewImg = { (LONG)((SCutImgRenderData*)pRenderData)->m_iLeft ,
		(LONG)((SCutImgRenderData*)pRenderData)->m_iTop,
		(LONG)((SCutImgRenderData*)pRenderData)->m_iRight,
		(LONG)((SCutImgRenderData*)pRenderData)->m_iBottom };
}

void CCutImgRender::MakeLocal(D3DXMATRIX& matLocal)
{
	if (m_pGameObj == nullptr)
		return;

	D3DXMatrixIdentity(&matLocal);
	const STextureInfo* pTextureInfo = D_TEXTUREMGR->GetTextureInfo(m_iTextureID);
	if (pTextureInfo == nullptr)
		return;

	matLocal._41 -= pTextureInfo->m_TextureInfo.Width * GetAnchor().x;
	matLocal._42 -= pTextureInfo->m_TextureInfo.Height * GetAnchor().y;
}

void CCutImgRender::UpdateWorld()
{
	D3DXMATRIX matRot;
	MakeRot(matRot);

	D3DXMATRIX matTrans;
	MakeTrans(matTrans);

	D3DXMATRIX matScale;
	MakeScale(matScale);

	D3DXMATRIX matLocal;
	MakeLocal(matLocal);

	m_matWorld = matLocal * matRot * matScale * matTrans;
	m_matWorld._43 = 0.0f;
}

void CCutImgRender::OnFrameMove(float fElapsedTime)
{
	__super::OnFrameMove(fElapsedTime);

	UpdateWorld();
}

void CCutImgRender::OnFrameRender()
{
	if (m_pGameObj == nullptr)
		return;

	D3DXMATRIX matCameraView;
	D3DXMatrixIdentity(&matCameraView);
	if (m_pGameObj->IsCamera() == true)
		D_CAMERAMGR->GetCameraViewTrans(&matCameraView);

	UpdateWorld();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	matWorld = m_matWorld * matCameraView;
	matWorld._43 = 0.0f;

	D_SPRITEMGR->SetTransform(&matWorld);
	D_SPRITEMGR->OnDraw(D_TEXTUREMGR->GetTexture(m_iTextureID), &m_rcViewImg, 0.0f, 0.0f, m_pGameObj->GetColor());
}