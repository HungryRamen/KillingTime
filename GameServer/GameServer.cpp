#include "NetLogicHead.h"



CGameServer::CGameServer(boost::asio::io_service& io_service) :
	m_acceptor(
		io_service,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4()
			, PORT_NUMBER))
	, m_iNetWorkID(-1),
	m_iSpawnCount(0),
	m_iSpawnPointMax(4)
{
	m_pGameServerMsg = new CGameServerMsg;
}


CGameServer::~CGameServer()
{
	for (size_t i = 0; i < m_vecSessionList.size(); ++i)
	{
		if (m_vecSessionList[i]->GetSocket().is_open())
			m_vecSessionList[i]->GetSocket().close();

		delete m_vecSessionList[i];
	}
}

void CGameServer::Init(const int nMaxSessionCount)
{
	for (int i = 0; i < nMaxSessionCount; ++i)
	{
		CSessionObj* pSession = new CSessionObj(i, m_acceptor.get_io_service(), this);
		/*
		Session( ���� ID, io_service, ChattingServer );
		*/


		m_vecSessionList.push_back(pSession);
		m_SessionQueue.push_back(i);

		/*
		Session Ŭ���� ( ������ ������ Ŭ���̾�Ʈ ) �� �ִ� ���� ����ŭ �̸� �Ҵ��ؼ�
		m_SessionList �� ��� �����ϴ�.

		���ο� ������ ���� ������ ������� �ʴ� Session�� �Ҵ��ϱ� ���� ���� ������� �ʴ�
		Session�� �ε��� ��ȣ�� m_SessionQueue�� ������ ���´�
		*/
	}
}

void CGameServer::Start()
{
	std::cout << " ���� ���� .... " << std::endl;

	PostAccept();
}

void CGameServer::CloseSession(const int nSessionID)
{
	std::cout << "Ŭ���̾�Ʈ ���� ����, ���� ID : " << nSessionID << std::endl;

	for (int i = m_vecConnectSession.size() - 1; i >= 0; --i)
	{
		if (m_vecConnectSession[i] == m_vecSessionList[nSessionID])
		{
			m_vecConnectSession.erase(m_vecConnectSession.begin() + i);
			break;
		}
	}

	D_GAMESERVERMGR->GetNetGameActorMgr()->DisconnectSession(nSessionID);
	m_vecSessionList[nSessionID]->GetSocket().close();
	m_SessionQueue.push_back(nSessionID);

	/*
	Session ( ������ ������ Ŭ���̾�Ʈ ) �� ������ �����Ͽ��ٸ�
	Socekt�� �ݰ� SessionQueue �� ���� ID�� �־� ���ο� ������ ������ �ְ��մϴ�.
	*/

	if (m_bIsAccepting == false)
		PostAccept();

	/*
	�����ϰ� �ִ� Ŭ���̾�Ʈ�� ���� ���̻� ������ ���� �� ���� ���¿��ٸ�
	PostAccept() �Լ��� ȣ���Ͽ� ������ ������ ���¶�� �˷��ְ�
	�� Session ������ ���� �� �ֵ��� �մϴ�.
	*/
}

bool CGameServer::PostAccept()
{
	// ����ִ� ����( ������� �ʰ� �ִ� ) �� ���� ��� ���·� ����ϴ�.

	if (m_SessionQueue.empty() == true)
	{
		m_bIsAccepting = false;
		return false;
	}

	/*
	����ִ� Session�� ���� ���
	���̻� ������ ���� �ʽ��ϴ�.

	���̻� ������ ���� �� ���� ������
	m_bIsAccepting�� false�� �����Ͽ�
	������ �Ұ����� ���¶�� ǥ���մϴ�.
	*/

	m_bIsAccepting = true;

	int nSessionID = m_SessionQueue.front();
	m_SessionQueue.pop_front();

	/*
	��� �ִ� Session �迭���� ����ִ� Session ID�� �ݴϴ�.
	*/


	/*
	Ŭ���̾�Ʈ�� ������ ����� CloseSession( ) �Լ��� ����Ͽ� �ش� Session ��
	���� ��ȣ�� �ٽ� m_SessionQueue�� �����ϰ�, ���� ���� �ޱ� ��û�� �ϰ� ����
	�ʴٸ� ���� �ޱ� ��û�� �Ѵ�.
	*/

	m_acceptor.async_accept(m_vecSessionList[nSessionID]->GetSocket(), /* ����ִ� Session */
		boost::bind(&CGameServer::Handle_Accept,
			this,
			m_vecSessionList[nSessionID],
			boost::asio::placeholders::error)
	);

	/*
	async_accept �񵿱� ������ Ŭ���̾�Ʈ�� ������ �޽��ϴ�.

	async_accept( ������ Ŭ���̾�Ʈ�� �Ҵ��� ���� Ŭ����
	, Bind �Լ��� ����Ͽ� async_accept�� �Ϸ� �Ǹ� ȣ���� �Լ��� ���� )

	async_accept �� �Ϸ� �Ǹ� Handle_Accept�Լ��� ȣ��ȴ�.
	Handle_Accept�� ȣ��Ǹ� ���� ó�� �۾��� �Ϸ�Ǿ��ٴ� �� �̴�.

	*/

	return true;
}

