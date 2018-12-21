#pragma once

#include "../GlobalLogic/GlobalLogicHeader.h"

class CCollider;

class CActor
{
protected:
	int m_iGameObjType;
	int m_iTeam;

	D3DXVECTOR2 m_vPos;
	D3DXVECTOR2 m_vScl;

	float m_fRotation;
	int m_iRenderTurn;
	bool m_bCamera;

	bool m_bDestroy;

	std::wstring m_wsGameDataID;
	std::wstring m_wsRenderID;

	ESTATE m_eState;

protected:
	vector< CCollider* > m_vecCollider;

public:

	CActor();
	virtual ~CActor();

	const D3DXVECTOR2* GetPos() {
		return &m_vPos;
	}

	void SetPos(D3DXVECTOR2	vPos) {
		m_vPos = vPos;
	}

	const D3DXVECTOR2* GetScl() {
		return &m_vScl;
	}

	void SetScl(D3DXVECTOR2 vScl)
	{
		m_vScl = vScl;
	}

	const float* GetRotation() {
		return &m_fRotation;
	}

	void SetRotation(float fRotation) {
		m_fRotation = fRotation;
	}
	const bool IsCamera() {
		return m_bCamera;
	}

	void SetRenderTurn(int const iRenderTurn)
	{
		m_iRenderTurn = iRenderTurn;
	}

	void SetCamera(bool bCamera) {
		m_bCamera = bCamera;
	}

	int const GetTeam()
	{
		return m_iTeam;
	}

	void SetTeam(int const iTeam)
	{
		m_iTeam = iTeam;
	}

	void SetGameDataID(const wstring wsID)
	{
		m_wsGameDataID = wsID;
	}

	const wstring GetGameDataID() { return m_wsGameDataID; }

	void SetRenderID(const wstring wsRenderID)
	{
		m_wsRenderID = wsRenderID;
	}

	const wstring GetRenderID() {
		return m_wsRenderID;
	}

	void SetGameObjType(const int iGameObjType)
	{
		m_iGameObjType = iGameObjType;
	}

	int const GetGameObjType() {
		return m_iGameObjType;
	}

	virtual void ChangeState(const ESTATE eState, bool bNet = false) {
		m_eState = eState;
	}

	virtual ESTATE const GetState() {
		return m_eState;
	}

	const bool* IsDestroy() {
		return &m_bDestroy;
	}

	void Destroy() {
		m_bDestroy = true;
	}

public:
	virtual void OnAtkCollider(CActor* pDefTarget) {}
	virtual void OnDefCollider(CActor* pAtkTarget) {}

	void PushCollider(CCollider* pCollider) {
		m_vecCollider.push_back(pCollider);
	}

};