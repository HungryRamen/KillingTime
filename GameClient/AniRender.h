#pragma once

class CAniRender : public CRender
{
private:
	vector<int>m_vecTextureID;
	int m_iCurIndex;

	float m_fTrackTime;
	float m_fTrackSpeed;

	bool m_bEndAni;
	bool m_bLoop;

protected:
	virtual void MakeLocal(D3DXMATRIX& matLocal);

public:
	CAniRender(CGameObj* pGameObj);
	virtual ~CAniRender();

	bool IsEndAni() { return m_bEndAni; }
	void SetCurIndex(int iIndex);

	virtual void Load(SRenderData* pRenderData);
	virtual void OnFrameMove(float fElapsedTime);
	virtual void UpdateWorld();
	virtual void OnFrameRender();
};