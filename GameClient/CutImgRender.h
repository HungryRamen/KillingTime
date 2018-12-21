#pragma once

class CCutImgRender : public CRender
{
private:
	int m_iTextureID;
	RECT m_rcViewImg;

protected:
	virtual void MakeLocal(D3DXMATRIX &matLocal);

public:
	CCutImgRender(CGameObj* pGameObj);
	virtual ~CCutImgRender();

	const int GetTextureID() {
		return m_iTextureID;
	}

	virtual void Load(SRenderData* pRenderData);
	virtual void OnFrameMove(float fElapsedTime);
	virtual void UpdateWorld();
	virtual void OnFrameRender();
};