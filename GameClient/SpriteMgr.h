#pragma once

class CSpriteMgr
{
private:
	static CSpriteMgr* m_pSpriteMgr;
	LPD3DXSPRITE       m_pD3DSprite;

public:
	CSpriteMgr();
	virtual ~CSpriteMgr();

	static CSpriteMgr* GetSpriteMgr() {
		if (m_pSpriteMgr == nullptr)m_pSpriteMgr = new CSpriteMgr;
		return m_pSpriteMgr;
	}

	LPD3DXSPRITE GetD3DSprite() { return m_pD3DSprite; }

	const int OnResetDevice();
	void OnLostDevice();


	const int OnBegin();
	const int OnEnd();

	const int SetTransform(const D3DXMATRIX* pmatTrans);
	const int OnDraw(LPDIRECT3DTEXTURE9 pTexture
		, const RECT* pRect
		, const float fX
		, const float fY);

	const int OnDraw(LPDIRECT3DTEXTURE9 pTexture
		, const RECT* pRect
		, const float fX
		, const float fY
		, D3DXCOLOR pColor);
};