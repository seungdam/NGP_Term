#pragma once
#include "../Game.h"
#include "../Scene/Scene.h"


// Singleton class, manages scene
class GameManager {
private:
	GameManager() {}
	~GameManager();

	int m_iCurSceneNum = 0;
	class Scene *m_pScene = nullptr;

public:
	static GameManager& GetInst() {
		static GameManager inst;
		return inst;
	}

	void Init();
	void OnDestroy();

	void ChangeScene(int nSceneNum);

	void SynchronizeObjects();

public:
	void Input(float fTimeElapsed);
	void Update(float fTimeElapsed);
	void Render(HDC hdc);
};

