#include "Scene.h"
#include "../Core.h"



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

		m_imgBackGround.Load(TEXT("Resource/bgTemp.png"));

		Player* p0 = new PurplePlayer;
		Player* p1 = new YellowPlayer;

		m_vPlayer.push_back(p0);
		m_vPlayer.push_back(p1);
		
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
	{
		m_imgBackGround.Load(TEXT("Resource/tempBGv2.bmp"));
		m_imgTile.Load(TEXT("Resource/tempsprite.bmp"));

		Player* p0 = new PurplePlayer;
		Player* p1 = new YellowPlayer;

		p0->SetPosition({ 375,3500,400,3550 });
		p1->SetPosition({ 175,3500,200,3550 });

		//m_p0StartPos = { 375,1150,400,1200 };
		//m_p1StartPos = { 175,1150,200,1200 };

		m_p0StartPos = { 1400,0,1425,50 };
		m_p1StartPos = { 1460,0,1485,50 };

		p0->SetPosition(m_p0StartPos);
		p1->SetPosition(m_p1StartPos);


		m_vPlayer.push_back(p0);
		m_vPlayer.push_back(p1);


		fp = fopen("Scene/scene_01.txt", "r");
		LoadMapFromFile(fp);
	}
		break;

	case END_SCENE:				// game clear;
		m_nTileYLen = 18;
		m_nTileXLen = 32;
		m_imgBackGround.Load(TEXT("Resource/gameover.bmp"));
		break;
	}



}

