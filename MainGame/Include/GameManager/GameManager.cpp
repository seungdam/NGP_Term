#include "../Core.h"

#include "GameManager.h"

GameManager::~GameManager()
{
}

void GameManager::ChangeScene(int nSceneNum)
{
	m_iCurSceneNum = nSceneNum;
	SAFE_DELETE(m_pScene);
	m_pScene = new Scene(m_iCurSceneNum);
	
	m_bChange = false;

	m_pScene->Init();

	Core::GetInst().GetNetworkManager()->SetScene(m_pScene);

}

void GameManager::SynchronizeObjects()
{
	// do synchronize
}

void GameManager::OnDestroy()
{
	if (m_pScene) delete m_pScene;
}

void GameManager::Init()
{
	// test scene == -1
	m_iCurSceneNum = LOADING_SCENE;
	m_pScene = new Scene(m_iCurSceneNum);

	m_pScene->Init();
}

void GameManager::Input(float fTimeElapsed)
{
	if (m_bChange) ChangeScene(m_iNextScene);

	if (m_pScene) m_pScene->Input(fTimeElapsed);
}

void GameManager::Update(float fTimeElapsed)
{
	if (m_bChange) ChangeScene(m_iNextScene);

	if (m_pScene && m_pScene->GetSceneNum() != 4) m_pScene->Update(fTimeElapsed);
}

void GameManager::Render(HDC hdc)
{
	if (m_bChange) ChangeScene(m_iNextScene);

	if (m_pScene) m_pScene->Render(hdc);
}
