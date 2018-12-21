#pragma once
class CNetGameActorContainer
{
private:
	int m_iNetObjID;

protected:
	std::map< int, CNetGameActor* > m_mapNetGameObjData;

public:
	CNetGameActorContainer();
	virtual ~CNetGameActorContainer();

	CNetGameActor* GetNetGameObjData(int iNetObjID)
	{
		std::map< int, CNetGameActor* >::iterator mit = m_mapNetGameObjData.find(iNetObjID);
		if (mit == m_mapNetGameObjData.end())
			return nullptr;

		return mit->second;
	}

	virtual CNetGameActor* AllocNetGameActor(const int iGameObjType) { return nullptr; }

	CNetGameActor* const CreateNetGameObjdata(SGameObjData* pData, D3DXVECTOR2 vPos, D3DXVECTOR2 vScl, float fRotation, int iRenderTurn, bool bCamera, int iMasterSessionID = -1, bool bStartSync = false);

	int SendAllNetGameObjDataToSession(int iSessionID);
	const int RemoveNetGameObjData(int iNetObjID);
	const int DisconnectSession(const int iSessionID);
	const int DestroyNetGameObjData(int iNetObjID);

	int BulletUserSerch(int iTeam);

	const int SyncNetObjContainer(const int iSessionID);

};

