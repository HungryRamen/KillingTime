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
		Session( 세션 ID, io_service, ChattingServer );
		*/


		m_vecSessionList.push_back(pSession);
		m_SessionQueue.push_back(i);

		/*
		Session 클래스 ( 서버에 접속한 클라이언트 ) 를 최대 접속 수만큼 미리 할당해서
		m_SessionList 에 담아 놓습니다.

		새로운 접속이 있을 때마다 사용하지 않는 Session을 할당하기 위해 아직 사용하지 않는
		Session의 인덱스 번호를 m_SessionQueue에 저장해 놓는다
		*/
	}
}

void CGameServer::Start()
{
	std::cout << " 서버 시작 .... " << std::endl;

	PostAccept();
}

void CGameServer::CloseSession(const int nSessionID)
{
	std::cout << "클라이언트 접속 종료, 세션 ID : " << nSessionID << std::endl;

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
	Session ( 서버에 접속한 클라이언트 ) 가 접속을 종료하였다면
	Socekt을 닫고 SessionQueue 에 세션 ID를 넣어 새로운 접속을 받을수 있게합니다.
	*/

	if (m_bIsAccepting == false)
		PostAccept();

	/*
	접속하고 있는 클라이언트가 많아 더이상 접속을 받을 수 없는 상태였다면
	PostAccept() 함수를 호출하여 접속이 가능한 상태라고 알려주고
	위 Session 접속을 받을 수 있도록 합니다.
	*/
}

bool CGameServer::PostAccept()
{
	// 비어있는 세션( 사용하지 않고 있는 ) 를 접속 대기 상태로 만듭니다.

	if (m_SessionQueue.empty() == true)
	{
		m_bIsAccepting = false;
		return false;
	}

	/*
	비어있는 Session이 없을 경우
	더이상 접속을 받지 않습니다.

	더이상 접속을 받을 수 없기 때문에
	m_bIsAccepting을 false로 변경하여
	접속이 불가능한 상태라고 표시합니다.
	*/

	m_bIsAccepting = true;

	int nSessionID = m_SessionQueue.front();
	m_SessionQueue.pop_front();

	/*
	비어 있는 Session 배열에서 비어있는 Session ID를 줍니다.
	*/


	/*
	클라이언트의 접속이 끊기면 CloseSession( ) 함수를 사용하여 해당 Session 인
	덱스 번호를 다시 m_SessionQueue에 저장하고, 현재 접속 받기 요청을 하고 있지
	않다면 접속 받기 요청을 한다.
	*/

	m_acceptor.async_accept(m_vecSessionList[nSessionID]->GetSocket(), /* 비어있는 Session */
		boost::bind(&CGameServer::Handle_Accept,
			this,
			m_vecSessionList[nSessionID],
			boost::asio::placeholders::error)
	);

	/*
	async_accept 비동기 식으로 클라이언트의 접속을 받습니다.

	async_accept( 접속한 클라이언트에 할당할 소켓 클래스
	, Bind 함수를 사용하여 async_accept가 완료 되면 호출할 함수를 랩핑 )

	async_accept 가 완료 되면 Handle_Accept함수가 호출된다.
	Handle_Accept는 호출되면 접속 처리 작업이 완료되었다는 뜻 이다.

	*/

	return true;
}

void CGameServer::Handle_Accept(CSessionObj* pSession, const boost::system::error_code& error)
{
	/*
	클라이언트 접속이 완료되면 호출 되는 함수입니다.
	*/


	if (error == false)
	{
		std::cout << "클라이언트 접속 성공 . Session ID " << pSession->GetSessionID() << std::endl;

		pSession->Init();
		/*
		Session을 이용하기 위해 초기화 합니다.
		*/


		pSession->PostReceive();

		/*
		접속이 에러없이 성공한 경우 접속한 클라이언트가 보내는 패킷을 받을 수 있도록,
		Accept ( 받아 들인 ) 된 받은 소켓 ( Session의 m_Socket ) 에 받기 작업을 요청해야한다.

		Session 클래스의 PostReceive 함수를 통해 받기 작업을 요청한다.
		만약 이 작업을 하지 않을 경우 지금 접속한 클라이언트가 보내는 데이터를 받지 못한다.
		*/

		PostAccept();

		/*pSession 이 접속을 성공하였기 때문에 비어있는 Session 을 불러들여야 함으로 PostAccept() 함수를 호출합니다.*/
	}
	else
		std::cout << "Error No : " << error.value() << " Error Message : " << error.message() << std::endl;

	/* 에러가 있다면 에러를 출력합니다.*/
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