#include "pch.h"
#include "Core.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	if (!Core::GetInst().Init(hInstance)) 
	{
		Core::GetInst().OnDestroy();
		return -1;
	}

	auto retval = Core::GetInst().Run();

	Core::GetInst().OnDestroy();

	return retval;
}