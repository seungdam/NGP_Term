#pragma once
#include <thread>

class Session;

class Core 
{
	Core();
	~Core();

public:
	static Core& GetInst() 
	{
		static Core inst;
		return inst;
	}

	void OnDestroy();

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hdc;
	SIZE		m_tWndSize;

private:
	// timer
	LARGE_INTEGER m_Sec;
	LARGE_INTEGER m_Time;
	float m_fTimeElapsed;
	float m_fTimeCnt = 0;

	bool m_bShowGrid = false;
	bool m_bGameLoop = true;



private:
	Session* m_Session = nullptr;
	thread m_recv_thread;
public:
	Session* GetSession() { return m_Session; }

public:
	SIZE GetSize() const { return m_tWndSize; }
	HWND GethWnd() const { return m_hWnd; }
	bool GetGridShow() const { return m_bShowGrid; }
	bool GetGameLoop() const { return m_bGameLoop; }

	void SetGridShow()		{ m_bShowGrid = !m_bShowGrid; }
	void SetGameLoopFalse() { m_bGameLoop = false; }
	template<typename T>
	void SetThreadFunction(T (*func)()) { m_recv_thread = std::thread(func); }
public:
	bool Init(HINSTANCE hInst, const char* ipAddr);
	int Run();

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	static __int32 RecvWorker();
};