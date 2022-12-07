#pragma once
#include <vector>
#include <Windows.h>
#include "../Object/Player.h"
#include "../Object/Step.h"
#include "../Object/Monster.h"
#include "../Object/RollerCoaster.h"
#include "../Object/Button.h"
#include "../Object/Tile.h"


class Scene {
public:
	Scene() = delete;
	Scene(int nSceneNum);
	~Scene();

private:
	// current and next scene num
	int m_nSceneNum;
	int m_nNextSceneNum;

	// movingObjs
	std::vector<class Player*> m_vPlayers;
	std::vector<class Monster*> m_vMonster;
	std::vector<class RollerCoaster*> m_vRollerCoaster;

	// static Objs
	std::vector<class Step*> m_vSteps;
	std::vector<class Button*> m_vButton;

	// tiles;
	std::vector<class Tile*> m_vTiles;
	int m_nTileXLen;
	int m_nTileYLen;

private:
	// player start position
	FPOINT m_p0StartPos;
	FPOINT m_p1StartPos;

private:
	bool LoadMapFromFile(FILE* fp);

	int Collision();
	void ResetPlayerPos(int index);

public:
	int GetSceneNum() const { return m_nSceneNum; }

public:
	// 플레이어를 playerSize만큼 생성해서 m_vPlayers에 넣는다.
	void InsertPlayers(int playerSize);

	void SetPlayerInput(int index, unsigned char move);
	FPOINT GetPlayerPosition(int index);
	unsigned char GetPlayerInput(int index);

	bool IsPlayersUpdated() const;

public:
	void Init();

	void Input(float fTimeElapsed);
	int Update(float fTimeElapsed);
};

