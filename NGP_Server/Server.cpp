#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> 
#include <Windows.h>
#include <unordered_map>
#include "SOCKETINFO.h"

#include "../protocol/protocol.h"

#pragma comment(lib, "WS2_32.lib")

#define SERVERPORT 9000
#define MAX_PLAYERS 3

using namespace std;

// SOCKETINFO�� ���� ���� ����
unordered_map<int, SOCKETINFO> g_clients;

// RecvThread�� �����ϴ� �Լ�
HANDLE rthread[MAX_PLAYERS];


// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ���� �Լ� ���� ���
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[����] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

DWORD WINAPI ServerRecvThread(LPVOID arg);

int main(int argc, char* argv[]) {

	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	
	// listen()
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");


	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	int currentPlayerNum = 0;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// if player < max player
		if (currentPlayerNum < MAX_PLAYERS) {
			int id = currentPlayerNum++;
			//pair<int, SOCKETINFO> client{ id, id, client_sock };

			g_clients.emplace(std::piecewise_construct,
				std::forward_as_tuple(id),
				std::forward_as_tuple(id, client_sock));

			rthread[id] = CreateThread(NULL, 0, ServerRecvThread, (LPVOID)&id, 0, NULL);
		}
		

	}





}


DWORD WINAPI ServerRecvThread(LPVOID arg)
{
	int id = *((int*)arg);
	cout << "Ŭ���̾�Ʈ ����, id: " << id << endl;

	while (true) {
		if (!g_clients[id].ServerDoRecv())
			break;
	}

	// ���� ����
	g_clients.erase(id);

	return 0;
}