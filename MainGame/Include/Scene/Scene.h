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
	vector<class Player*> m_vMyPlayer;
	vector<class Player*> m_vOtherPlayers;
	vector<class Monster*> m_vMonster;
	vector<class RollerCoaster*> m_vRollerCoaster;
	// 내용물 delete 금지
	vector<class Player*> m_vPlayerVectors;

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
	FRECT m_p0StartPos = { 0.0f, 0.0f, 25.0f, 50.0f };
	FRECT m_p1StartPos = { 0.0f, 0.0f, 25.0f, 50.0f };

private:
	HBITMAP m_hDoubleBufferBitmap = NULL;

private:
	bool LoadMapFromFile(FILE* fp);
	void InsertNewPlayer(int myId);

	void Collision();
	void ResetPlayerPos(int index);

public:
	void SetPlayerData(S2C_PLAYER_MOVE_PACKET* pData);


	int GetSceneNum() const { return m_nSceneNum; }

public:
	void Init();

	void Input(float fTimeElapsed);
	void Update(float fTimeElapsed);
	void Render(HDC hdc);
};

