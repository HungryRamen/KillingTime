#include "DXUT.h"
#include "../GlobalLogic/NetGlobalHead.h"

const char* pcAdress = "10.72.147.142";

CIOService::CIOService(boost::asio::io_service& io_service)
	: m_ioService(io_service),
	m_Socket(io_service)
{
	m_bIsLogin = false;
	m_pConnectMsg = new CConnectMsg;
	InitializeCriticalSectionAndSpinCount(&m_lock, 1);
}


CIOService::~CIOService()
{
	m_ioService.stop();
	Close();

	EnterCriticalSection(&m_lock);

	while (m_SendDataQueue.empty() == false)
	{
		delete[] m_SendDataQueue.front();
		m_SendDataQueue.pop_front();
	}

	LeaveCriticalSection(&m_lock);
	DeleteCriticalSection(&m_lock);
}

void CIOService::Connect(boost::asio::ip::tcp::endpoint endpoint)
{
	m_iPacketBufferMark = 0;

	cout << "Connect function " << endl;

	m_Socket.async_connect(endpoint,
		boost::bind(&CIOService::Handle_Connect, this,
			boost::asio::placeholders::error));
}

void CIOService::Close()
{
	if (m_Socket.is_open())
		m_Socket.close();
}

void CIOService::PostSend(const bool bImmediately, const int nSize, char* pData)
{
	char* pSendData = nullptr;

	EnterCriticalSection(&m_lock);

	/*
	공유 리소스 m_SendDataQueue의
	충돌을 보호하기 위해 크리티컬렉션으로 동기화
	*/

	if (bImmediately == false)
	{
		pSendData = new char[nSize];
		memcpy(pSendData, pData, nSize);

		m_SendDataQueue.push_back(pSendData);
	}
	else
		pSendData = pData;

	if (bImmediately || m_SendDataQueue.size() < 2)
	{
		boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize),
			boost::bind(&CIOService::Handle_Write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	LeaveCriticalSection(&m_lock);
}

void CIOService::PostReceive()
{
	memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

	m_Socket.async_read_some
	(
		boost::asio::buffer(m_ReceiveBuffer),
		boost::bind(&CIOService::Handle_Receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

void CIOService::Handle_Connect(const boost::system::error_code& error)
{
	if (error == false)
	{
		PostReceive();
	}
	else
		std::cout << "서버 접속 실패. Error No : " << error.value() << error.message() << std::endl;
}

void CIOService::Handle_Write(const boost::system::error_code& error, size_t bytes_transferred)
{
	EnterCriticalSection(&m_lock);

	if (m_SendDataQueue.empty() == true)
	{
		LeaveCriticalSection(&m_lock);
		return;
	}

	delete[] m_SendDataQueue.front();
	m_SendDataQueue.pop_front();

	char* pData = nullptr;

	if (m_SendDataQueue.empty() == false)
		pData = m_SendDataQueue.front();

	LeaveCriticalSection(&m_lock);

	if (pData != nullptr)
	{
		SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)pData;
		PostSend(true, pHeader->m_iPacketSize, pData);
	}
}

void CIOService::Handle_Receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	EnterCriticalSection(&m_lock);

	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << " 클라이언와 연결이 끊어졌습니다. " << std::endl;
		}
		else
		{
			std::cout << "Error No : " << error.value() << " Error Message : " << error.message() << std::endl;
		}

		LeaveCriticalSection(&m_lock);
		Close();
	}
	else
	{
		memcpy(&m_PacketBuffer[m_iPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);

		int iPacketData = m_iPacketBufferMark + bytes_transferred;
		int iReadData = 0;

		while (iPacketData > 0)
		{
			if (iPacketData < sizeof(SNET_PACKET_HEAD))
				break;

			SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)&m_PacketBuffer[iReadData];

			if (pHeader->m_iPacketSize <= iPacketData)
			{

				char* pCopyData = new char[MAX_RECEIVE_BUFFER_LEN];
				memcpy(pCopyData, pHeader, pHeader->m_iPacketSize);

				m_ReceiveDataQueue.push_back((SNET_PACKET_HEAD*)pCopyData);

				iPacketData -= pHeader->m_iPacketSize;
				iReadData += pHeader->m_iPacketSize;
			}
			else
			{
				break;
			}
		}

		if (iPacketData > 0)
		{
			char TempBuffer[MAX_RECEIVE_BUFFER_LEN * 100] = { 0, };
			memcpy(&TempBuffer[0], &m_PacketBuffer[iReadData], iPacketData);
			memcpy(&m_PacketBuffer[0], &TempBuffer[0], iPacketData);
		}

		m_iPacketBufferMark = iPacketData;
		PostReceive();
	}

	LeaveCriticalSection(&m_lock);
}

