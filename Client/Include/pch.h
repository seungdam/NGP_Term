#pragma once
#pragma warning (disable : 4996)

#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <vector>
#include <list>
#include <string>
#include <atlimage.h>
#include <iostream>
#include <time.h>

#pragma comment(lib, "ws2_32")



#ifdef _DEBUG
#include <crtdbg.h>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif 


#include "Type.h"
#include "Macro.h"
#include "../../Protocol/Packet.h"



using namespace std;


#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"

#define MAX_MOVING_STEPS 30

#define TILE_IMAGE_SIZE 70
#define TILE_IMAGE_STRIDE (TILE_IMAGE_SIZE + 2)
#define TILE_IMAGE_FLOOR 4
#define TILE_IMAGE_BLOCK 5
#define IMAGE_TRANSPARENT RGB(80, 40, 0)

#define TILESIZE 40
#define END_SCENE 4
#define LOADING_SCENE 998
#define NEXT_SCENE 997
#define WINDOW_SIZE_WIDTH 1280
#define WINDOW_SIZE_HEIGHT 720


