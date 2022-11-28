#include "Core.h"
#include "GameManager/GameManager.h"
#include <time.h>

DWORD WINAPI Core::Recv_Thread(LPVOID arg) {

	while (Core::GetInst().GetGameLoop()) {
		Core::GetInst().GetNetworkManager()->ClientDoRecv();
	}

	return 0;
}

Core::Core()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(155);
}

Core::~Core()
{
}

void Core::OnDestroy()
{
	if (m_pSoundSystem) {
		FMOD_System_Close(m_pSoundSystem);
		FMOD_System_Release(m_pSoundSystem);
	}

	if (m_NetworkManager) {
		delete m_NetworkManager;
	}
}

bool Core::Init(HINSTANCE hInst)
{
	// fmod system init
	//FMOD_System_Create(&m_pSoundSystem);
	//FMOD_System_Init(m_pSoundSystem, 32, FMOD_INIT_NORMAL, NULL);

	// window init
	m_NetworkManager = new Networker;
	m_hInst = hInst;

	m_tWndSize.cx = WINDOW_SIZE_WIDTH;
	m_tWndSize.cy = WINDOW_SIZE_HEIGHT;

	WNDCLASSEX WndClass;

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.lpfnWndProc = WndProc;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = m_hInst;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"Class Name";
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	RECT rt = { 0,0,WINDOW_SIZE_WIDTH,WINDOW_SIZE_HEIGHT };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, NULL);
	m_hWnd = CreateWindow(L"Class Name", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
		rt.right - rt.left, rt.bottom - rt.top, NULL, NULL, m_hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}


int Core::Run()
{
	MSG Message;

	HDC hdc = NULL;

	QueryPerformanceFrequency(&m_Sec);
	QueryPerformanceCounter(&m_Time);

	// if loggin failed
	if (!m_NetworkManager->ConnectTo(SERVERIP)) 
		return -1;

	m_NetworkManager->ClientDoRecv();

	HANDLE rThread = CreateThread(NULL, 0, Recv_Thread,(LPVOID)m_NetworkManager, 0, 0);
	if (rThread == NULL) {

	}
	GameManager::GetInst().Init();

	m_NetworkManager->SetScene(GameManager::GetInst().GetScene());

	FMOD_SOUND* pBGSound;
	FMOD_CHANNEL* pChannel;

	// bg sound
	//FMOD_System_CreateSound(m_pSoundSystem, "Sounds/bgMusic.mp3", FMOD_LOOP_NORMAL, 0, &pBGSound);
	//FMOD_System_PlaySound(m_pSoundSystem, pBGSound, NULL, 0, &pChannel);
	//FMOD_Channel_SetVolume(pChannel, 0.1f);

	while (GetGameLoop()) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			// get elapsed time
			LARGE_INTEGER tTime;
			QueryPerformanceCounter(&tTime);
			m_fTimeElapsed = (tTime.QuadPart - m_Time.QuadPart) / (float)m_Sec.QuadPart;
			m_fTimeCnt += m_fTimeElapsed;
			m_Time = tTime;

			GameManager::GetInst().Input(m_fTimeElapsed);
			GameManager::GetInst().Update(m_fTimeElapsed);
			GameManager::GetInst().Render(hdc);

#ifdef DEBUG
			TCHAR szTitle[30];
			swprintf(szTitle, L"FPS : %.1f", 1 / m_fTimeElapsed);
			SetConsoleTitle(szTitle);
#endif // DEBUG
		}
	}

	GameManager::GetInst().OnDestroy();

	//FMOD_Channel_Stop(pChannel);
	//FMOD_Sound_Release(pBGSound);
	m_NetworkManager->Disconnect();
	return 0;
}


LRESULT Core::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = NULL, memdc = NULL;

	switch (Msg) {
	case WM_CREATE:
		//GameManager::GetInst().Init();

		// Set Main Timer;
		//SetTimer(hWnd, 0, 1, NULL);
		break;

	case WM_PAINT:
		break;

	case WM_KEYDOWN:
		if (wParam == 'g') Core::GetInst().SetGridShow();
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		Core::GetInst().SetGameLoopFalse();
		//GameManager::OnDestroy();
		break;

	case WM_TIMER:
		switch (wParam) {
		case 0:
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
