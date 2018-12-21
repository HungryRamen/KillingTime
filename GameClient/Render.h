#pragma once
class CRender
{
protected:
	D3DXMATRIX m_matWorld;
	CGameObj* m_pGameObj;

	D3DXVECTOR2 m_vAnchor;

protected:
	virtual void MakeRot(D3DXMATRIX& matRot);
	virtual void MakeTrans(D3DXMATRIX& matTrans);
	virtual void MakeScale(D3DXMATRIX& matScale);
	virtual void MakeLocal(D3DXMATRIX& matLocal) { D3DXMatrixIdentity(&matLocal); }

public:
	D3DXVECTOR2 const GetAnchor() {
		return m_vAnchor;
	}

	void SetAnchor(D3DXVECTOR2 vAnchor)
	{
		m_vAnchor = vAnchor;
	}

public:
	CRender(CGameObj* pObj);
	virtual ~CRender();

	virtual void Load(const wstring wsID) {}
	virtual void Load(SRenderData* pRenderData);

	virtual void UpdateWorld() {}
	virtual void OnFrameMove(float fElapsedTime) {}
	virtual void OnFrameRender() {}
};

