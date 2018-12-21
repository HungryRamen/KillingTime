#include "../GlobalLogic/GlobalLogicHeader.h"

void SGameObjData::Load(wifstream* wifRead)
{
	wstring wsCommand;
	// 추후 다른 방향으로 설정해야함.
	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	int iColliderIndex = 0;

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;
		if (wsCommand == L"ID")
		{
			((*wifRead)) >> m_wsID;
			continue;
		}

		if (wsCommand == L"RenderID")
		{
			((*wifRead)) >> m_wsRenderDataID;
			continue;
		}

		if (wsCommand == L"Camera")
		{
			((*wifRead)) >> m_bCamera;
			continue;
		}

		//if (wsCommand == L"AddRenderImg")
		//{
		//	while (!(*wifRead).eof())
		//	{
		//		((*wifRead)) >> wsCommand;
		//		if (wsCommand == L"RenderID")
		//		{
		//			((*wifRead)) >> wsCommand;
		//			m_vecRenderDataID.push_back(wsCommand);
		//		}
		//		else if (wsCommand == L"ColliderData")
		//			break;
		//	}
		//}

		// 이부분 새로 정리가 필요함 더미 데이터 로드시 문제가 발생할 수 있음.
		if (wsCommand == L"ColliderData")
		{
			while (!(*wifRead).eof())
			{
				wstringstream wsCollider;
				int iColliderModel;
				int iColliderType;

				((*wifRead)) >> wsCommand;

				wsCollider << L"Collider" << iColliderIndex;
				if (wsCollider.str().c_str() == wsCommand)
				{
					if (wsCommand == L"end")
						break;

					((*wifRead)) >> wsCommand;
					if (wsCommand == L"ColliderModel")
					{
						wstring wsModel;
						((*wifRead)) >> wsModel;
						iColliderModel = D_GAMEDATAMGR->ChangeColliderModelStringIDToNumID(wsModel);
					}

					((*wifRead)) >> wsCommand;
					if (wsCommand == L"ColliderType")
					{
						wstring wsColliderType;
						((*wifRead)) >> wsColliderType;
						iColliderType = D_GAMEDATAMGR->ChangeColliderTypeStringIDToNumID(wsColliderType);
					}


					SColliderData* pColliderData = D_GAMEDATAMGR->AllocColliderModel(iColliderModel);
					if (pColliderData == nullptr)
						continue;

					pColliderData->m_iColliderModel = iColliderModel;
					pColliderData->m_iColliderType = iColliderType;
					pColliderData->m_iColliderNum = iColliderIndex;

					pColliderData->Load(wifRead);

					switch (pColliderData->m_iColliderType)
					{
					case ECOLLIDER_ATTACK:
						m_vecATKCollider.push_back(pColliderData);
						++iColliderIndex;
						continue;

					case ECOLLIDER_DEFENCE:
						m_vecDEFCollider.push_back(pColliderData);
						++iColliderIndex;
						continue;

					default:
						++iColliderIndex;
						continue;
					}

					++iColliderIndex;
					continue;
				}
				else
					break;
			}
		}
	}
}

void SColliderData::Load(wifstream* wifRead)
{
	wstring wsCommand;

	((*wifRead)) >> wsCommand;
	if (wsCommand == L"PosX")
		((*wifRead)) >> m_vPos.x;

	((*wifRead)) >> wsCommand;
	if (wsCommand == L"PosY")
		((*wifRead)) >> m_vPos.y;

}

void SCircleColliderData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;
		if (wsCommand == L"Radius")
		{
			((*wifRead)) >> m_iRadius;
			continue;
		}

		if (wsCommand == L"End")
			break;
	}
}

void SSquareColliderData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;
		if (wsCommand == L"Width")
		{
			((*wifRead)) >> m_iWidth;
			continue;
		}

		if (wsCommand == L"Height")
		{
			((*wifRead)) >> m_iHeight;
			continue;
		}

		if (wsCommand == L"End")
			break;
	}
}

void SStateGameObjData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;

	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;

		if (wsCommand == L"RenderState")
		{
			int iStateID = 0;
			while (!(*wifRead).eof())
			{
				((*wifRead)) >> wsCommand;

				wstringstream wsState;
				wsState << L"State" << iStateID;

				if (wsCommand == wsState.str().c_str())
				{
					((*wifRead)) >> wsCommand;
					m_vecRenderDataID.push_back(wsCommand);
					++iStateID;
					continue;
				}

				if (wsCommand == L"End")
					return;
			}
		}
	}
}

