#pragma once

class CGameServer;

class CSessionObj
{
private:
	DWORD m_iSessionID;
	wstring m_wsUserName;
	boost::asio::ip::tcp::socket m_Socket;

	std::array< char, MAX_RECEIVE_BUFFER_LEN > m_ReceiveBuffer;

	DWORD m_iPacketBufferMark;
	char m_cPacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];

	std::deque<char*> m_SendDataQueue;

	CGameServer* m_pServer;
	bool m_bInit;

	int m_iKillCount;
private:
	void Handle_Write(const boost::system::error_code& error, size_t bytes_transferred);
	void Handle_Receive(const boost::system::error_code& error, size_t bytes_transferred);

public:
	CSessionObj(DWORD dwSessionID, boost::asio::io_service& io_service, CGameServer* pServer);
	virtual ~CSessionObj();

	void Init();

	void PostReceive();
	void PostSend(const bool bImmediately, const int nSize, char* pData);

	void SetName(wstring wsName) {
		m_wsUserName = wsName;
	}

	wstring GetName() { 
		return m_wsUserName; 
	}

	int GetKillCount() {
		return m_iKillCount;
	}

	int PlusKillCount() { 
		return m_iKillCount++;
	}
	bool IsInit() {
		return m_bInit;
	}

	/* Immediately : ¡Ô∞¢ */

public:
	DWORD GetSessionID() { return m_iSessionID; }

	boost::asio::ip::tcp::socket& GetSocket() {
		return m_Socket;
	}

};