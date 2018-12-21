#pragma once

using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif   

#ifndef D3DXVECTOR2
#include "d3dx9math.h"
#endif

enum EGAMEOBJ
{
	EGAMEOBJ_NORMAL,
	EGAMEOBJ_UNIT,
	EGAMEOBJ_BULLET,
	EGAMEOBJ_HPBAR,
};

enum EGAMEOBJDATA_TYPE
{
	EGAMEOBJDATA_NORMAL = EGAMEOBJ_NORMAL,
	EGAMEOBJDATA_UNIT = EGAMEOBJ_UNIT,
	EGAMEOBJDATA_BULLET = EGAMEOBJ_BULLET,

};

enum ESTATE
{
	AT_NONE = -1,
	AT_NORMAL,
	AT_MOVE,
	AT_ATK,
	AT_DIE,
};

enum ERENDERDATA_TYPE
{
	ERENDERDATA_IMGRENDER,
	ERENDERDATA_ANIRENDER,
	ERENDERDATA_CUTIMGRENDER,
};

enum ECOLLIDER_TYPE
{
	ECOLLIDER_ATTACK,
	ECOLLIDER_DEFENCE,
};

enum ECOLLIDER_MODEL
{
	ECOLLIDER_CIRCLE,
	ECOLLIDER_SQUARE,
};

struct SColliderData
{
	int m_iColliderType;
	int m_iColliderModel;
	D3DXVECTOR2 m_vPos;
	int m_iColliderNum;

	SColliderData() :
		m_iColliderType(-1),
		m_iColliderModel(-1),
		m_vPos(0.0f, 0.0f),
		m_iColliderNum(-1)
	{
	}

	virtual void Load(wifstream* wifRead);
};

#include "../GlobalLogic/Actor.h"

struct SCircleColliderData : SColliderData
{
	int m_iRadius;

	SCircleColliderData() :
		m_iRadius(-1)
	{
	}

	virtual void Load(wifstream* wifRead);
};

struct SSquareColliderData : SColliderData
{
	int m_iWidth;
	int m_iHeight;

	SSquareColliderData() :
		m_iWidth(-1),
		m_iHeight(-1)
	{
	}

	virtual void Load(wifstream* wifRead);
};

struct SGameObjData
{
	wstring m_wsID;
	wstring m_wsFile;

	int m_iGameDataType;

	wstring m_wsRenderDataID;
	bool m_bCamera;

	SGameObjData() :
		m_iGameDataType(-1),
		m_bCamera(false)
	{
	}

	~SGameObjData()
	{
		vector<SColliderData*>::iterator vitAtk = m_vecATKCollider.begin();
		for (vitAtk; vitAtk != m_vecATKCollider.end(); ++vitAtk)
			SAFE_DELETE((*vitAtk));

		vector<SColliderData*>::iterator vitDef = m_vecDEFCollider.begin();
		for (vitDef; vitDef != m_vecATKCollider.end(); ++vitDef)
			SAFE_DELETE((*vitDef));

		m_vecATKCollider.clear();
		m_vecDEFCollider.clear();
	}

	vector<SColliderData*> m_vecATKCollider;
	vector<SColliderData*> m_vecDEFCollider;

	virtual void Load(wifstream* wifRead);
};

struct SStateGameObjData : public SGameObjData
{
	vector<wstring> m_vecRenderDataID;

	virtual void Load(wifstream* wifRead);
};

struct SUnitObjData : public SStateGameObjData
{
	int m_iHp;
	int m_iDamage;
	float m_fSpeed;

	SUnitObjData() :
		m_fSpeed(0.0f),
		m_iHp(0),
		m_iDamage(0)
	{
		m_iGameDataType = EGAMEOBJDATA_UNIT;
	}

	virtual void Load(wifstream* wifRead);
};

// 이부분에 플레이어랑 공통된 부분은 차후 제거하자.
struct SBulletObjData : public SUnitObjData
{
	float m_fLifeTime;

	SBulletObjData() :
		m_fLifeTime(0.0f)
	{
		m_iGameDataType = EGAMEOBJDATA_BULLET;
	}

	virtual void Load(wifstream* wifRead);
};


struct SRenderData
{
	int m_iRenderType;
	D3DXVECTOR2 m_vAnchor;

	wstring m_wsID;
	wstring m_wsFile;

	SRenderData() :
		m_iRenderType(-1),
		m_vAnchor(0.0f, 0.0f)
	{
	}

	virtual void Load(wifstream* wifRead);
};

struct SImgRenderData : public SRenderData
{
public:
	wstring m_wsImgFile;

	virtual void Load(wifstream* wifRead);
};

struct SAniRenderData : public SRenderData
{
public:
	int m_iImgCount;
	float m_fTrackSpeed;
	bool m_bLoop;
	ESTATE m_eAniState;
	SAniRenderData() :
		m_iImgCount(0),
		m_fTrackSpeed(0.0f),
		m_bLoop(false),
		m_eAniState(AT_NONE)
	{
	}
	vector<wstring>m_vecImgFile;
	virtual void Load(wifstream* wifRead);
};

struct SCutImgRenderData : public SRenderData
{
public:
	wstring m_wsImgFile;
	int m_iLeft;
	int m_iTop;
	int m_iRight;
	int m_iBottom;

	SCutImgRenderData() :
		m_iLeft(-1),
		m_iTop(-1),
		m_iRight(-1),
		m_iBottom(-1)
	{
	}

	virtual void Load(wifstream* wifRead);
};

struct STileData
{
	int m_iTileID;
	wstring m_wsFile;
	bool m_bWall;
};

struct STileObjData
{
	int m_iTileDataID;
	int m_iTileType;
	int m_iTileIndex;

	STileObjData() :
		m_iTileDataID(-1),
		m_iTileType(-1),
		m_iTileIndex(-1)
	{
	}
};

struct STileMapData_NoneVector
{
	int m_iWidth;
	int m_iHeight;
	int m_iTileSize;

	STileMapData_NoneVector() :
		m_iWidth(-1),
		m_iHeight(-1),
		m_iTileSize(-1)
	{
	}
};

struct STileMapData :STileMapData_NoneVector
{
	vector<STileObjData>m_vecTileObjData;
};

class CGameDataMgr
{
private:
	static CGameDataMgr* m_pGameDataMgr;

private:
	map< wstring, SGameObjData* >	m_mapGameObjData;
	map< wstring, SRenderData* >	m_mapRenderData;

public:
	CGameDataMgr();
	virtual ~CGameDataMgr();

	static CGameDataMgr* GetGameDataMgr() {
		if (m_pGameDataMgr == nullptr) m_pGameDataMgr = new CGameDataMgr();
		return m_pGameDataMgr;
	}

	void LoadData();

	SGameObjData* AllocData(int iType);
	SRenderData* AllocRenderData(int iRenderType);
	SColliderData* AllocColliderModel(int iColliderType);

	const int ChangeStringIDToNumID(const wstring wsID);
	const int ChangeRenderStringIDToNumID(const wstring wsID);

	const int ChangeColliderModelStringIDToNumID(const wstring wsID);
	const int ChangeColliderTypeStringIDToNumID(const wstring wsID);

	const int LoadGameObjData(const wstring wsFile);
	const int LoadRenderData(const wstring wsFile);

	SGameObjData* GetGameObjData(const wstring wsID);
	SRenderData* GetRenderData(const wstring wsID);

	const STileMapData* LoadTileMap(const wstring wsFile);
};

