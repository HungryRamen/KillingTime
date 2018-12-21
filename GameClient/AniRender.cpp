#include "DXUT.h"

CAniRender::CAniRender(CGameObj* pGameObj) :
	CRender(pGameObj),
	m_iCurIndex(0),
	m_fTrackTime(0.0f),
	m_fTrackSpeed(0.0f),
	m_bEndAni(false),
	m_bLoop(false)
{
}


CAniRender::~CAniRender()
{
}

void CAniRender::Load(SRenderData* pRenderData)
{
	__super::Load(pRenderData);

	if (pRenderData == nullptr)
		return;

	m_fTrackSpeed = ((SAniRenderData*)pRenderData)->m_fTrackSpeed;
	m_bLoop = ((SAniRenderData*)pRenderData)->m_bLoop;

	for (int i = 0; i < (int)((SAniRenderData*)pRenderData)->m_vecImgFile.size(); ++i)
	{
		int iTexture = D_TEXTUREMGR->Load(((SAniRenderData*)pRenderData)->m_vecImgFile[i]);
		m_vecTextureID.push_back(iTexture);
	}
}

void CAniRender::MakeLocal(D3DXMATRIX &matLocal)
{
	if (m_pGameObj == nullptr)
		return;

	D3DXMatrixIdentity(&matLocal);
	const STextureInfo* pTextureInfo = D_TEXTUREMGR->GetTextureInfo(m_vecTextureID[m_iCurIndex]);
	if (pTextureInfo == nullptr)
		return;

	matLocal._41 -= pTextureInfo->m_TextureInfo.Width * GetAnchor().x;
	matLocal._42 -= pTextureInfo->m_TextureInfo.Height * GetAnchor().y;
}

void CAniRender::UpdateWorld()
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

void CAniRender::SetCurIndex(int iIndex)
{
	m_iCurIndex = iIndex;
	m_fTrackTime = 0.0f;
	m_bEndAni = false;
}

void CAniRender::OnFrameMove(float fElapsedTime)
{
	__super::OnFrameMove(fElapsedTime);

	m_fTrackTime += fElapsedTime;

	bool bChange = m_fTrackTime >= m_fTrackSpeed;
	if (bChange)
	{
		int iIndex = m_iCurIndex + 1;
		if (iIndex >= (int)m_vecTextureID.size())
		{
			if (m_bLoop)
				SetCurIndex(0);
			else
			{
				m_bEndAni = true;
			}
		}
		else
			SetCurIndex(iIndex);
	}

	UpdateWorld();
}

void CAniRender::OnFrameRender()
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
	D_SPRITEMGR->OnDraw(D_TEXTUREMGR->GetTexture(m_vecTextureID[m_iCurIndex]), m_pGameObj->GetRect(), 0.0f, 0.0f, m_pGameObj->GetColor());
}