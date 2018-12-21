#include "DXUT.h"

CPlayerController::CPlayerController(CGameObj* pObj) :
	CGameObjController(pObj),
	m_fAttackTime(0.0f),
	m_fAttackMaxTime(0.24f),
	m_fRespawnTime(0.0f),
	m_bDie(false)
{
	D_CAMERAMGR->SetTargetObj(pObj);
	AddState(AT_NONE, SFSMState((DWORD)this, nullptr, nullptr, nullptr));
	AddState(AT_NORMAL, SFSMState((DWORD)this, nullptr, OnUpdateNormal, nullptr));
	AddState(AT_MOVE, SFSMState((DWORD)this, nullptr, OnUpdateMove, nullptr));
	AddState(AT_ATK, SFSMState((DWORD)this, nullptr, OnUpdateAtk, nullptr));
	AddState(AT_DIE, SFSMState((DWORD)this, nullptr, OnUpdateDie, nullptr));
}

CPlayerController::~CPlayerController()
{
}

void CPlayerController::OnFrameMove(float fElapsedTime)
{
	OnUpdateState(fElapsedTime);
	OnChangeReserveState();
}

void CPlayerController::OnUpdateNormal(DWORD dwObj, float fElapsedTime)
{
	CPlayerController* pController = (CPlayerController*)dwObj;
	CGameObj* pObj = pController->GetGameObj();
	if (((CUnitObj*)pObj)->GetHP() <= 0)
	{
		pObj->ChangeState(AT_DIE);
		pController->SetDie(true);
		return;
	}
	float fRotation = *pObj->GetRotation();
	D3DXVECTOR2 vPos = *pObj->GetPos();
	pController->SetAttackTime(pController->GetAttackTime() - fElapsedTime);

	fRotation = D3DXToDegree(atan2(vPos.x - D_MOUSEMGR->GetCameraMousePos()->x, D_MOUSEMGR->GetCameraMousePos()->y - vPos.y));
	if (fRotation != *pObj->GetRotation())
	{
		SNET_REQ_CHANGE_ROTATION* pPacket = new SNET_REQ_CHANGE_ROTATION;
		pPacket->Init();
		pPacket->m_fRotation = (float)(int)fRotation;
		pPacket->m_iNetObjID = pObj->GetNetObjID();
		D_CONNECTMGR->PushReservePacket(pPacket);
		pObj->SetRotation(fRotation);
	}
	if (DXUTIsKeyDown('W') || DXUTIsKeyDown('S') || DXUTIsKeyDown('A')
		|| DXUTIsKeyDown('D'))
		pObj->ChangeState(AT_MOVE);
	else if (D_MOUSEMGR->IsClick())
		pObj->ChangeState(AT_ATK);


}

void CPlayerController::OnUpdateMove(DWORD dwObj, float fElapsedTime)
{
	CPlayerController* pController = (CPlayerController*)dwObj;
	CUnitObj* pObj = (CUnitObj*)pController->GetGameObj();

	if (((CUnitObj*)pObj)->GetHP() <= 0)
	{
		pObj->ChangeState(AT_DIE);
		pController->SetDie(true);
		return;
	}

	if (pObj->GetNetObjID() == -1)
		return;

	float fRotation = *pObj->GetRotation();
	D3DXVECTOR2 vPos = *pObj->GetPos();
	float fSpeed = pObj->GetSpeed();
	float fMoveRotation = 0.0f;
	bool bChangeState = true;
	pController->SetAttackTime(pController->GetAttackTime() - fElapsedTime);

	fRotation = D3DXToDegree(atan2(vPos.x - D_MOUSEMGR->GetCameraMousePos()->x, D_MOUSEMGR->GetCameraMousePos()->y - vPos.y));
	if (DXUTIsKeyDown('W') == true)
	{
		bChangeState = false;
		if (DXUTIsKeyDown('A') == true)
		{
			fMoveRotation = 315.0f;
		}
		else if (DXUTIsKeyDown('D') == true)
		{
			fMoveRotation = 45.0f;
		}
	}
	else if (DXUTIsKeyDown('S') == true)
	{
		fMoveRotation = 180.0f;
		bChangeState = false;
		if (DXUTIsKeyDown('A') == true)
		{
			fMoveRotation = 225.0f;
		}
		else if (DXUTIsKeyDown('D') == true)
		{
			fMoveRotation = 135.0f;
		}
	}
	else if (DXUTIsKeyDown('A') == true)
	{
		bChangeState = false;
		fMoveRotation = 270.0f;
	}
	else if (DXUTIsKeyDown('D') == true)
	{
		bChangeState = false;
		fMoveRotation = 90.0f;
	}
	
	if (bChangeState == false)
	{
		D3DXVECTOR2 vRotationPos(sin(D3DXToRadian(fMoveRotation)), -cos(D3DXToRadian(fMoveRotation)));
		vPos += vRotationPos * (fSpeed * fElapsedTime);
		vPos = D3DXVECTOR2((int)vPos.x, (int)vPos.y);
		if (vPos.x <= 0.0f)
			vPos.x = 0.0f;
		else if (vPos.x >= D_GAMETILEMGR->GetTileWidth() * D_GAMETILEMGR->GetOffSetX())
			vPos.x = D_GAMETILEMGR->GetTileWidth() * D_GAMETILEMGR->GetOffSetX();
		if (vPos.y <= 0.0f)
			vPos.y = 0.0f;
		else if (vPos.y >= D_GAMETILEMGR->GetTileHeight() * D_GAMETILEMGR->GetOffSetY())
			vPos.y = D_GAMETILEMGR->GetTileHeight() * D_GAMETILEMGR->GetOffSetY();
	}
	if (vPos != *pObj->GetPos())
	{
		SNET_REQ_CHANGE_POS* pPacket = new SNET_REQ_CHANGE_POS;
		pPacket->Init();
		pObj->SetPos(vPos);
		D_GAMETILEMGR->TilePlayerCollision(pObj);
		pPacket->m_vPos = D3DXVECTOR2((int)pObj->GetPos()->x, (int)pObj->GetPos()->y);
		pPacket->m_iNetObjID = pObj->GetNetObjID();
		D_CONNECTMGR->PushReservePacket(pPacket);
		bChangeState = false;
	}
	
	if (fRotation != *pObj->GetRotation())
	{
		SNET_REQ_CHANGE_ROTATION* pPacket = new SNET_REQ_CHANGE_ROTATION;
		pPacket->Init();
		pPacket->m_fRotation = (float)(int)fRotation;
		pPacket->m_iNetObjID = pObj->GetNetObjID();
		D_CONNECTMGR->PushReservePacket(pPacket);
		pObj->SetRotation(fRotation);
	}

	if (bChangeState)
		pObj->ChangeState(AT_NORMAL);
}

