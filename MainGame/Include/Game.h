#pragma once
#pragma warning (disable : 4996)
#pragma comment(lib, "ws2_32")

#include <winsock2.h>

#include <Windows.h>
#include <vector>
#include <list>
#include <string>
#include <atlimage.h>

#include <fmod.h>

#ifdef _DEBUG
#include <crtdbg.h>
#include <iostream>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif // _DEBUG


using namespace std;

#include "Type.h"
#include "Macro.h"
#include "../../protocol/protocol.h"
#include "Networker.h"

#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
//#define SERVERIP "192.168.219.108"

#define MAX_MOVING_STEPS 30

#define TILE_IMAGE_SIZE 70
#define TILE_IMAGE_STRIDE (TILE_IMAGE_SIZE + 2)
#define TILE_IMAGE_FLOOR 4
#define TILE_IMAGE_BLOCK 5
#define IMAGE_TRANSPARENT RGB(80, 40, 0)

#define TILESIZE 40
#define END_SCENE 999
#define LOADING_SCENE 998
#define NEXT_SCENE 997
#define WINDOW_SIZE_WIDTH 1280
#define WINDOW_SIZE_HEIGHT 720


template <typename T>
void ReleaseAndCleanVector(T& p) {
	typename T::iterator iter;
	for (iter = p.begin(); iter != p.end(); ++iter) SAFE_DELETE((*iter));

	p.clear();
}