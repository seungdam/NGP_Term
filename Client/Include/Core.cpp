#include "pch.h"
#include "Core.h"
#include "GameManager/GameManager.h"
#include "Session.h"
#include <string>

__int32 __stdcall Core::RecvWorker()
{

	while (Core::GetInst().GetGameLoop()) 
	{
		Core::GetInst().GetSession()->DoRecv();

		if (Core::GetInst().GetSession()->m_most_high_score_id >= 0) 
		{
			wstring str{ L"Winner Clients is~" };
			str += to_wstring(Core::GetInst().GetSession()->m_most_high_score_id);
			MessageBox(NULL, str.c_str(), L"WINNER", MB_OK);
			Core::GetInst().SetGameLoopFalse();
			break;
		}
	}

	return 0;
}

Core::Core()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), OUT & wsa) != 0)
	{
		cout << "WSA START ERROR!!" << endl;
	}

#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG
}

Core::~Core()
{
	WSACleanup();
}

void Core::OnDestroy()
{
	if (m_Session) 
	{
		delete m_Session;
	}
}

bool Core::Init(HINSTANCE hInst, const char* ipAddr)
{
	// window init
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
	m_hWnd = CreateWindow(L"Class Name", 
						  L"Title", 
		                  WS_OVERLAPPEDWINDOW, 
		                  CW_USEDEFAULT, 
		                  0,
		                  rt.right - rt.left, rt.bottom - rt.top, 
						  NULL, NULL, m_hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	
	m_Session = new Session();
	if (ipAddr == nullptr)
	{
		return false;
	}
	if (m_Session->DoConnect(ipAddr))
	{
		return false;
	}

	return true;
}


int Core::Run()
{
	MSG Message;
	HDC hdc = NULL;

	auto retval = m_Session->DoRecv();
	
	if(!retval) 
	{
		return -1;
	}


#ifdef DEBUG
	wstring s = to_wstring(m_Session->GetID());
	SetWindowText(m_hWnd, s.c_str());
#endif // DEBUG

	Core::SetThreadFunction<__int32>(Core::RecvWorker);
	m_Session->SetScene(GameManager::GetInst().GetScene());
	

	QueryPerformanceFrequency(&m_Sec);
	QueryPerformanceCounter(&m_Time);
	GameManager::GetInst().Init();
	GameManager::GetInst().Render(hdc);
	GameManager::GetInst().ChangeScene(1);

	while (GetGameLoop()) 
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else 
		{
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
			//swprintf(szTitle, L"FPS : %.1f", 1 / m_fTimeElapsed);
			SetConsoleTitle(szTitle);
#endif 
		}
	}

	
	GameManager::GetInst().OnDestroy();
	m_Session->Disconnect();
	return 0;
}


LRESULT Core::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = NULL, memdc = NULL;

	switch (Msg) 
	{
	case WM_CREATE:
	
		break;

	case WM_PAINT:
		break;

	case WM_KEYDOWN:
		if (wParam == 'g')
		{
			Core::GetInst().SetGridShow();
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		Core::GetInst().SetGameLoopFalse();
		break;

	case WM_TIMER:
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