void SUnitObjData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;

	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;

		if (wsCommand == L"End" || wsCommand == L"ColliderData")
			return;

		if (wsCommand == L"Speed")
		{
			((*wifRead)) >> m_fSpeed;
			continue;
		}

		if (wsCommand == L"HP")
		{
			((*wifRead)) >> m_iHp;
			continue;
		}

		if (wsCommand == L"Damage")
		{
			((*wifRead)) >> m_iDamage;
			continue;
		}
	}
}

void SBulletObjData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;

	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;
		if (wsCommand == L"End" || wsCommand == L"ColliderData")
			return;

		if (wsCommand == L"LifeTime")
		{
			((*wifRead)) >> m_fLifeTime;
			continue;
		}
	}
}

void SRenderData::Load(wifstream* wifRead)
{
	wstring wsCommand;

	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;
		if (wsCommand == L"ID")
		{
			((*wifRead)) >> m_wsID;
			continue;
		}

		if (wsCommand == L"AnchorX")
		{
			((*wifRead)) >> m_vAnchor.x;
			continue;
		}

		if (wsCommand == L"AnchorY")
		{
			((*wifRead)) >> m_vAnchor.y;
			continue;
		}
	}
}

void SImgRenderData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;
	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;
		if (wsCommand == L"ImgFile")
		{
			((*wifRead)) >> m_wsImgFile;
			continue;
		}
	}
}

void SAniRenderData::Load(wifstream* wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;
	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;

		if (wsCommand == L"ImgCount")
		{
			((*wifRead)) >> m_iImgCount;
			continue;
		}

		if (wsCommand == L"TrackSpeed")
		{
			((*wifRead)) >> m_fTrackSpeed;
			continue;
		}

		if (wsCommand == L"Loop")
		{
			((*wifRead)) >> m_bLoop;
			continue;
		}
		
		if (wsCommand == L"State")
		{
			int iState = -1;
			((*wifRead)) >> iState;
			switch (iState)
			{
			case 1:
				m_eAniState = AT_MOVE;
				break;
			case 2:
				m_eAniState = AT_ATK;
				break;
			case 3:
				m_eAniState = AT_DIE;
				break;
			}
			continue;
		}

		if (wsCommand == L"ImgFile")
		{
			m_vecImgFile.clear();
			((*wifRead)) >> wsCommand;
			for (int i = 0; i < m_iImgCount; ++i)
			{
				wstringstream os;
				os << wsCommand << i << L".png";
				m_vecImgFile.push_back(os.str());
			}
		}
	}
}

void SCutImgRenderData::Load(wifstream * wifRead)
{
	__super::Load(wifRead);

	wstring wsCommand;
	(*wifRead).clear();
	(*wifRead).seekg(ios::beg);

	while (!(*wifRead).eof())
	{
		((*wifRead)) >> wsCommand;

		if (wsCommand == L"Left")
		{
			((*wifRead)) >> m_iLeft;
			continue;
		}
		if (wsCommand == L"Top")
		{
			((*wifRead)) >> m_iTop;
			continue;
		}
		if (wsCommand == L"Right")
		{
			((*wifRead)) >> m_iRight;
			continue;
		}
		if (wsCommand == L"Bottom")
		{
			((*wifRead)) >> m_iBottom;
			continue;
		}

		if (wsCommand == L"ImgFile")
		{
			((*wifRead)) >> m_wsImgFile;
			continue;
		}
	}
}

CGameDataMgr* CGameDataMgr::m_pGameDataMgr = nullptr;

CGameDataMgr::CGameDataMgr()
{
}


CGameDataMgr::~CGameDataMgr()
{
}