void CPlayerController::OnUpdateAtk(DWORD dwObj, float fElapsedTime)
{
	CPlayerController* pController = (CPlayerController*)dwObj;
	CGameObj* pObj = pController->GetGameObj();
	if (((CUnitObj*)pObj)->GetHP() <= 0)
	{
		pObj->ChangeState(AT_DIE);
		pController->SetDie(true);
		return;
	}
	if (DXUTIsKeyDown('W') || DXUTIsKeyDown('S') || DXUTIsKeyDown('A')
		|| DXUTIsKeyDown('D'))
	{
		pObj->ChangeState(AT_MOVE);
		return;
	}
	bool bChangeState = true;
	pController->SetAttackTime(pController->GetAttackTime() - fElapsedTime);
	float fRotation = *pObj->GetRotation();
	D3DXVECTOR2 vPos = *pObj->GetPos();
	fRotation = D3DXToDegree(atan2(vPos.x - D_MOUSEMGR->GetCameraMousePos()->x, D_MOUSEMGR->GetCameraMousePos()->y - vPos.y));
	if (D_MOUSEMGR->IsClick())
	{
		if (pController->GetAttackTime() <= 0.0f)
		{
			SNET_REQ_CREATE_BULLET* pPacket = new SNET_REQ_CREATE_BULLET;
			pPacket->Init();
			pPacket->m_iNetObjID = pObj->GetNetObjID();
			pController->SetAttackTime(pController->GetAttackMaxTime());
			D_CONNECTMGR->PushReservePacket(pPacket);
		}
		bChangeState = false;
	}
	if (fRotation != *pObj->GetRotation())
	{
		SNET_REQ_CHANGE_ROTATION* pPacket = new SNET_REQ_CHANGE_ROTATION;
		pPacket->Init();
		pPacket->m_fRotation = (float)(int)fRotation;
		pPacket->m_iNetObjID = pObj->GetNetObjID();
		D_CONNECTMGR->PushReservePacket(pPacket);
		pObj->SetRotation(fRotation);
	}
	if (bChangeState)
		pObj->ChangeState(AT_NORMAL);
}

void CPlayerController::OnUpdateDie(DWORD dwObj, float fElapsedTime)
{
	CPlayerController* pController = (CPlayerController*)dwObj;
	CGameObj* pObj = pController->GetGameObj();
	pController->SetRespawnTime(pController->GetRespawnTime() + fElapsedTime);
	if (pController->GetRespawnTime() >= 5.0f && pController->IsDie())
	{
		pController->SetRespawnTime(0.0f);
		((CAniRender*)pObj->GetRender())->SetCurIndex(0);
		SNET_REQ_RESPAWN_PLAYER* pPacket = new SNET_REQ_RESPAWN_PLAYER;
		pPacket->Init();
		pPacket->m_iNetObjID = pObj->GetNetObjID();
		D_CONNECTMGR->PushReservePacket(pPacket);
		pController->SetDie(false);
	}
}

void CPlayerController::OnChangeState(ESTATE const eState, bool bNet)
{
	__super::OnChangeState(eState, bNet);

	if (bNet == false)
	{
		SNET_REQ_CHANGE_STATE* pPacket = new SNET_REQ_CHANGE_STATE;
		pPacket->Init();
		pPacket->m_eState = eState;
		pPacket->m_iNetObjID = GetGameObj()->GetNetObjID();
		D_CONNECTMGR->PushReservePacket(pPacket);
	}
}