Scene::~Scene()
{
	DeleteObject(m_hDoubleBufferBitmap);
	m_imgTile.Destroy();
	m_imgBackGround.Destroy();
	ReleaseAndCleanVector(m_vPlayer);
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

void Scene::ResetPlayerPos()
{
	m_vPlayer.front()->SetPosition(m_p0StartPos);
	m_vPlayer.back()->SetPosition(m_p1StartPos);
}

void Scene::Init()
{
	// player init
	for (auto const& d : m_vPlayer) d->Init();
	for (auto& d : m_vMonster) d->Init();
	for (auto& d : m_vSteps) d->Init();
}

void Scene::Input(float fTimeElapsed)
{
	// player Move
	uint8_t newDir = 0;
	for (auto const& d : m_vPlayer) d->Input(fTimeElapsed, newDir);
	
	Core::GetInst().GetNetworkManager()->ClientDoSendMovePacket(newDir);
	//Core::GetInst().GetNetworkManager()->SendPlayerPacket();

	// camera
	FPOINT pCenter;
	if (m_vPlayer.empty()) return;

	pCenter.x = (m_vPlayer.front()->GetPosition().left + m_vPlayer.back()->GetPosition().left) / 2;
	pCenter.y = (m_vPlayer.front()->GetPosition().bottom + m_vPlayer.back()->GetPosition().bottom) / 2;

	m_CameraOffset.x = pCenter.x - m_CameraRectSize.cx / 2;
	m_CameraOffset.y = pCenter.y - m_CameraRectSize.cy / 2;

	if (m_CameraOffset.x < 0) m_CameraOffset.x = 0;
	else if (m_CameraOffset.x > TILESIZE * m_nTileXLen - m_CameraRectSize.cx) m_CameraOffset.x = (float)(TILESIZE * m_nTileXLen - m_CameraRectSize.cx);

	if (m_CameraOffset.y < 0) m_CameraOffset.y = 0;
	else if (m_CameraOffset.y > TILESIZE * m_nTileYLen - m_CameraRectSize.cy) m_CameraOffset.y = (float)(TILESIZE * m_nTileYLen - m_CameraRectSize.cy);
}

void Scene::Update(float fTimeElapsed)
{
	// 몬스터 업데이트(대충 몬스터 이동 이라는 뜻)
	for (auto const& d : m_vMonster) d->Update(fTimeElapsed);

	// 대충 롤러코스터 움직이라는 뜻
	for (auto& d : m_vRollerCoaster) d->Update(fTimeElapsed);

	Collision();
	
}

void Scene::Collision()
{
	if (m_nSceneNum == END_SCENE) return;

	RECT camRect = { (LONG)m_CameraOffset.x, (LONG)m_CameraOffset.y, (LONG)m_CameraOffset.x + m_CameraRectSize.cx, (LONG)m_CameraOffset.y + m_CameraRectSize.cy };
	// 플레이어와 몬스터 충돌 확인
	bool bCollide = false;
	for (auto const dPlayer : m_vPlayer) {
		for (auto const dMonster : m_vMonster) {
			FRECT player = dPlayer->GetPosition();
			FRECT monster = dMonster->GetPosition();

			// Screen culling
			if (monster.IntersectRect(camRect) && player.IntersectRect(monster)) {
				ResetPlayerPos();
				return;
			}
		}
	}

	// 플레이어와 타일맵 충돌 확인
	for (auto& dPlayer : m_vPlayer) {
		FRECT playerPos = dPlayer->GetPosition();

		POINT tLeft = { (LONG)floor((playerPos.left) / 40.0f), (LONG)floor((playerPos.bottom) / 40.0f) };
		POINT tRight = { (LONG)floor((playerPos.right) / 40.0f), (LONG)floor((playerPos.bottom) / 40.0f) };

		// 바닥
		int LB = tLeft.y * m_nTileXLen + tLeft.x;
		int RB = tRight.y * m_nTileXLen + tRight.x;
		TILE_DATA leftBottom = (0 <= LB && LB < m_vTiles.size()) ? m_vTiles[LB]->GetTile() : TILE_DATA::TD_NON;
		TILE_DATA rightBottom = (0 <= RB && RB < m_vTiles.size()) ? m_vTiles[RB]->GetTile() : TILE_DATA::TD_NON;

		if (leftBottom == TILE_DATA::TD_BLOCK || rightBottom == TILE_DATA::TD_BLOCK ||
			leftBottom == TILE_DATA::TD_FLOOR || rightBottom == TILE_DATA::TD_FLOOR) {
			dPlayer->Move(0, 40 * tLeft.y - playerPos.bottom);
			dPlayer->SetFallingFalse();

		}
		else if (leftBottom == TILE_DATA::TD_SPIKE || rightBottom == TILE_DATA::TD_SPIKE) {
			ResetPlayerPos();
			return;
		}
		else if (leftBottom == TILE_DATA::TD_GOAL || rightBottom == TILE_DATA::TD_GOAL) {
			// Stage Clear
			GameManager::GetInst().ChangeScene(m_nNextSceneNum);
			return;
		}
		else dPlayer->SetFallingTrue();						// NON, go fall

		// 벽?
		LB = tLeft.y * m_nTileXLen + tLeft.x - m_nTileXLen;
		RB = tRight.y * m_nTileXLen + tRight.x - m_nTileXLen;

		if (LB < 0 || RB < 0) {
			dPlayer->SetFallingTrue();
			break;
		}
		//else if(LB > )

		if (m_vTiles[LB]->GetTile() == TILE_DATA::TD_BLOCK ||
			m_vTiles[LB]->GetTile() == TILE_DATA::TD_FLOOR ||
			m_vTiles[RB]->GetTile() == TILE_DATA::TD_BLOCK ||
			m_vTiles[RB]->GetTile() == TILE_DATA::TD_FLOOR)
			dPlayer->GoBackX();
		else if (((0 <= LB && LB < m_vTiles.size()) &&
			(0 <= RB && RB < m_vTiles.size())) &&
			m_vTiles[LB]->GetTile() == TILE_DATA::TD_GOAL ||
			m_vTiles[RB]->GetTile() == TILE_DATA::TD_GOAL) {
			ResetPlayerPos();
			return;
		}

	}

	// 플레이어와 장애물 충돌 확인		(step, rollercoaster, button)
	for (auto& dPlayer : m_vPlayer) {
		// step
		FRECT playerPos = dPlayer->GetPosition();
		for (auto const dStep : m_vSteps) {
			FRECT stepPosition = dStep->GetPosition();

			// screen culling
			if (!stepPosition.IntersectRect(camRect) || !(dStep->IsAlive() + dStep->GetCount())) continue;

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
				//printf("%f\n", playerPos.top - dStep->GetPosition().bottom);
			}
		}

		// rollercoaster
		for (auto const dRCoaster : m_vRollerCoaster) {
			FRECT rcPosition = dRCoaster->GetPosition();

			if (!rcPosition.IntersectRect(camRect)) continue;

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
				//printf("%f\n", playerPos.top - rcPosition.bottom);
			}
		}
	}

	// 플레이어와 플레이어 충돌 확인
	Player* p0 = m_vPlayer.front();
	Player* p1 = m_vPlayer.back();
	FRECT p0Pos = p0->GetPosition();
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


	// button
	// turn off everything
	for (auto& d : m_vRollerCoaster) d->DeActive();
	for (auto& d : m_vSteps) if (!d->IsAlive()) d->DeActive();
	for (auto& dButton : m_vButton) {

		for (auto& dPlayer : m_vPlayer) {
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
}

void Scene::Render(HDC hdc)
{
	hdc = GetDC(Core::GetInst().GethWnd());
	HDC memdc = CreateCompatibleDC(hdc);
	if (m_hDoubleBufferBitmap == NULL) m_hDoubleBufferBitmap = CreateCompatibleBitmap(hdc, m_nTileXLen * TILESIZE, m_nTileYLen * TILESIZE);

	SIZE wndSize = Core::GetInst().GetSize();
	SIZE imgSize = { m_imgBackGround.GetWidth(), m_imgBackGround.GetHeight() };

	SelectObject(memdc, m_hDoubleBufferBitmap);

	// draw bg
	RECT imgRect = { 
	(LONG)((m_CameraOffset.x * wndSize.cx) / (m_nTileXLen * TILESIZE)),
	(LONG)((m_CameraOffset.y * wndSize.cy) / (m_nTileYLen * TILESIZE)),
	(LONG)((m_CameraOffset.x + imgSize.cx) * wndSize.cx) / (m_nTileXLen * TILESIZE),
	(LONG)((m_CameraOffset.y + imgSize.cy) * wndSize.cy) / (m_nTileYLen * TILESIZE) };
					 
	//printf("%4d %4d %4d %4d\n", imgRect.left, imgRect.top, imgRect.right, imgRect.bottom);

	m_imgBackGround.BitBlt(memdc, { (int)(m_CameraOffset.x), (int)(m_CameraOffset.y),
		(int)(m_CameraOffset.x) + wndSize.cx, (int)(m_CameraOffset.y) + wndSize.cy },
		{ imgRect.left, imgRect.top }, SRCCOPY);

	//if End Scene return
	if (m_nSceneNum == END_SCENE) {
		BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memdc, (int)m_CameraOffset.x, (int)m_CameraOffset.y, SRCCOPY);

		//DeleteObject(m_hDoubleBufferBitmap);
		DeleteDC(memdc);
		ReleaseDC(Core::GetInst().GethWnd(), hdc);
		return;
	}

	// draw Tiles
	int x = (int)m_CameraOffset.x / 40 - 1;
	int y = (int)m_CameraOffset.y / 40 - 1;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	for (int i = y; i < y + 20; ++i) {
		for (int j = x; j < x + 34; ++j) {

			if (m_vTiles[m_nTileXLen * i + j]->GetTile() == TILE_DATA::TD_BLOCK) {
				RECT temp = { j * TILESIZE, i * TILESIZE, (j + 1) * TILESIZE, (i + 1) * TILESIZE };

				m_imgTile.TransparentBlt(memdc, j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE,
					TILE_IMAGE_STRIDE * TILE_IMAGE_BLOCK, 0, TILE_IMAGE_SIZE, TILE_IMAGE_SIZE, IMAGE_TRANSPARENT);

				//FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			} else if (m_vTiles[m_nTileXLen * i + j]->GetTile() == TILE_DATA::TD_FLOOR) {
				RECT temp = { j * TILESIZE, i * TILESIZE, (j + 1) * TILESIZE, (i + 1) * TILESIZE };

				m_imgTile.TransparentBlt(memdc, j * TILESIZE, i * TILESIZE, TILESIZE, TILESIZE,
					TILE_IMAGE_STRIDE * TILE_IMAGE_FLOOR, 0, TILE_IMAGE_SIZE, TILE_IMAGE_SIZE, IMAGE_TRANSPARENT);

				//FrameRect(memdc, &temp, (HBRUSH)GetStockObject(BLACK_BRUSH));
			}
		}
	}

	// draw step, rollercoaster
	for (auto const d : m_vSteps) d->Render(memdc);
	for (auto const d : m_vRollerCoaster) d->Render(memdc);

	// draw button
	for (auto const d : m_vButton) d->Render(memdc);

	// draw monster
	for (auto const d : m_vMonster) d->Render(memdc);
	// draw player
	for (auto const d : m_vPlayer) d->Render(memdc);


	BitBlt(hdc, 0, 0, wndSize.cx, wndSize.cy, memdc, (int)m_CameraOffset.x, (int)m_CameraOffset.y, SRCCOPY);

	//DeleteObject(m_hDoubleBufferBitmap);
	DeleteDC(memdc);
	ReleaseDC(Core::GetInst().GethWnd(), hdc);
}