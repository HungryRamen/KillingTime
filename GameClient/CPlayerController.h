#pragma once

class CPlayerController : public CGameObjController
{
private:
	// 추후에 데이터로 빼야함.
	float m_fAttackTime;
	float m_fAttackMaxTime;
	float m_fRespawnTime;
	bool m_bDie;
private:
	static void OnUpdateNormal(DWORD dwObj, float fElaspedTime);
	static void OnUpdateMove(DWORD dwObj, float fElapsedTime);
	static void OnUpdateAtk(DWORD dwObj, float fElapsedTime);
	static void OnUpdateDie(DWORD dwObj, float fElapsedTime);

public:
	CPlayerController(CGameObj* pObj);
	virtual ~CPlayerController();

	float GetAttackTime() { return m_fAttackTime; }
	void SetAttackTime(float fAttackTime) {
		m_fAttackTime = fAttackTime;
	}

	float GetRespawnTime() { return m_fRespawnTime; }
	void SetRespawnTime(float fRespawnTime) {
		m_fRespawnTime = fRespawnTime;
	}

	bool IsDie() { return m_bDie; }
	void SetDie(bool bUse) {
		m_bDie = bUse;
	}

	float GetAttackMaxTime() { return m_fAttackMaxTime; };

	virtual void OnFrameMove(float fElapsedTime);
	virtual void OnChangeState(ESTATE const eState, bool bNet = false);
};

