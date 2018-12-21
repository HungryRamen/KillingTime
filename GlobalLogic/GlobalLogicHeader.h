#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

using namespace std;

#ifndef D_GAMEDATAMGR
#define D_GAMEDATAMGR CGameDataMgr::GetGameDataMgr()
#endif    

#ifndef D_COLLIDERMGR
#define D_COLLIDERMGR CColliderMgr::GetColliderMgr()
#endif    

#include "../GlobalLogic/GameDataMgr.h"
#include "../GlobalLogic/NetGlobalHead.h"
#include "../GlobalLogic/Actor.h"
#include "../GlobalLogic/CColliderMgr.h"
#include "../GlobalLogic/Util.h"