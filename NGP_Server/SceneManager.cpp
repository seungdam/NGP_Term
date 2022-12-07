#include "Game.h"
#include "Scene/Scene.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	if (m_pScene) delete m_pScene;
}

void SceneManager::ChangeScene(int nextScene)
{
	if (m_pScene) delete m_pScene;
	m_pScene = new Scene(nextScene);

	m_pScene->InsertPlayers(MAX_PLAYERS);
	m_pScene->Init();

}

int SceneManager::Update(float fEtime)
{
	if (m_pScene) 
		return m_pScene->Update(fEtime);

	return 0;
	
}
