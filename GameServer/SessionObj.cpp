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
	async_read_some �񵿱������ Ŭ���̾�Ʈ�� ���� �����͸� �޽��ϴ�.

	async_read_some( Ŭ���̾�Ʈ�� ���� �����͸� ���� ����, ��û�� �۾�(async_read)�� ������ ȣ���� �Լ�)

	���ڷ� �����͸� ���� ���ۿ� ���� �ڵ带 ���� boost::asio::placeholders::error �� ����ϸ�
	�����͸� ������ ���� ũ�⸦ ��ȯ�մϴ�.

	*/
}


void CSessionObj::Handle_Receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
			std::cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�" << std::endl;
		else
			std::cout << "Error No : " << error.value() << "Error Message : " << error.message() << std::endl;

		m_pServer->CloseSession(m_iSessionID);

		/* ���� �����Ϳ� ������ �߻��ߴٸ� ( ������ ���� �͵� ���� ) ������ ����ϰ� ������ �ݽ��ϴ�. */
	}
	else
	{
		/*
		���� �����Ϳ� ������ ���ٸ� �����͸� ó���մϴ�.

		�����͸� ó���� �� PostReceive()-[ async_read_some ] �Լ��� ȣ���ؼ�
		Ŭ���̾�Ʈ�� ������ �����͸� �ٽ� �޽��ϴ�.
		*/


		memcpy(&m_cPacketBuffer[m_iPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);
		/*
		�޾ƿ� �����͸� ��Ŷ �迭�� �����մϴ�.
		*/


		DWORD dwPacketData = m_iPacketBufferMark + bytes_transferred; // ���� �������� ũ��
		DWORD dwReadData = 0;

		while (dwPacketData > 0) // ���� �����͸� ��� ó���� ������ �ݺ��մϴ�.
		{
			if (dwPacketData < sizeof(SNET_PACKET_HEAD))
				break;

			// ���� �����Ͱ� ��Ŷ ��� ���� ������ �ݺ����� �ߴ��մϴ�.

			SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)&m_cPacketBuffer[dwReadData];
			if (pHeader->m_iPacketSize <= dwPacketData)
				/*
				��Ŷ �ش� ũ�� ��ŭ �����Ͱ� �ִٸ� ��Ŷ �ش��� �����ϱ� ������ �м��մϴ�.
				*/
			{
				m_pServer->ProcessPacket(m_iSessionID, &m_cPacketBuffer[dwReadData]);
				// ��Ŷ�� �м��մϴ�.

				dwPacketData -= pHeader->m_iPacketSize;
				dwReadData += pHeader->m_iPacketSize;
			}
			else
				break;	// ��Ŷ �ش� ��ŭ�� ũ�Ⱑ �ƴ϶�� ��Ŷ �ش��� ���� ������ �ߴ��մϴ�.
		}

		if (dwPacketData > 0)
		{
			char TempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
			memcpy(&TempBuffer[0], &m_cPacketBuffer[dwReadData], dwPacketData);
			memcpy(&m_cPacketBuffer[0], &TempBuffer[0], dwPacketData);
		}

		// ���� ������ ���� �����ϰ� ������ �ޱ� ��û
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
		// ���� �����͸� ť�� ����
	}
	else
	{
		pSendData = pData;
	}

	if (bImmediately == false && m_SendDataQueue.size() > 1)
		return;

	// �ռ� ���� �����Ͱ� ���� �� �������� �ʾҴٸ� ������ ������. 

	boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize),
		boost::bind(&CSessionObj::Handle_Write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);

	/*
	async_write( Ŭ���̾�Ʈ ����, ���� �����͸� ���� ����, ������ �Ϸ� �Ǹ� ȣ���� �Լ� );

	*/
}


void CSessionObj::Handle_Write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (m_SendDataQueue.empty() == true)
		return;

	delete[] m_SendDataQueue.front(); //ť�� ������ ������ ����
	m_SendDataQueue.pop_front();

	// ť�� ����� �����Ͱ� �ִٸ� �����͸� ������. 
	if (m_SendDataQueue.empty() == false)
	{
		char* pData = m_SendDataQueue.front();

		SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)pData;
		PostSend(true, pHeader->m_iPacketSize, pData);
	}
}

