#pragma once

class SceneManager
{
	Scene* m_pScene = nullptr;

public:
	SceneManager();
	~SceneManager();

	void ChangeScene(int nextScene);

	int Update(float fETime);

	Scene* GetScene() { return m_pScene; }
};

