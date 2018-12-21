#include "NetLogicHead.h"

CSessionObj::CSessionObj(DWORD dwSessionID, boost::asio::io_service& io_service, CGameServer* pServer)
	: m_Socket(io_service)
	, m_iSessionID(dwSessionID)
	, m_pServer(pServer)
	, m_bInit(false)
	, m_iKillCount(0)
{
}



CSessionObj::~CSessionObj()
{
	while (m_SendDataQueue.empty() == false)
	{
		delete[] m_SendDataQueue.front();
		m_SendDataQueue.pop_front();
	}
}

void CSessionObj::Init()
{
	m_iPacketBufferMark = 0;
	m_bInit = true;
}


void CSessionObj::PostReceive()
{
	m_Socket.async_read_some
	(
		boost::asio::buffer(m_ReceiveBuffer),
		boost::bind(&CSessionObj::Handle_Receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);

	/*
	async_read_some 비동기식으로 클라이언트가 보낸 데이터를 받습니다.

	async_read_some( 클라이언트가 보낸 데이터를 받을 버퍼, 요청한 작업(async_read)이 끝나면 호출할 함수)

	인자로 데이터를 담을 버퍼와 에러 코드를 받을 boost::asio::placeholders::error 를 사용하며
	데이터를 받으면 받은 크기를 반환합니다.

	*/
}


void CSessionObj::Handle_Receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
			std::cout << "클라이언트와 연결이 끊어졌습니다" << std::endl;
		else
			std::cout << "Error No : " << error.value() << "Error Message : " << error.message() << std::endl;

		m_pServer->CloseSession(m_iSessionID);

		/* 받은 데이터에 에러가 발생했다면 ( 접속이 끊긴 것도 포함 ) 에러를 출력하고 세션을 닫습니다. */
	}
	else
	{
		/*
		받은 데이터에 에러가 없다면 데이터를 처리합니다.

		데이터를 처리한 후 PostReceive()-[ async_read_some ] 함수를 호출해서
		클라이언트가 보내는 데이터를 다시 받습니다.
		*/


		memcpy(&m_cPacketBuffer[m_iPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);
		/*
		받아온 데이터를 패킷 배열에 저장합니다.
		*/


		DWORD dwPacketData = m_iPacketBufferMark + bytes_transferred; // 받은 데이터의 크기
		DWORD dwReadData = 0;

		while (dwPacketData > 0) // 받은 데이터를 모두 처리할 때까지 반복합니다.
		{
			if (dwPacketData < sizeof(SNET_PACKET_HEAD))
				break;

			// 남은 데이터가 패킷 헤더 보다 작으면 반복문을 중단합니다.

			SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)&m_cPacketBuffer[dwReadData];
			if (pHeader->m_iPacketSize <= dwPacketData)
				/*
				패킷 해더 크기 만큼 데이터가 있다면 페킷 해더가 존재하기 때문에 분석합니다.
				*/
			{
				m_pServer->ProcessPacket(m_iSessionID, &m_cPacketBuffer[dwReadData]);
				// 패킷을 분석합니다.

				dwPacketData -= pHeader->m_iPacketSize;
				dwReadData += pHeader->m_iPacketSize;
			}
			else
				break;	// 패킷 해더 만큼의 크기가 아니라면 패킷 해더가 없기 때문에 중단합니다.
		}

		if (dwPacketData > 0)
		{
			char TempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
			memcpy(&TempBuffer[0], &m_cPacketBuffer[dwReadData], dwPacketData);
			memcpy(&m_cPacketBuffer[0], &TempBuffer[0], dwPacketData);
		}

		// 남은 데이터 양을 저장하고 데이터 받기 요청
		m_iPacketBufferMark = dwPacketData;
		PostReceive();
	}
}

void CSessionObj::PostSend(const bool bImmediately, const int nSize, char* pData)
{
	char* pSendData = nullptr;

	if (pData == nullptr)
		return;

	if (bImmediately == false)
	{
		pSendData = new char[nSize];
		memcpy(pSendData, pData, nSize);

		m_SendDataQueue.push_back(pSendData);
		// 보낼 데이터를 큐에 저장
	}
	else
	{
		pSendData = pData;
	}

	if (bImmediately == false && m_SendDataQueue.size() > 1)
		return;

	// 앞서 보낸 데이터가 아직 다 보내지지 않았다면 다음에 보낸다. 

	boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize),
		boost::bind(&CSessionObj::Handle_Write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);

	/*
	async_write( 클라이언트 소켓, 보낼 데이터를 담은 버퍼, 전송이 완료 되면 호출할 함수 );

	*/
}


void CSessionObj::Handle_Write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (m_SendDataQueue.empty() == true)
		return;

	delete[] m_SendDataQueue.front(); //큐에 저장한 데이터 삭제
	m_SendDataQueue.pop_front();

	// 큐에 저장된 데이터가 있다면 데이터를 보낸다. 
	if (m_SendDataQueue.empty() == false)
	{
		char* pData = m_SendDataQueue.front();

		SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)pData;
		PostSend(true, pHeader->m_iPacketSize, pData);
	}
}

