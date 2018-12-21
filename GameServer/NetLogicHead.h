#pragma once

#include <iostream>
#include <deque>
#include <map>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include "../GlobalLogic/GlobalLogicHeader.h"


#include "SessionObj.h"
#include "GameServer.h"


#include "GameServerMsg.h"

#include "CBullet.h"

#include "NetObjContainer.h"
#include "CNetGameActorMgr.h"
#include "CGameServerMgr.h"

#include "GameTileMgr.h"


#define D_GAMESERVERMGR CGameServerMgr::GetGameServerMgr()
#define D_GAMEDATAMGR CGameDataMgr::GetGameDataMgr()
#define D_GAMETILEMGR CGameTileMgr::GetGameTileMgr()
