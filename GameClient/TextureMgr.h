#pragma once

struct STextureInfo
{
	wstring m_wsFile;
	int m_iTextureID;
	int m_iRefCount;

	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXIMAGE_INFO m_TextureInfo;

	STextureInfo() :
		m_iTextureID(-1),
		m_iRefCount(0),
		m_pTexture(nullptr)
	{
	}

	~STextureInfo()
	{
		SAFE_RELEASE(m_pTexture);
	}
};

class CTextureMgr
{
private:
	static CTextureMgr* m_pTextureMgr;
private:
	map<int, STextureInfo*> m_mapTextureInfo;
	map<wstring, STextureInfo*>m_mapRefInfo;

	int m_iTextureID;

public:
	CTextureMgr();
	virtual ~CTextureMgr();

	static CTextureMgr* GetTextureMgr() {
		if (m_pTextureMgr == nullptr)m_pTextureMgr = new CTextureMgr;
		return m_pTextureMgr;
	}

	const int Load(const wstring wsFile);

	const STextureInfo* GetTextureInfo(const int iTextureID);
	const STextureInfo* GetTextureInfo(const wstring wsFile);

	const LPDIRECT3DTEXTURE9 GetTexture(const int iTextureID);
	const LPDIRECT3DTEXTURE9 GetTexture(const wstring wsFile);

	const int Destroy(const int iTextureID);

	const int DestroyAll();
};
