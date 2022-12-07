#include "../Game.h"
#include "Scene.h"
#include <random>

Scene::Scene(int iSceneNum) : m_nSceneNum(iSceneNum)
{
	m_nTileYLen = 18;
	FILE* fp = NULL;
	m_nNextSceneNum = -1;
	// set player position and background
	switch (m_nSceneNum) {
	case -10:				// test Scene;
	{
		m_nTileXLen = 32;
		m_nNextSceneNum = 999;

		// load moving objs

		// static objs
		Step* s0 = new Step(STEP_FOR::SF_ALL, { 700,550,750,575 });
		Step* s1 = new Step(STEP_FOR::SF_YELLOW, { 800,550,850,575 });
		Step* s2 = new Step(STEP_FOR::SF_PUR, { 900,550,950,575 });

		m_vSteps.push_back(s0);
		m_vSteps.push_back(s1);
		m_vSteps.push_back(s2);

		// tiles
		//m_vTiles;
		for (int i = 0; i < m_nTileYLen; ++i) {
			for (int j = 0; j < m_nTileXLen; ++j) {
				Tile* t;
				
				if (i == m_nTileYLen - 1) t = new Tile(TILE_DATA::TD_BLOCK);
				else if (i == m_nTileYLen - 2 && j >= m_nTileXLen / 2) t = new Tile(TILE_DATA::TD_BLOCK);
				else t = new Tile(TILE_DATA::TD_NON);

				m_vTiles.push_back(t);
			}
		}
	}
	break;

	case 1:
	case 2:
	case 3:
	{
		switch (m_nSceneNum) {
		case 1:		
			fp = fopen("Scene/scene_01.txt", "r");	
			//m_p0StartPos = { 600, 40 };
			//m_p1StartPos = { 620, 40 };
			m_p0StartPos = { 375, 1150 };
			m_p1StartPos = { 175, 1150 };
			break;
		case 2:		
			fp = fopen("Scene/stage2.txt", "r");	
			m_p0StartPos = { 40, 400 };
			m_p1StartPos = { 80, 400 };
			break;
		case 3:		
			fp = fopen("Scene/stage3.txt", "r");	
			m_p0StartPos = { 40, 400 };
			m_p1StartPos = { 80, 400 };
			break;
		default:	
			fp = fopen("Scene/scene_01.txt", "r");	
			m_p0StartPos = { 600, 40 };
			m_p1StartPos = { 620, 40 };
			break;
		}

		LoadMapFromFile(fp);
	}
	break;

	case END_SCENE:				// game clear;
		m_nTileYLen = 18;
		m_nTileXLen = 32;
		break;
	}



}

Scene::~Scene()
{
	ReleaseAndCleanVector(m_vPlayers);
	ReleaseAndCleanVector(m_vTiles);
	ReleaseAndCleanVector(m_vSteps);
	ReleaseAndCleanVector(m_vButton);
	ReleaseAndCleanVector(m_vRollerCoaster);
	ReleaseAndCleanVector(m_vMonster);
}

