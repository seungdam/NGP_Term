#include "Game.h"
#include "SOCKETINFO.h"
#include "../protocol/protocol.h"
#include "Scene/Scene.h"
#include "SceneManager.h"


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

using namespace std;

// SOCKETINFO를 담을 전역 변수
unordered_map<int, SOCKETINFO> g_clients;

// RecvThread를 관리하는 함수
HANDLE rthread[MAX_PLAYERS];
HANDLE sthread;

DWORD WINAPI ServerRecvThread(LPVOID arg);
DWORD WINAPI ServerSendThread(LPVOID arg);
int currentPlayerNum = 0;
bool g_bGameLoop = true;

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


	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_quit("accept()");
		}
		if (currentPlayerNum < MAX_PLAYERS) {
			// id는 0부터 부여한다
			int id = currentPlayerNum++;
			g_clients.try_emplace(id, id, client_sock);
			rthread[id] = CreateThread(NULL, 0, ServerRecvThread, (LPVOID)&id, 0, NULL);
			if (rthread[id] != NULL) CloseHandle(rthread[id]);
			if (MAX_PLAYERS == currentPlayerNum) {
				// send 스레드 생성, 
				// 정보) 로그인 패킷 송신 부분 SendThread로 이동 확인 했으면 이 주석 지울 것
				sthread = CreateThread(NULL, 0, ServerSendThread, nullptr, 0, NULL);
				if (sthread != NULL) CloseHandle(sthread);
			}
		}
		else { // 정원 외 클라이언트 접속 시 접속 끊기
			S2C_LOGIN_PACKET p;
			p.b_success = false;
			p.c_id = -1;
			p.type = (char)SERVER_PACKET_INFO::LOGIN();
			retval = send(client_sock, (char*)&p, sizeof(S2C_LOGIN_PACKET), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
			closesocket(client_sock);
		}
	}

	closesocket(sock);
	WSACleanup();
}

void Disconnect(int id) {

	std::cout << id << " 클라이언트 종료" << std::endl;
	g_clients.erase(id);
	for (auto& i : g_clients) {
		std::cout << "남은 클라 ID: " << i.first << std::endl;
	}
}

DWORD WINAPI ServerRecvThread(LPVOID arg)
{
	int id = *((int*)arg);
	int retval;
	cout << "클라이언트 접속, id: " << id << endl;

	while (g_bGameLoop) {
		retval = g_clients[id].ServerDoRecv();
		if (retval == SOCKET_ERROR || retval == 0) {
		//if (false) {
			err_display("recv()");
			break;
		}
	}
	Disconnect(id);
	return 0;
}

DWORD WINAPI ServerSendThread(LPVOID arg)
{
	// scene 생성
	const int startSceneNum = 1;
	int curScene = startSceneNum;
	SceneManager* pManager = new SceneManager;

	pManager->ChangeScene(curScene++);
	SOCKETINFO::SetScene(pManager->GetScene());

	// 접속한 플레이어들에게 로그인
	for (auto& i : g_clients) {
		i.second.ServerDoSendLoginPacket(true);
	}

	// 타이머
	LARGE_INTEGER sec;
	LARGE_INTEGER time;
	
	QueryPerformanceFrequency(&sec);
	QueryPerformanceCounter(&time);

	float fTimeElapsed = 0.0f;
	float fSendElapsed = 0.0f;

	while (true) {
		// get elapsed time
		LARGE_INTEGER tTime;
		QueryPerformanceCounter(&tTime);
		fTimeElapsed = (tTime.QuadPart - time.QuadPart) / (float)sec.QuadPart;
		time = tTime;

		fSendElapsed += fTimeElapsed;

		// update
		int goal_clients_id = pManager->Update(fTimeElapsed);
		if (goal_clients_id >= 0) {
			g_clients[goal_clients_id].AddScore();

			//for (auto& i : g_clients) {
			for (auto iter = g_clients.begin(); iter != g_clients.end(); ) {
				int retval = iter->second.ServerDoSend((char)(SERVER_PACKET_INFO::SCENE_CHANGE), curScene);
				if (retval == SOCKET_ERROR) {
					std::cout << iter->first << " 클라이언트 종료" << std::endl;
					iter = g_clients.erase(iter);
				}
				else ++iter;
				
			}
			// 씬 변경
			pManager->ChangeScene(curScene);
			SOCKETINFO::SetScene(pManager->GetScene());

			// 게임 끝이라면?
			if (curScene == 4) {
				int max_score_id = 0;
				int score = 0;
				for (auto& i : g_clients) {
					if (i.second.GetScore() > score) max_score_id = i.first;
					score = i.second.GetScore();
				}

				for (auto iter = g_clients.begin(); iter != g_clients.end(); ) {
					int retval = iter->second.ServerDoSend((char)(SERVER_PACKET_INFO::GAME_END), max_score_id);
					if (retval == SOCKET_ERROR) {
						std::cout << iter->first << " 클라이언트 종료" << std::endl;
						iter = g_clients.erase(iter);
					}
					else ++iter;
				}
				g_bGameLoop = false;
				break;
			}
			curScene++;
		}
		SOCKETINFO::UpdatePlayerInfo();

		int retval = 0;
		// send to player
		if (SOCKETINFO::IsUpdated()) {
			fSendElapsed = 0.0f;
			for (auto& i : g_clients) {
				 retval = i.second.ServerDoSend((char)(SERVER_PACKET_INFO::PLAYER_MOVE));
				 if (retval == SOCKET_ERROR) {
					 Disconnect(i.first);
					 break;
				 }
			}
			SOCKETINFO::UpdateBeforeInfo();
		}

	}

	//g_clients.clear();

	if (pManager) delete pManager;

	return 0;
}