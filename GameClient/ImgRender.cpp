#include "DXUT.h"

CImgRender::CImgRender(CGameObj* pGameObj) :
	CRender(pGameObj),
	m_iTextureID(-1)
{
}

CImgRender::~CImgRender()
{
}

void CImgRender::Load(SRenderData* pRenderData)
{
	__super::Load(pRenderData);

	if (pRenderData == nullptr)
		return;

	m_iTextureID = D_TEXTUREMGR->Load(((SImgRenderData*)pRenderData)->m_wsImgFile);

}

void CImgRender::MakeLocal(D3DXMATRIX &matLocal)
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

void CImgRender::UpdateWorld()
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

void CImgRender::OnFrameMove(float fElapsedTime)
{
	__super::OnFrameMove(fElapsedTime);

	UpdateWorld();
}

void CImgRender::OnFrameRender()
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
	D_SPRITEMGR->OnDraw(D_TEXTUREMGR->GetTexture(m_iTextureID), m_pGameObj->GetRect(), 0.0f, 0.0f, m_pGameObj->GetColor());
}