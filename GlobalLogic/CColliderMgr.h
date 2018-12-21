#pragma once

#include "../GlobalLogic/GlobalLogicHeader.h"

class CActor;

class CCollider
{
protected:
	int m_iColliderType;
	int m_iColliderModel;

	D3DXVECTOR2 m_vPos;
	D3DXVECTOR2 m_vCollisionPos; // 충돌위치랑 오브젝트 위치값 합친거
	CActor* m_pParent;
	bool m_bDestroy;

public:
	CCollider() :
		m_pParent(nullptr),
		m_iColliderType(-1),
		m_iColliderModel(-1),
		m_vPos(0.0f, 0.0f)
	{
	}

	//CCollider(CActor* pParent, int iColliderType, int iColliderModel, D3DXVECTOR2 vPos) :
	//	m_pParent(pParent),
	//	m_iColliderModel(iColliderModel),
	//	m_iColliderType(iColliderType),
	//	m_vPos(vPos)
	//{
	//}

public:
	CActor * GetParent() {
		return m_pParent;
	}

	void SetParent(CActor* pParent)
	{
		m_pParent = pParent;
	}

	int const GetColliderType() {
		return m_iColliderType;
	}

	void SetCollierType(int iType) {
		m_iColliderType = iType;
	}

	int const GetGolliderModel() {
		return m_iColliderModel;
	}

	void SetColliderModel(int iModel) {
		m_iColliderModel = iModel;
	}

	const D3DXVECTOR2* GetPos() {
		return &m_vPos;
	}

	void SetPos(D3DXVECTOR2 vPos) {
		m_vPos = vPos;
	}

	D3DXVECTOR2 GetCollisionPos() {
		return (m_vPos + *m_pParent->GetPos());
	}

	void Destroy() {
		m_bDestroy = true;
	}

	const bool* IsDestroy() {
		return &m_bDestroy;
	}

public:
	void OnAtkCollider(CActor* pTarget) {
		m_pParent->OnAtkCollider(pTarget);
	}

	void OnDefCollider(CActor* pTarget) {
		m_pParent->OnDefCollider(pTarget);
	}

};

class CCircleCollider : public CCollider
{
protected:
	int m_iRadius;

public:
	CCircleCollider() :
		m_iRadius(-1)
	{
	}

public:
	int const GetRadius() {
		return m_iRadius;
	}

	void SetRaidus(int iRadius) {
		m_iRadius = iRadius;
	}

};

class CSquareCollider : public CCollider
{
protected:
	int m_iWidth;
	int m_iHeight;

public:
	CSquareCollider() :
		m_iWidth(-1),
		m_iHeight(-1)
	{
	}

public:
	int const GetWidth() {
		return m_iWidth;
	}

	int const GetHeight() {
		return m_iHeight;
	}

	void SetWidth(int iWIdth) {
		m_iWidth = iWIdth;
	}

	void SetHeight(int iHeight) {
		m_iHeight = iHeight;
	}
};

class CColliderMgr
{
private:
	static CColliderMgr* m_pColliderMgr;

private:
	vector< CCollider* > m_vecAtkCollider;
	vector< CCollider* > m_vecDefCollider;

	CCollider* AllocCollider(int iColliderModel);

public:
	static CColliderMgr* GetColliderMgr() {
		if (m_pColliderMgr == nullptr) m_pColliderMgr = new CColliderMgr;
		return m_pColliderMgr;
	}

public:
	CColliderMgr();
	virtual ~CColliderMgr();

	const CCollider* CreateCollider(CActor* pParent, SColliderData* pColliderData);
	void OnFrameMove(float fElapsedTime);
	void ProcessCollider();

public:
	bool IsCircleCollisionCheck(CCollider* pAtk, CCollider* pDef);
	bool IsCircleSqureCollisionCheck(CCollider* pAtk, CCollider* pDef);
};