bool Scene::LoadMapFromFile(FILE* fp)
{
	if (fp == NULL) return false;
	
	// get x,y size
	fscanf(fp, "%d %d", &m_nTileXLen, &m_nTileYLen);

	// get tiles
	m_vTiles.reserve(m_nTileYLen * m_nTileXLen);
	for (int i = 0; i < m_nTileYLen; ++i) {
		for (int j = 0; j < m_nTileXLen; ++j) {
			TILE_DATA temp;
			fscanf(fp, "%d", &temp);

			Tile* t = new Tile(temp);

			m_vTiles.push_back(t);
		}
	}

	int size;
	// monster
	fscanf(fp, "%d", &size);
	m_vMonster.reserve(size);
	for (int i = 0; i < size; ++i) {
		RECT t;
		MOVE_DIR md;
		fscanf(fp, "%d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &md);

		Monster* temp = new Monster(t, md);
		//printf("%d %d %d %d\n", t.left, t.top, t.right, t.bottom);
		m_vMonster.push_back(temp);
	}

	// rollercoaster
	fscanf(fp, "%d", &size);
	m_vRollerCoaster.reserve(size);
	for (int i = 0; i < size; ++i) {
		RECT t;			// init position
		STEP_FOR sf;	// step for
		int b;			// always on
		int g;			// group number
		RECT mt;		// destination
		fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &sf, &b, &g, &mt.left, &mt.top, &mt.right, &mt.bottom);

		//RollerCoaster(RECT pos, STEP_FOR t, BOOL b, int g, RECT mt);
		RollerCoaster* temp = new RollerCoaster(t, sf, b, g, mt);

		m_vRollerCoaster.push_back(temp);
	}
	
	// step
	fscanf(fp, "%d", &size);
	m_vSteps.reserve(size);
	for (int i = 0; i < size; ++i) {
		RECT t;			// position
		STEP_FOR sf;	// step for
		int group;		// group number
		BOOL alive;		// always on
		fscanf(fp, "%d %d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &sf, &alive, &group);

		Step* temp = new Step(t, sf, alive, group);

		m_vSteps.push_back(temp);
	}

	// button
	fscanf(fp, "%d", &size);
	m_vButton.reserve(size);
	for (int i = 0; i < size; ++i) {
		RECT t;			// position
		BOOL fixed;		// is fixed
		int g;			// group number to controll
		fscanf(fp, "%d %d %d %d %d %d", &t.left, &t.top, &t.right, &t.bottom, &fixed, &g);

		Button* temp = new Button(t, fixed, g);

		m_vButton.push_back(temp);
	}

	fclose(fp);
	
	return true;
}

void Scene::ResetPlayerPos(int index)
{
	// m_vPlayers에는 플레이어 하나 당 두 오브젝트가 들어간다
	// index에 3이 들어왔다면 (0,1), (2,3), (4,5)이기 때문에 2,3의 위치를 시작위치로 초기화 해주어야 한다
	// 3 / 2 = 1
	// 1 * 2 = 2	- 2,3을 조종하는 플레이어의 첫번째 플레이어

	//printf("idx %d reset\n", index);

	int purpleIndex = (index / 2) * 2;
	int yellowIndex = purpleIndex + 1;

	m_vPlayers[purpleIndex]->SetPosition(m_p0StartPos);
	m_vPlayers[yellowIndex]->SetPosition(m_p1StartPos);
	m_vPlayers[purpleIndex]->SetBefPos(m_p0StartPos);
	m_vPlayers[yellowIndex]->SetBefPos(m_p1StartPos);
}

void Scene::InsertPlayers(int playerSize)
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> uid(-50.0f, 50.0f);

	for (int i = 0; i < playerSize; ++i) {
		FPOINT offset = { uid(dre), uid(dre) };

		Player* p0 = new PurplePlayer;
		Player* p1 = new YellowPlayer;

		FPOINT p0Pos = m_p0StartPos;
		FPOINT p1Pos = m_p1StartPos;
		
		p0Pos.x += offset.x;
		p0Pos.y += offset.y;

		p1Pos.x += offset.x;
		p1Pos.y += offset.y;

		p0->SetPosition(p0Pos);
		p1->SetPosition(p1Pos);

		m_vPlayers.push_back(p0);
		m_vPlayers.push_back(p1);
	}
}

void Scene::SetPlayerInput(int index, unsigned char move)
{
	int purpleIndex = index * 2;
	int yellowIndex = purpleIndex + 1;

	m_vPlayers[purpleIndex]->SetInputData(move);
	m_vPlayers[yellowIndex]->SetInputData(move);
}

FPOINT Scene::GetPlayerPosition(int index)
{
	return m_vPlayers[index]->GetPivot();
}

unsigned char Scene::GetPlayerInput(int index)
{
	return m_vPlayers[index * 2]->GetInputData();
}

bool Scene::IsPlayersUpdated() const
{
	for (const auto& player : m_vPlayers)
		if (player->IsPlayerUpdated()) return true;

	return false;
}

void Scene::Init()
{
	// player init
	for (auto const& d : m_vPlayers) d->Init();
	for (auto& d : m_vMonster) d->Init();
	for (auto& d : m_vSteps) d->Init();
}

void Scene::Input(float fTimeElapsed)
{
}

int Scene::Update(float fTimeElapsed)
{
	// 플레이어들을 입력값으로 움직인다
	for (auto& d : m_vPlayers) d->Update(fTimeElapsed);

	// 몬스터 업데이트(몬스터 이동 이라는 뜻)
	for (auto const& d : m_vMonster) d->Update(fTimeElapsed);

	// 롤러코스터 움직이라는 뜻
	for (auto& d : m_vRollerCoaster) d->Update(fTimeElapsed);

	return Collision();
}