//  추후 제거해야함.
void CGameDataMgr::LoadData()
{
	LoadGameObjData(L"../Data/Unit/Player.data");
	LoadRenderData(L"../Data/Unit/Player_Render.data");
	LoadRenderData(L"../Data/Unit/Player_Render2.data");
	LoadRenderData(L"../Data/Unit/Player_Render3.data");
	LoadRenderData(L"../Data/Unit/Player_Render4.data");

	LoadGameObjData(L"../Data/Bullet/Bullet1.data");
	LoadRenderData(L"../Data/Bullet/Bullet1_Render.data");
	LoadRenderData(L"../Data/Bullet/Bullet2_Render.data");

	LoadGameObjData(L"../Data/Map/Map.data");
	LoadRenderData(L"../Data/Map/Map_Render.data");

	LoadGameObjData(L"../Data/UI/HPBar.data");
	LoadRenderData(L"../Data/UI/HPBar_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile1.data");
	LoadRenderData(L"../Data/Tile/Tile1_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile2.data");
	LoadRenderData(L"../Data/Tile/Tile2_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile3.data");
	LoadRenderData(L"../Data/Tile/Tile3_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile4.data");
	LoadRenderData(L"../Data/Tile/Tile4_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile5.data");
	LoadRenderData(L"../Data/Tile/Tile5_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile6.data");
	LoadRenderData(L"../Data/Tile/Tile6_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile7.data");
	LoadRenderData(L"../Data/Tile/Tile7_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile8.data");
	LoadRenderData(L"../Data/Tile/Tile8_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile9.data");
	LoadRenderData(L"../Data/Tile/Tile9_Render.data");

	LoadGameObjData(L"../Data/Tile/Tile10.data");
	LoadRenderData(L"../Data/Tile/Tile10_Render.data");
}

SColliderData* CGameDataMgr::AllocColliderModel(int iType)
{
	switch (iType)
	{
	case ECOLLIDER_CIRCLE:
		return new SCircleColliderData;

	case ECOLLIDER_SQUARE:
		return new SSquareColliderData;

	default:
		break;
	}

	return nullptr;
}

SGameObjData* CGameDataMgr::AllocData(int iType)
{
	switch (iType)
	{
	case EGAMEOBJDATA_NORMAL:
		return new SGameObjData;

	case EGAMEOBJDATA_UNIT:
		return new SUnitObjData;

	case EGAMEOBJDATA_BULLET:
		return new SBulletObjData;

	default:
		break;
	}

	return nullptr;
}

SRenderData* CGameDataMgr::AllocRenderData(int iType)
{
	switch (iType)
	{
	case ERENDERDATA_IMGRENDER:
		return new SImgRenderData;
	case ERENDERDATA_ANIRENDER:
		return new SAniRenderData;
	case ERENDERDATA_CUTIMGRENDER:
		return new SCutImgRenderData;
	default:
		break;
	}

	return nullptr;
}

// 이부분은 요걸 관리하는 파일을 만들어서 불러들이도록 하자. ( 예 : map< wstrig, int > m_mapGameDataID )
const int CGameDataMgr::ChangeStringIDToNumID(const wstring wsID)
{
	if (wsID == L"Normal")
		return EGAMEOBJDATA_NORMAL;
	else if (wsID == L"Unit")
		return EGAMEOBJDATA_UNIT;
	else if (wsID == L"Bullet")
		return EGAMEOBJDATA_BULLET;
	else
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
}

// 이부분은 요걸 관리하는 파일을 만들어서 불러들이도록 하자. ( 예 : map< wstrig, int > m_mapGameDataID )
const int CGameDataMgr::ChangeRenderStringIDToNumID(const wstring wsID)
{
	if (wsID == L"ImgRender")
		return ERENDERDATA_IMGRENDER;
	else if (wsID == L"AniRender")
		return ERENDERDATA_ANIRENDER;
	else if (wsID == L"CutImgRender")
		return ERENDERDATA_CUTIMGRENDER;
	else
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
}

const int CGameDataMgr::ChangeColliderModelStringIDToNumID(const wstring wsID)
{
	if (wsID == L"Circle")
		return ECOLLIDER_CIRCLE;
	else if (wsID == L"Square")
		return ECOLLIDER_SQUARE;
	else
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
}

const int CGameDataMgr::ChangeColliderTypeStringIDToNumID(const wstring wsID)
{
	if (wsID == L"Attack")
		return ECOLLIDER_ATTACK;
	else if (wsID == L"Defence")
		return ECOLLIDER_DEFENCE;
	else
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
}

