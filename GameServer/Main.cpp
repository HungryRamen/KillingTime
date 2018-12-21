#include "NetLogicHead.h"

using namespace std;

void OnTimer(const boost::system::error_code& error, boost::asio::steady_timer* pTimer);

void SetTimer(boost::asio::steady_timer* pTimer)
{
	pTimer->expires_from_now(std::chrono::milliseconds(16));

	pTimer->async_wait(boost::bind(OnTimer, boost::asio::placeholders::error,
		pTimer));
}

void OnTimer(const boost::system::error_code& error, boost::asio::steady_timer* pTimer)
{
	if (!error)
	{
		D_GAMESERVERMGR->OnNetFrameMove();
		SetTimer(pTimer);
	}
	else
	{
		std::cout << "Error No : " << error.value() << " Error Message : " << error.message() << std::endl;
	}
}

int main()
{
	D_GAMEDATAMGR->LoadData();
	D_GAMETILEMGR->LoadTileMap(L"../Data/0.2Map");

	boost::asio::io_service io_service;
	CGameServer Server(io_service);

	D_GAMESERVERMGR->SetGameserver(&Server);
	Server.Init(MAX_SESSION_COUNT);	// 서버 초기화 ( 최대 인원수 )
	Server.Start();					// 서버 시작

	boost::asio::steady_timer timer(io_service);
	SetTimer(&timer);

	io_service.run();

	getchar();
	return 0;
}