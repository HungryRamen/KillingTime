#pragma once

class CGameServerMsg;
class CSessionObj;

class CGameServer
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	/*
	acceptor Ŭ������ Ŭ���̾�Ʈ�� ������ �޾Ƶ��̴� ������ �ϴµ�,
	io_service�� endpoint�� ���ڷ� ����ߴ�.

	m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER ) )

	endpoint�� ��Ʈ��ũ �ּҸ� �����Ѵ�.
	������ Ŭ���̾�Ʈ�� endpoint ���� ����� �ٸ��� ������ IP �ּ� �� ��Ʈ ��ȣ�� ����Ѵ�.
	*/

	int m_ISeqNumber;
	bool m_bIsAccepting;

	int m_iSpawnCount;
	int m_iSpawnPointMax;
	/*
	������ �������� Ȯ��
	*/

	std::vector< CSessionObj* > m_vecSessionList;		// Session ���
	std::deque< DWORD > m_SessionQueue;			// ����ִ� ( ���������� ���� ) Session ID ���
	int m_iNetWorkID;

	std::vector< CSessionObj* > m_vecConnectSession;
	CGameServerMsg* m_pGameServerMsg;

private:
	bool PostAccept();

	void Handle_Accept(CSessionObj* pSession, const boost::system::error_code& error);

public:
	CGameServer(boost::asio::io_service& io_service);
	virtual ~CGameServer();

	void Init(const int IMaxSessionCount);

	void Start();

	void CloseSession(const int nSessionID);

	void ProcessPacket(const int nSessionID, const char* pData);

	int GetSpawnCount() { return m_iSpawnCount++; }
	int GetSpawnPointMax() { return m_iSpawnPointMax; }

	D3DXVECTOR2 GetSpawnPos(int iPoint);
public:
	void PostSendSession(const int iSessionID, const bool bImmediately, const int iSize, char* pData);
	void PostSendAllSession(const bool bImmediately, const int iSize, char* pData, int iSendSeesion = -1);

	int GetConnectSessionCount() {
		return m_vecConnectSession.size();
	}

	void InsertConnectSession(const int iSessionID,wstring wsUserName)
	{
		if ((int)m_vecSessionList.size() <= iSessionID)
			return;

		m_vecConnectSession.push_back(m_vecSessionList[iSessionID]);
		m_vecConnectSession.back()->SetName(wsUserName);
	}

	wstring wsSerchSessionUserName(const int iSessionID)
	{
		if ((int)m_vecSessionList.size() <= iSessionID)
			return L"";

		for (int i = 0; i < (int)m_vecConnectSession.size(); ++i)
		{
			if(m_vecConnectSession[i]->GetSessionID() == iSessionID)
				return m_vecConnectSession[i]->GetName();
		}
		return L"";
	}

	int iSerchSessionUserKillCount(const int iSessionID)
	{
		if ((int)m_vecSessionList.size() <= iSessionID)
			return 0;

		for (int i = 0; i < (int)m_vecConnectSession.size(); ++i)
		{
			if (m_vecConnectSession[i]->GetSessionID() == iSessionID)
				return m_vecConnectSession[i]->GetKillCount();
		}
		return 0;
	}
	int iPlusSessionUserKillCount(const int iSessionID)
	{
		if ((int)m_vecSessionList.size() <= iSessionID)
			return 0;

		for (int i = 0; i < (int)m_vecConnectSession.size(); ++i)
		{
			if (m_vecConnectSession[i]->GetSessionID() == iSessionID)
			{
				return m_vecConnectSession[i]->PlusKillCount();
			}
		}
		return 0;
	}

public:
	int const CreateObjFunction(const wstring wsID, D3DXVECTOR2 vPos, float fRoataion = 0.0f, const int iMasterSession = -1);
};

