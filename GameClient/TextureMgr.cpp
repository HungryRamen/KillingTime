#include "DXUT.h"

CTextureMgr* CTextureMgr::m_pTextureMgr = nullptr;

CTextureMgr::CTextureMgr() :
	m_iTextureID(-1)
{
}


CTextureMgr::~CTextureMgr()
{
	DestroyAll();
}

const int CTextureMgr::Load(const wstring wsFile)
{
	map< wstring, STextureInfo* >::iterator mit = m_mapRefInfo.find(wsFile);
	if (mit != m_mapRefInfo.end())
	{
		++mit->second->m_iRefCount;

		return mit->second->m_iTextureID;
	}

	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO TextureInfo;

	if (FAILED(D3DXCreateTextureFromFileEx(DXUTGetD3D9Device()
		, wsFile.c_str()
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 1
		, 0
		, D3DFMT_A8R8G8B8
		, D3DPOOL_MANAGED
		, D3DX_FILTER_NONE
		, D3DX_FILTER_NONE
		, 0
		, &TextureInfo
		, nullptr
		, &pTexture)))
		return -1;

	++m_iTextureID;

	STextureInfo* pTextureInfo = new STextureInfo;
	pTextureInfo->m_iTextureID = m_iTextureID;
	pTextureInfo->m_wsFile = wsFile;
	pTextureInfo->m_pTexture = pTexture;
	pTextureInfo->m_TextureInfo = TextureInfo;

	pTextureInfo->m_iRefCount += 1;

	m_mapTextureInfo.insert(make_pair(m_iTextureID, pTextureInfo));
	m_mapRefInfo.insert(make_pair(wsFile, pTextureInfo));

	return m_iTextureID;
}

const STextureInfo* CTextureMgr::GetTextureInfo(const int iTextureID)
{
	map< int, STextureInfo* >::iterator mit = m_mapTextureInfo.find(iTextureID);
	if (mit == m_mapTextureInfo.end())
		return NULL;

	return mit->second;
}

const STextureInfo* CTextureMgr::GetTextureInfo(const wstring wsFile)
{
	map< wstring, STextureInfo* >::iterator mit = m_mapRefInfo.find(wsFile);
	if (mit == m_mapRefInfo.end())
		return NULL;

	return mit->second;
}

const LPDIRECT3DTEXTURE9 CTextureMgr::GetTexture(const int iTextureID)
{
	map< int, STextureInfo* >::iterator mit = m_mapTextureInfo.find(iTextureID);
	if (mit == m_mapTextureInfo.end())
		return NULL;

	return mit->second->m_pTexture;
}

const LPDIRECT3DTEXTURE9 CTextureMgr::GetTexture(const wstring wsFile)
{
	map< wstring, STextureInfo* >::iterator mit = m_mapRefInfo.find(wsFile);
	if (mit == m_mapRefInfo.end())
		return NULL;

	return mit->second->m_pTexture;
}

const int CTextureMgr::Destroy(const int iTextureID)
{
	map< int, STextureInfo* >::iterator mit = m_mapTextureInfo.find(iTextureID);
	if (mit == m_mapTextureInfo.end())
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	mit->second->m_iRefCount -= 1;
	if (mit->second->m_iRefCount > 0)
		return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;

	m_mapRefInfo.erase(mit->second->m_wsFile);

	SAFE_DELETE(mit->second);
	m_mapTextureInfo.erase(m_iTextureID);

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

const int CTextureMgr::DestroyAll()
{
	if (m_mapTextureInfo.empty() == true)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	map< int, STextureInfo* >::iterator mit = m_mapTextureInfo.begin();
	while (mit != m_mapTextureInfo.end())
	{
		SAFE_DELETE(mit->second);
		++mit;
	}

	m_mapTextureInfo.clear();
	m_mapRefInfo.clear();

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}