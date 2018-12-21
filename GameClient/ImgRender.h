#pragma once

class CGameObj;

class CImgRender : public CRender
{
private:
	int m_iTextureID;
protected:
	virtual void MakeLocal(D3DXMATRIX& matLocal);

public:
	CImgRender(CGameObj* pGameObj);
	virtual ~CImgRender();

	const int GetTextureID() {
		return m_iTextureID;
	}

	virtual void Load(SRenderData* pRenderData);
	virtual void OnFrameMove(float fElapsedTime);
	virtual void UpdateWorld();
	virtual void OnFrameRender();
};

