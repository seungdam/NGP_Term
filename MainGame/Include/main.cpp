#include "Core.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	if (!Core::GetInst().Init(hInstance)) {
		Core::GetInst().OnDestroy();
		return -1;
	}

	RECT temp;

	int retValue = Core::GetInst().Run();

	Core::GetInst().OnDestroy();

	return retValue;
}