const int CGameDataMgr::LoadGameObjData(const wstring wsFile)
{
	int iGameDataType = -1;

	wifstream wifRead(wsFile.c_str());

	wstring wsCommand;
	wstring wsValue;

	if (wifRead.fail())
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	if (!wifRead.eof())
	{
		wifRead >> wsCommand;
		if (wsCommand == L"GameDataType")
		{
			wifRead >> wsValue;
			iGameDataType = ChangeStringIDToNumID(wsValue);
		}
		else
		{
			wifRead.clear();
			wifRead.close();
			return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
			// 이부분은 나중에 함수로 빼자 초기화 구문으로
		}
	}

	if (iGameDataType == EMSG_SYSTEM_FAIL)
		return EMSG_SYSTEM_FAIL;

	SGameObjData* pObjData = AllocData(iGameDataType);
	if (pObjData == nullptr)
		return EMSG_SYSTEM_FAIL;

	pObjData->m_wsFile = wsFile.c_str();
	pObjData->m_iGameDataType = iGameDataType;
	pObjData->Load(&wifRead);

	wifRead.clear();
	wifRead.close();

	map< wstring, SGameObjData* >::iterator mit = m_mapGameObjData.find(pObjData->m_wsID);
	if (mit == m_mapGameObjData.end())
		m_mapGameObjData.insert(make_pair(pObjData->m_wsID, pObjData));

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

const int CGameDataMgr::LoadRenderData(const wstring wsFile)
{
	int iRenderType = -1;

	wifstream wifRead(wsFile.c_str());

	wstring wsCommand;
	wstring wsValue;

	if (wifRead.fail())
		return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;

	if (!wifRead.eof())
	{
		wifRead >> wsCommand;
		if (wsCommand == L"RenderDataType")
		{
			wifRead >> wsValue;
			iRenderType = ChangeRenderStringIDToNumID(wsValue);
		}
		else
		{
			wifRead.clear();
			wifRead.close();
			return EMSG_SYSTEM::EMSG_SYSTEM_FAIL;
			// 이부분은 나중에 함수로 빼자 초기화 구문으로
		}
	}

	if (iRenderType == EMSG_SYSTEM_FAIL)
		return EMSG_SYSTEM_FAIL;

	SRenderData* pRenderData = AllocRenderData(iRenderType);
	if (pRenderData == nullptr)
		return EMSG_SYSTEM_FAIL;

	pRenderData->m_wsFile = wsFile.c_str();
	pRenderData->m_iRenderType = iRenderType;
	pRenderData->Load(&wifRead);

	wifRead.clear();
	wifRead.close();

	map< wstring, SRenderData* >::iterator mit = m_mapRenderData.find(pRenderData->m_wsID);
	if (mit == m_mapRenderData.end())
		m_mapRenderData.insert(make_pair(pRenderData->m_wsID, pRenderData));

	return EMSG_SYSTEM::EMSG_SYSTEM_SUCCESS;
}

SGameObjData* CGameDataMgr::GetGameObjData(const wstring wsID)
{
	map< wstring, SGameObjData* >::iterator mit = m_mapGameObjData.find(wsID);
	if (mit == m_mapGameObjData.end())
		return nullptr;

	return mit->second;
}

SRenderData* CGameDataMgr::GetRenderData(const wstring wsID)
{
	map< wstring, SRenderData* >::iterator mit = m_mapRenderData.find(wsID);
	if (mit == m_mapRenderData.end())
		return nullptr;

	return mit->second;
}

const STileMapData * CGameDataMgr::LoadTileMap(const wstring wsFile)
{
	if (wsFile.empty() == true)
		return nullptr;

	FILE* fp = _wfsopen(wsFile.c_str(), L"rb", _SH_DENYNO);
	STileMapData* pTileData = new STileMapData;

	if (fp == nullptr)
		return nullptr;

	fread(pTileData, sizeof(struct STileMapData_NoneVector), 1, fp);
	pTileData->m_vecTileObjData.clear();

	int iTileSize = pTileData->m_iTileSize;

	for (int i = 0; i < iTileSize; ++i)
	{
		STileObjData* pTileObjData = new STileObjData;
		fread(pTileObjData, sizeof(struct STileObjData), 1, fp);
		if (pTileObjData != nullptr)
		{
			pTileData->m_vecTileObjData.push_back((*pTileObjData));
		}
	}

	return pTileData;
}
