#pragma once

class CGameServerMsg;
class CSessionObj;

class CGameServer
{
private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	/*
	acceptor 클래스는 클라이언트의 접속을 받아들이는 역할을 하는데,
	io_service와 endpoint를 인자로 사용했다.

	m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER ) )

	endpoint는 네트워크 주소를 설정한다.
	서버와 클라이언트는 endpoint 설정 방식이 다른데 서버는 IP 주소 와 포트 번호를 사용한다.
	*/

	int m_ISeqNumber;
	bool m_bIsAccepting;

	int m_iSpawnCount;
	int m_iSpawnPointMax;
	/*
	접속이 가능한지 확인
	*/

	std::vector< CSessionObj* > m_vecSessionList;		// Session 목록
	std::deque< DWORD > m_SessionQueue;			// 비어있는 ( 접속중이지 않은 ) Session ID 목록
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

