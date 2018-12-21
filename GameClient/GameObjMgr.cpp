#include "DXUT.h"

CGameObjMgr* CGameObjMgr::m_pGameObjMgr = nullptr;

CGameObjMgr::CGameObjMgr()
{
}


CGameObjMgr::~CGameObjMgr()
{
}

CGameObj* CGameObjMgr::Alloc(int iType)
{
	switch (iType)
	{
	case EGAMEOBJ_NORMAL: return new CGameObj;
	case EGAMEOBJ_UNIT: return new CUnitObj;
	case EGAMEOBJ_BULLET: return new CBulletObj;
	case EGAMEOBJ_HPBAR: return new CHPBar;
	}

	return nullptr;
}

void CGameObjMgr::OnFrameMove(float fElapsedtime)
{
	PostProcess();

	for each(map< int, vector< CGameObj* >>::const_reference mr in m_mapObj)
		OnFrameMove(mr.second, fElapsedtime);
}

void CGameObjMgr::OnFrameMove(const vector< CGameObj* >&vecObj, float fElapsedTime)
{
	for each(vector< CGameObj* >::const_reference vr in vecObj)
		vr->OnFrameMove(fElapsedTime);
}

void CGameObjMgr::OnFrameRender()
{
	for each(map< int, vector< CGameObj* >>::const_reference mr in m_mapObj)
	{
		OnFrameRender(mr.second);
	}
}

void CGameObjMgr::OnFrameRender(const vector< CGameObj* >&vecObj)
{
	for each(vector< CGameObj*>::const_reference vr in vecObj)
		vr->OnFrameRender();
}