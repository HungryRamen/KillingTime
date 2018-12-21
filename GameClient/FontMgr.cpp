
#include "DXUT.h"

CFontMgr* CFontMgr::m_pFontMgr = NULL;

void SFontData::DrawText()
{
	RECT rcRect = { (LONG)(m_vPos.x)
		, (LONG)(m_vPos.y)
		, -1
		, -1 };

	m_pFont->DrawText(NULL
		, m_wsString.c_str()
		, -1
		, &rcRect
		, m_dFormat | DT_NOCLIP
		, m_pColor);
}

CFontMgr::CFontMgr()
{

}

CFontMgr::~CFontMgr()
{
	DestroyAll();
}

void CFontMgr::DestroyAll()
{
	for each(map< wstring, SFontData* >::const_reference mr in m_mapFontData)
	{
		delete mr.second;
	}

	m_mapFontData.clear();
}

void CFontMgr::CreateFont(wstring wsName, wstring wsStr, D3DXVECTOR2 vPos, DWORD dFormat, int iFontSize, D3DXCOLOR pColor)
{
	map< wstring, SFontData* >::iterator mit = m_mapFontData.find(wsName);

	if (mit != m_mapFontData.end())
		return;

	LPD3DXFONT pFont;

	if (FAILED(D3DXCreateFont(DXUTGetD3D9Device()
		, iFontSize
		, 0
		, FW_BOLD
		, 1
		, false
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS
		, ANTIALIASED_QUALITY
		, FF_DONTCARE
		, L"±¼¸²Ã¼"
		, &pFont)))
		return;

	SFontData* pFontData = new SFontData;
	pFontData->m_dFormat = dFormat;
	pFontData->m_iFontSize = iFontSize;
	pFontData->m_pColor = pColor;
	pFontData->m_pFont = pFont;
	pFontData->m_vPos = vPos;
	pFontData->m_wsString = wsStr;

	m_mapFontData.insert(make_pair(wsName, pFontData));
}

void CFontMgr::Destroy(wstring wsName)
{
	map< wstring, SFontData* >::iterator mit = m_mapFontData.find(wsName);
	if (mit == m_mapFontData.end())
		return;

	SAFE_DELETE(mit->second);
	m_mapFontData.erase(mit);
}

void CFontMgr::OnFrameRender()
{
	map< wstring, SFontData* >::iterator mit = m_mapFontData.begin();
	while (mit != m_mapFontData.end())
	{
		mit->second->DrawText();
		++mit;
	}
}

void CFontMgr::UserKillPlus(int iNetObjID)
{
	map<int, int>::iterator mit = m_mapKill.find(iNetObjID);

	if (mit == m_mapKill.end())
		return;

	mit->second++;
}

void CFontMgr::UserNameInsert(int iNetObjID, wstring wsName)
{
	map<int, wstring>::iterator mit = m_mapUserName.find(iNetObjID);

	if (mit != m_mapUserName.end())
		return;

	m_mapUserName.insert(make_pair(iNetObjID, wsName));
}

void CFontMgr::UserKillInsert(int iNetObjID, int iKillCount)
{
	map<int, int>::iterator mit = m_mapKill.find(iNetObjID);

	if (mit != m_mapKill.end())
		return;

	m_mapKill.insert(make_pair(iNetObjID, iKillCount));
}

wstring CFontMgr::FindUserName(int iNetObjID)
{
	map<int, wstring>::iterator mit = m_mapUserName.find(iNetObjID);

	if (mit == m_mapUserName.end())
		return L"";

	return mit->second;
}

int CFontMgr::FindUserKillCount(int iNetObjID)
{
	map<int, int>::iterator mit = m_mapKill.find(iNetObjID);

	if (mit == m_mapKill.end())
		return 0;

	return mit->second;
}

void CFontMgr::UserNameDelete(int iNetObjID)
{
	map<int, wstring>::iterator mit = m_mapUserName.find(iNetObjID);

	if (mit == m_mapUserName.end())
		return;

	m_mapUserName.erase(mit);
}

void CFontMgr::UserKillDelete(int iNetObjID)
{
	map<int, int>::iterator mit = m_mapKill.find(iNetObjID);

	if (mit == m_mapKill.end())
		return;

	m_mapKill.erase(mit);
}

void CFontMgr::ChangePos(wstring wsName, D3DXVECTOR2 vPos)
{
	map<wstring, SFontData*>::iterator mit = m_mapFontData.find(wsName);
	if (mit == m_mapFontData.end())
		return;

	mit->second->SetPos(vPos);
}

void CFontMgr::ViewKillScore()
{
	wstringstream Score;
	if (DXUTIsKeyDown(VK_TAB))
	{

		map<int, int>::iterator mit = m_mapKill.begin();


		while (mit != m_mapKill.end())
		{
			map<int, wstring>::iterator mit2 = m_mapUserName.find(mit->first);
			if (mit2 == m_mapUserName.end())
			{
				++mit;
				continue;
			}
			Score << mit2->second << L":" << mit->second << L"\n";
			++mit;
		}
	}
	D_FONT->SetStr(L"Score", Score.str());
}

void CFontMgr::SetStr(wstring wsName, wstring wsStr)
{
	map< wstring, SFontData* >::iterator mit = m_mapFontData.find(wsName);
	if (mit == m_mapFontData.end())
		return;

	mit->second->m_wsString = wsStr;
}