#pragma once

#include "../GlobalLogic/NetGlobalHead.h"

class CConnectMsg;

class CIOService
{
private:
	boost::asio::io_service& m_ioService;
	boost::asio::ip::tcp::socket m_Socket;

	std::array< char, MAX_RECEIVE_BUFFER_LEN > m_ReceiveBuffer;

	int m_iPacketBufferMark;
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 100];

	CRITICAL_SECTION m_lock;
	std::deque< char* > m_SendDataQueue;
	std::deque< SNET_PACKET_HEAD* > m_ReceiveDataQueue;

	bool m_bIsLogin;

	CConnectMsg* m_pConnectMsg;
private:
	void PostReceive();

	void Handle_Connect(const boost::system::error_code& error);
	void Handle_Write(const boost::system::error_code& error, size_t bytes_transferred);
	void Handle_Receive(const boost::system::error_code& error, size_t bytes_transferred);
	void ProcessPacket(SNET_PACKET_HEAD* pData);

public:
	CIOService(boost::asio::io_service& io_service);
	~CIOService();

	bool IsConnecting() { return m_Socket.is_open(); }

	void LoginOK() { m_bIsLogin = true; }
	bool IsLogin() { return m_bIsLogin; }

	void Connect(boost::asio::ip::tcp::endpoint endpoint);

	void Close();
	void PostSend(const bool bImmediately, const int nSize, char* pData);

	void ProcessReceivePacket();
};


class CConnectMgr
{
private:
	static CConnectMgr* m_pConnectMgr;
	vector<SNET_PACKET_HEAD*> m_vecReservePacket;

	CIOService* m_pIOService;
	boost::asio::io_service m_ioService;
	DWORD m_iSessionID;
	string m_sPcAdress;
private:
	void ProcessReceivePacket() { m_pIOService->ProcessReceivePacket(); };
	void ProceesingConnet(float fElapsedTime);

public:
	static CConnectMgr* GetConectMgr() { if (m_pConnectMgr == nullptr)  m_pConnectMgr = new CConnectMgr; return m_pConnectMgr; }

public:
	CConnectMgr();
	virtual ~CConnectMgr();

	int Init();
	void Handle_Connect();

	void LoginOk();
	void PostSend(const bool bImmediately, const int nSize, char* pData);

	void LoadIP(const wstring wsFile);

	void LoginServer(const wstring wsID);
	void PushReservePacket(SNET_PACKET_HEAD* pPacket);

	DWORD GetSessionID() {
		return m_iSessionID;
	}

	void SetSessionID(DWORD dwNetWorkID) {
		m_iSessionID = dwNetWorkID;
	}

	void OnFrameMove(float fElaspedTime);

};

