#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> 
#include <Windows.h>
#include <unordered_map>
#include "SOCKETINFO.h"

#include "../protocol/protocol.h"
#pragma comment(lib, "WS2_32.lib")

// 소켓 함수 오류 출력 후 종료
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
// 소켓 함수 오류 출력
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

#define SERVERPORT 9000
#define MAX_PLAYERS 1

using namespace std;

// SOCKETINFO를 담을 전역 변수
unordered_map<int, SOCKETINFO> g_clients;

// RecvThread를 관리하는 함수
HANDLE rthread[MAX_PLAYERS];


DWORD WINAPI ServerRecvThread(LPVOID arg);

int main(int argc, char* argv[]) {

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
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

	int acceptClientNum = 0;

	int currentPlayerNum = 0;

	int id = currentPlayerNum;

	while (1) {

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_quit("accept()");
		}
		if (currentPlayerNum < MAX_PLAYERS) {
			currentPlayerNum++;
			g_clients.try_emplace(id, id, client_sock);
			rthread[id] = CreateThread(NULL, 0, ServerRecvThread, (LPVOID)&id, 0, NULL);
			if (MAX_PLAYERS == currentPlayerNum) {
				for (auto& i : g_clients) {
					i.second.ServerDoSendLoginPacket(true);
				}
			}
		}

		// 로그인 패킷
		/*auto iter = g_clients.find(clientaddr.sin_port);
		if (iter == g_clients.end()) {
			S2C_LOGIN_PACKET logpacket;
			logpacket.c_id = id;
			g_clients.emplace(std::piecewise_construct, std::forward_as_tuple(clientaddr.sin_port),
				std::forward_as_tuple(logpacket.c_id));

			retval = send(client_sock, (char*)&logpacket.c_id, sizeof(logpacket.c_id), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
		}*/


		// if player < max player
		
	}
	closesocket(sock);
	WSACleanup();
}


DWORD WINAPI ServerRecvThread(LPVOID arg)
{
	int id = *((int*)arg);
	bool retval;
	cout << "클라이언트 접속, id: " << id << endl;

	while (true) {
		retval = g_clients[id].ServerDoRecv();
		if (!retval) {
			err_display("recv()");
			break;
		}
	}

	// 연결 해제
	g_clients.erase(id);

	return 0;
}