#include "DXUT.h"


CRender::CRender(CGameObj* pGameObj) :
	m_pGameObj(pGameObj),
	m_vAnchor(0.0f, 0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


CRender::~CRender()
{
}

void CRender::Load(SRenderData* pRenderData)
{
	m_vAnchor = pRenderData->m_vAnchor;
}

void CRender::MakeRot(D3DXMATRIX& matRot)
{
	if (m_pGameObj == nullptr)
		return;

	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(*m_pGameObj->GetRotation()));
}

void CRender::MakeTrans(D3DXMATRIX& matTrans)
{
	if (m_pGameObj == nullptr)
		return;

	D3DXMatrixIdentity(&matTrans);
	matTrans._41 = m_pGameObj->GetPos()->x;
	matTrans._42 = m_pGameObj->GetPos()->y;
	matTrans._43 = 0.0f;
}

void CRender::MakeScale(D3DXMATRIX& matScale)
{
	if (m_pGameObj == nullptr)
		return;

	D3DXMatrixIdentity(&matScale);
	matScale._11 = m_pGameObj->GetScl()->x;
	matScale._22 = m_pGameObj->GetScl()->y;
}