void CIOService::ProcessReceivePacket()
{
	EnterCriticalSection(&m_lock);

	if (m_ReceiveDataQueue.empty() == true)
	{
		LeaveCriticalSection(&m_lock);
		return;
	}

	while (m_ReceiveDataQueue.empty() == false)
	{
		SNET_PACKET_HEAD* pPacket = m_ReceiveDataQueue.front();
		if (pPacket == nullptr)
		{
			m_ReceiveDataQueue.pop_front();
			continue;
		}

		ProcessPacket(pPacket);
		m_ReceiveDataQueue.pop_front();
	}
	LeaveCriticalSection(&m_lock);
}

void CIOService::ProcessPacket(SNET_PACKET_HEAD* pHeader)
{
	m_pConnectMsg->ProcessPakcet(pHeader->m_iSendID, pHeader);

}

CConnectMgr* CConnectMgr::m_pConnectMgr = nullptr;

CConnectMgr::CConnectMgr()
{
}


CConnectMgr::~CConnectMgr()
{
	SAFE_DELETE(m_pIOService);
}

int CConnectMgr::Init()
{
	LoadIP(L"../Data/ConnectIp.txt");
	auto endpoint = boost::asio::ip::tcp::endpoint(
		boost::asio::ip::address::from_string(m_sPcAdress.c_str()),
		PORT_NUMBER);

	m_pIOService = new CIOService(m_ioService);
	m_pIOService->Connect(endpoint);

	boost::thread thread(boost::bind(&boost::asio::io_service::run, &m_ioService));

	return -1;
}

void CConnectMgr::OnFrameMove(float fElaspedTime)
{
	ProceesingConnet(fElaspedTime);
	ProcessReceivePacket();
}

void CConnectMgr::ProceesingConnet(float fElaspedTime)
{
	if (m_pIOService->IsLogin() == true)
	{
		if (m_vecReservePacket.empty() == true)
			return;

		vector<SNET_PACKET_HEAD*>::iterator vit = m_vecReservePacket.begin();
		for (vit; vit != m_vecReservePacket.end(); ++vit)
			m_pIOService->PostSend(true, (*vit)->m_iPacketSize, (char*)(*vit));

		m_vecReservePacket.clear();
	}
}

void CConnectMgr::Handle_Connect()
{
}

void CConnectMgr::LoginOk()
{
	if (m_pIOService == nullptr)
		return;

	m_pIOService->LoginOK();
}

void CConnectMgr::PostSend(const bool bImmediately, const int nSize, char* pData)
{
	if (m_pIOService == nullptr)
		return;

	m_pIOService->PostSend(bImmediately, nSize, pData);
}

void CConnectMgr::LoadIP(const wstring wsFile)
{
	ifstream ifRead(wsFile.c_str());

	if (ifRead.fail())
		return;

	ifRead >> m_sPcAdress;

	ifRead.close();
}

void CConnectMgr::PushReservePacket(SNET_PACKET_HEAD* pPacket)
{
	m_vecReservePacket.push_back(pPacket);
}

void CConnectMgr::LoginServer(const wstring wsID)
{
	if (m_pIOService->IsLogin() == false)
	{
		SNET_REQ_SERVER_JOIN SendPkt;
		SendPkt.Init();
		SendPkt.m_wsUserName = D_FONT->GetMyName();
		MessageBox(DXUTGetHWND(), L"Login", L"", 0);
		m_pIOService->PostSend(false, SendPkt.m_iPacketSize, (char*)&SendPkt);
	}
}