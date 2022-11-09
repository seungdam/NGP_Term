#pragma once

#include "../Game.h"
#include "../Tile.h"
#include "../Object/Player.h"
#include "../Object/Step.h"
#include "../Object/Monster.h"
#include "../Object/RollerCoaster.h"
#include "../Object/Button.h"

class Scene {
public:
	Scene() = delete;
	Scene(int nSceneNum);
	~Scene();

private:
	// current and next scene num
	int m_nSceneNum;
	int m_nNextSceneNum;

	// background, tile image
	CImage m_imgBackGround;
	CImage m_imgTile;

	// movingObjs
	vector<class Player* > m_vPlayer;
	vector<class Monster*> m_vMonster;
	vector<class RollerCoaster*> m_vRollerCoaster;

	// static Objs
	vector<class Step*> m_vSteps;
	vector<class Button*> m_vButton;

	// tiles;
	vector<class Tile*> m_vTiles;
	int m_nTileXLen;
	int m_nTileYLen;

	// camera
	FPOINT m_CameraOffset = { 0.0f,0.0f };
	const SIZE m_CameraRectSize = { 1280,800 };
	float dx;

private:
	// player start position
	RECT m_p0StartPos;
	RECT m_p1StartPos;

private:
	HBITMAP m_hDoubleBufferBitmap = NULL;

private:
	bool LoadMapFromFile(FILE* fp);

	void Collision();
	void ResetPlayerPos();

public:
	int GetSceneNum() const { return m_nSceneNum; }

public:
	void Init();

	void Input(float fTimeElapsed);
	void Update(float fTimeElapsed);
	void Render(HDC hdc);
};

