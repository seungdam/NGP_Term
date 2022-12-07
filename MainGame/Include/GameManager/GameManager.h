#pragma once
#include "../Game.h"
#include "../Scene/Scene.h"


// Singleton class, manages scene
class GameManager {
private:
	GameManager() {}
	~GameManager();

	int m_iCurSceneNum = 0;

	bool m_bChange = false;
	int m_iNextScene = 1;

	class Scene *m_pScene = nullptr;

public:
	static GameManager& GetInst() {
		static GameManager inst;
		return inst;
	}

	
	void Init();
	void OnDestroy();

	void OnSceneChangePacket(int nextScene) { m_bChange = true, m_iNextScene = nextScene; }
	void ChangeScene(int nSceneNum);

	void SynchronizeObjects();

	Scene* GetScene() { return m_pScene; }

public:
	void Input(float fTimeElapsed);
	void Update(float fTimeElapsed);
	void Render(HDC hdc);
};

