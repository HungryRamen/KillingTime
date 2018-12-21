#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <deque>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "../GlobalLogic/NetGlobalHead.h"
using namespace std;
#define Init_Vector2 D3DXVECTOR2(0.0f,0.0f)

#include "DXUT.h"

#include "TextureMgr.h"
#include "SpriteMgr.h"
#include "CameraMgr.h"

#include "GameObj.h"
#include "UnitObj.h"
#include "CBulletObj.h"
#include "CHPBar.h"

#include "Render.h"
#include "ImgRender.h"
#include "AniRender.h"
#include "CutImgRender.h"

#include "CFSM.h"

#include "../GlobalLogic/GlobalLogicHeader.h"
#include "../GlobalLogic/GameDataMgr.h"

#include "GameObjContainer.h"
#include "GameObjMgr.h"
#include "ConnectMgr.h"
#include "GameTileMgr.h"

#include "ConnectMsg.h"

#include "CGameObjController.h"
#include "CPlayerController.h"

#include "CGameStage.h"
#include "CGameMainStage.h"
#include "CGameLobyStage.h"

#include "CGameStageMgr.h"
#include "MouseMgr.h"
#include "FontMgr.h"

#define D_CAMERAMGR CCameraMgr::GetCameraMgr()
#define D_SPRITEMGR CSpriteMgr::GetSpriteMgr()
#define D_TEXTUREMGR CTextureMgr::GetTextureMgr()
#define D_GAMEOBJMGR CGameObjMgr::GetGameObjMgr()
#define D_CONNECTMGR CConnectMgr::GetConectMgr()
#define D_GAMESTAGEMGR CGameStageMgr::GetGameStageMgr()
#define D_GAMEDATAMGR CGameDataMgr::GetGameDataMgr()
#define D_MOUSEMGR CMouseMgr::GetMouseMgr()
#define D_GAMETILEMGR CGameTileMgr::GetGameTileMgr()
#define D_FONT CFontMgr::GetFontMgr()