#include "pch.h"
#include "Core.h"
#include <sstream>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	__int32 argc = 0;
	string cmd_line(lpszCmdParam);
	istringstream iss(cmd_line);
	string arg;
	vector<string> args;
	while (iss >> arg)
	{
		args.push_back(arg);
	}


	if (!Core::GetInst().Init(hInstance, args[0].c_str()))
	{
		Core::GetInst().OnDestroy();
		return -1;
	}

	auto retval = Core::GetInst().Run();

	Core::GetInst().OnDestroy();

	return retval;
}