void CGameServer::Handle_Accept(CSessionObj* pSession, const boost::system::error_code& error)
{
	/*
	Ŭ���̾�Ʈ ������ �Ϸ�Ǹ� ȣ�� �Ǵ� �Լ��Դϴ�.
	*/


	if (error == false)
	{
		std::cout << "Ŭ���̾�Ʈ ���� ���� . Session ID " << pSession->GetSessionID() << std::endl;

		pSession->Init();
		/*
		Session�� �̿��ϱ� ���� �ʱ�ȭ �մϴ�.
		*/


		pSession->PostReceive();

		/*
		������ �������� ������ ��� ������ Ŭ���̾�Ʈ�� ������ ��Ŷ�� ���� �� �ֵ���,
		Accept ( �޾� ���� ) �� ���� ���� ( Session�� m_Socket ) �� �ޱ� �۾��� ��û�ؾ��Ѵ�.

		Session Ŭ������ PostReceive �Լ��� ���� �ޱ� �۾��� ��û�Ѵ�.
		���� �� �۾��� ���� ���� ��� ���� ������ Ŭ���̾�Ʈ�� ������ �����͸� ���� ���Ѵ�.
		*/

		PostAccept();

		/*pSession �� ������ �����Ͽ��� ������ ����ִ� Session �� �ҷ��鿩�� ������ PostAccept() �Լ��� ȣ���մϴ�.*/
	}
	else
		std::cout << "Error No : " << error.value() << " Error Message : " << error.message() << std::endl;

	/* ������ �ִٸ� ������ ����մϴ�.*/
}


void CGameServer::ProcessPacket(const int iSessionID, const char* pData)
{
	SNET_PACKET_HEAD* pHeader = (SNET_PACKET_HEAD*)pData;

	m_pGameServerMsg->ProcessPacket(iSessionID, pHeader);
	return;
}

void CGameServer::PostSendAllSession(const bool bImmediately, const int nSize, char* pData, int iSendSeesion)
{
	std::vector<CSessionObj*>::iterator vit = m_vecConnectSession.begin();
	for (vit; vit != m_vecConnectSession.end(); ++vit)
	{
		if ((*vit)->GetSessionID() == iSendSeesion)
			continue;
		(*vit)->PostSend(bImmediately, nSize, pData);
	}
}

int const CGameServer::CreateObjFunction(const wstring wsID, D3DXVECTOR2 vPos, float fRoataion, const int iMasterSession)
{
	SGameObjData* pObjData = D_GAMEDATAMGR->GetGameObjData(wsID);
	if (pObjData == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	const int iCreateObjID = pObjData->m_iGameDataType;
	if (iCreateObjID == -1)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	CNetGameActor* pNetGameObjData = nullptr;

	switch (iCreateObjID)
	{
	case EGAMEOBJ::EGAMEOBJ_UNIT:
	{
		pNetGameObjData = D_GAMESERVERMGR->GetNetGameActorMgr()->CreateNetGameObjdata(pObjData, GetSpawnPos(GetSpawnCount() % m_iSpawnPointMax),
			D3DXVECTOR2(1.0f, 1.0f), fRoataion, 0, true, iMasterSession, true);

		break;
	}

	case EGAMEOBJ::EGAMEOBJ_BULLET:
	{
		pNetGameObjData = D_GAMESERVERMGR->GetNetGameActorMgr()->CreateNetGameObjdata(pObjData, vPos,
			D3DXVECTOR2(1.0f, 1.0f), fRoataion, 0, true, iMasterSession);

		break;
	}

	default:
		break;
	}

	if (pNetGameObjData == nullptr)
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	return pNetGameObjData->GetNetObjID();
}

D3DXVECTOR2 CGameServer::GetSpawnPos(int iPoint)
{
	D3DXVECTOR2 vPos[4]{ D3DXVECTOR2(100.0f,100.0f),D3DXVECTOR2(2400.0f,100.0f) ,D3DXVECTOR2(100.0f,2400.0f) ,D3DXVECTOR2(2400.0f,2400.0f) };
	return vPos[iPoint];
}

void CGameServer::PostSendSession(const int iSessionID, const bool bImmediately, const int iSize, char* pData)
{
	if (pData == nullptr)
		return;

	if (m_vecSessionList.size() < iSessionID)
		return;

	if (m_vecSessionList[iSessionID] == nullptr)
		return;

	m_vecSessionList[iSessionID]->PostSend(bImmediately, iSize, pData);
}