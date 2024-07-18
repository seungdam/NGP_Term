#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#pragma comment(lib, "Debug\\CoreEngine.lib")
#include <crtdbg.h>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#else
#pragma comment(lib, "Release\\CoreEngine.lib")
#endif 

#include "CorePch.h";

using namespace std;