int Scene::Collision()
{
	if (m_nSceneNum == END_SCENE) return 0;

	// 플레이어와 몬스터 충돌 확인
	bool bCollide = false;
	for (int i = 0; i < m_vPlayers.size(); ++i) {
		for (auto& dMonster : m_vMonster) {
			FRECT player = m_vPlayers[i]->GetPosition();
			FRECT monster = dMonster->GetPosition();

			if (player.IntersectRect(monster)) {
				ResetPlayerPos(i);
			}
		}
	}

	// 플레이어와 타일맵 충돌 확인
	for (int i = 0; i < m_vPlayers.size(); ++i) {
		FRECT playerPos = m_vPlayers[i]->GetPosition();

		FPOINT tLeft = { (LONG)floor((playerPos.left) / 40.0f), (LONG)floor((playerPos.bottom) / 40.0f) };
		FPOINT tRight = { (LONG)floor((playerPos.right) / 40.0f), (LONG)floor((playerPos.bottom) / 40.0f) };

		// 바닥
		int LB = tLeft.y * m_nTileXLen + tLeft.x;
		int RB = tRight.y * m_nTileXLen + tRight.x;
		TILE_DATA leftBottom = (0 <= LB && LB < m_vTiles.size()) ? m_vTiles[LB]->GetTile() : TILE_DATA::TD_NON;
		TILE_DATA rightBottom = (0 <= RB && RB < m_vTiles.size()) ? m_vTiles[RB]->GetTile() : TILE_DATA::TD_NON;

		if (leftBottom == TILE_DATA::TD_BLOCK || rightBottom == TILE_DATA::TD_BLOCK ||
			leftBottom == TILE_DATA::TD_FLOOR || rightBottom == TILE_DATA::TD_FLOOR) {
			m_vPlayers[i]->Move(0, 40 * tLeft.y - playerPos.bottom);
			m_vPlayers[i]->SetFallingFalse();
		}
		else if (leftBottom == TILE_DATA::TD_SPIKE || rightBottom == TILE_DATA::TD_SPIKE) {
			ResetPlayerPos(i);
		}
		else if (leftBottom == TILE_DATA::TD_GOAL || rightBottom == TILE_DATA::TD_GOAL) {
			// Stage Clear
			std::cout << "hit goal" << std::endl;
			// (주의) 스테이지 클리어 나중에 추가할 것
			if (i == 0 || i == 1)
				return 0;
			else if (i == 2 || i == 3)
				return 1;
			else {
				return 2;
			}
		}
		else m_vPlayers[i]->SetFallingTrue();						// NON, go fall

		// 벽?
		LB = tLeft.y * m_nTileXLen + tLeft.x - m_nTileXLen;
		RB = tRight.y * m_nTileXLen + tRight.x - m_nTileXLen;

		if (LB < 0 || RB < 0) {
			m_vPlayers[i]->SetFallingTrue();
			break;
		}
		//else if(LB > )

		if (LB < 0 || m_vTiles.size() <= LB || RB < 0 || m_vTiles.size() <= RB) break;

		if (m_vTiles[LB]->GetTile() == TILE_DATA::TD_BLOCK ||
			m_vTiles[LB]->GetTile() == TILE_DATA::TD_FLOOR ||
			m_vTiles[RB]->GetTile() == TILE_DATA::TD_BLOCK ||
			m_vTiles[RB]->GetTile() == TILE_DATA::TD_FLOOR)
			m_vPlayers[i]->GoBackX();
		else if (((0 <= LB && LB < m_vTiles.size()) &&
			(0 <= RB && RB < m_vTiles.size())) &&
			m_vTiles[LB]->GetTile() == TILE_DATA::TD_GOAL ||
			m_vTiles[RB]->GetTile() == TILE_DATA::TD_GOAL) {
			std::cout << "hit goal" << std::endl;
			// (주의) 스테이지 클리어 나중에 추가할 것
			return CLEAR_STAGE;
		}
	}

	// 플레이어와 장애물 충돌 확인		(step, rollercoaster, button)
	for (auto& dPlayer : m_vPlayers) {
		// step
		FRECT playerPos = dPlayer->GetPosition();
		for (auto const dStep : m_vSteps) {
			FRECT stepPosition = dStep->GetPosition();

			if (!(dStep->IsAlive() + dStep->GetCount())) continue;

			if ((int)(dStep->GetType()) - dPlayer->GetPlayerNum() && playerPos.IntersectRect(stepPosition)) {
				// hit side
				if (playerPos.bottom > stepPosition.bottom && playerPos.top < stepPosition.top)
					dPlayer->GoBackX();

				// hit top
				else if (playerPos.bottom - stepPosition.bottom < 0) {
					float offset = stepPosition.top - playerPos.bottom + 0.1f;
					dPlayer->Move(0, offset);
					dPlayer->SetFallingFalse();
				}
				// hit bottom
				else if (playerPos.top - stepPosition.bottom < 0) {
					float offset = stepPosition.bottom - playerPos.top + 0.1f;
					dPlayer->Move(0, offset);
					dPlayer->OnHitCeil();
				}
			}
		}

		// rollercoaster
		for (auto const dRCoaster : m_vRollerCoaster) {
			FRECT rcPosition = dRCoaster->GetPosition();

			if ((int)(dRCoaster->GetType()) - dPlayer->GetPlayerNum() && playerPos.IntersectRect(rcPosition)) {
				// hit side
				if (playerPos.bottom > rcPosition.bottom && playerPos.top < rcPosition.top)
					dPlayer->GoBackX();

				// hit top
				else if (playerPos.bottom - rcPosition.bottom < 0) {
					float offset = rcPosition.top - playerPos.bottom + 1.0f;
					dPlayer->Move(0, offset);
					dPlayer->SetFallingFalse();
				}
				// hit bottom
				else if (playerPos.top - rcPosition.bottom < 0) {
					float offset = rcPosition.bottom - playerPos.top;
					dPlayer->Move(0, offset);
					dPlayer->OnHitCeil();
				}
			}
		}
	}

	// 플레이어와 플레이어 충돌 확인
	// (주의) 여기 부분 살짝 수정 필요

	for (int i = 0; i < m_vPlayers.size(); ++i) {
		Player* p0 = m_vPlayers[i];
		FRECT p0Pos = p0->GetPosition();
		for (int j = i + 1; j < m_vPlayers.size(); ++j) {
			Player* p1 = m_vPlayers[j];
			FRECT p1Pos = p1->GetPosition();

			if (p0Pos.IntersectRect(p1Pos)) {
				if (p0Pos.IntersectRect(p1Pos)) {
					// p0 is higher position
					if (p0Pos.bottom - p1Pos.bottom < -20) {
						float offset = p1Pos.top - p0Pos.bottom + 0.1f;
						p0->Move(0, offset);
						p0->SetFallingFalse();
					}
					else if (p1Pos.bottom - p0Pos.bottom < -20) {
						float offset = p0Pos.top - p1Pos.bottom + 0.1f;
						p1->Move(0, offset);
						p1->SetFallingFalse();
					}
					else {
						p1->GoBackX();
						p0->GoBackX();
					}
				}
			}
		}
	}




	// button
	// turn off everything
	for (auto& d : m_vRollerCoaster) d->DeActive();
	for (auto& d : m_vSteps) if (!d->IsAlive()) d->DeActive();
	for (auto& dButton : m_vButton) {

		for (auto& dPlayer : m_vPlayers) {
			if (dPlayer->GetPosition().IntersectRect(dButton->GetPosition())) {
				//if (dButton->IsActive()) break;
				dButton->SetActiveState(true);

				// turn on everything
				for (auto& d : m_vRollerCoaster) if (d->GetGroup() == dButton->getGroupCtrl())			d->Active();
				for (auto& d : m_vSteps) if (!d->IsAlive() && d->GetGroup() == dButton->getGroupCtrl()) d->Active();
				break;
			}
			else {
				if (dButton->IsFixed()) 
					break;
				dButton->SetActiveState(false);
			}
		}
	}

	// 몬스터와 몬스터 충돌
	for (auto iter = m_vMonster.begin(); iter != m_vMonster.end(); ++iter) {
		for (auto inIt = (iter + 1); inIt != m_vMonster.end(); ++inIt) {
			if ((*iter)->GetPosition().IntersectRect((*inIt)->GetPosition())) {
				(*iter)->GoBack();
				(*inIt)->GoBack();

				(*iter)->Reverse();
				(*inIt)->Reverse();
				break;
			}
		}
	}
	// 몬스터와 타일맵 충돌(벽)
	for (auto d : m_vMonster) {
		POINT t;
		switch (d->GetDirection()) {
		case MOVE_DIR::MD_BACK:		t = { (LONG)(d->GetPosition().left), (LONG)(d->GetPosition().top) };	break;
		case MOVE_DIR::MD_FRONT:	t = { (LONG)(d->GetPosition().right), (LONG)(d->GetPosition().top) };	break;
		default: continue;
		}
		t.x = t.x / 40;
		t.y = t.y / 40;
		//tLeft.y* m_pScene->m_nTileXLen + tLeft.x
		if (m_vTiles[t.y * m_nTileXLen + t.x]->GetTile() == TILE_DATA::TD_BLOCK ||
			m_vTiles[t.y * m_nTileXLen + t.x]->GetTile() == TILE_DATA::TD_FLOOR)
			d->Reverse();
	}

	//if (!bCollide) {
	//	m_pScene->m_vPlayer.front()->SetFallingTrue();
	//	m_pScene->m_vPlayer.back()->SetFallingTrue();
	//}
	return -1;
}

