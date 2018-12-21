
#pragma once

struct SFontData
{
	LPD3DXFONT m_pFont;
	DWORD m_dFormat;
	int m_iFontSize;

	wstring m_wsString;
	D3DXCOLOR m_pColor;
	D3DXVECTOR2 m_vPos;

	SFontData() :
		m_pFont(NULL),
		m_dFormat(0),
		m_iFontSize(0),
		m_vPos(0.0f, 0.0f)
	{
		m_pColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	~SFontData()
	{
		SAFE_RELEASE(m_pFont);
	}

	void SetPos(D3DXVECTOR2 vPos) {
		m_vPos = vPos;
	}

	void DrawText();
};

class CFontMgr
{
private:
	static CFontMgr* m_pFontMgr;

private:
	map< wstring, SFontData* > m_mapFontData;
	wstring m_wsMyName;
	map<int, wstring>m_mapUserName;
	map<int, int>m_mapKill;
public:
	CFontMgr();
	virtual ~CFontMgr();

	static CFontMgr* GetFontMgr() { if (m_pFontMgr == NULL) m_pFontMgr = new CFontMgr; return m_pFontMgr; }

	void CreateFont(wstring wsName, wstring wsStr, D3DXVECTOR2 vPos, DWORD dFormat, int iFontSize, D3DXCOLOR pColor);

	void SetStr(wstring wsName, wstring wsStr);

	void Destroy(wstring wsName);
	void DestroyAll();

	void OnFrameRender();

	void SetMyName(wstring wsName) { m_wsMyName = wsName; }
	wstring GetMyName() {
		return m_wsMyName;
	}
	void UserKillPlus(int iNetObjID);

	void UserNameInsert(int iNetObjID, wstring wsName);
	void UserKillInsert(int iNetObjID, int iKillCount);
	wstring FindUserName(int iNetObjID);
	int FindUserKillCount(int iNetObjID);
	void UserNameDelete(int iNetObjID);
	void UserKillDelete(int iNetObjID);
	void ChangePos(wstring wsName, D3DXVECTOR2 vPos);

	void ViewKillScore();
};