#include "Game.h"
#include "SOCKETINFO.h"
#include "../protocol/protocol.h"
#include "Scene/Scene.h"


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

#define SERVERPORT 9000

using namespace std;

// SOCKETINFO�� ���� ���� ����
unordered_map<int, SOCKETINFO> g_clients;

// RecvThread�� �����ϴ� �Լ�
HANDLE rthread[MAX_PLAYERS];
HANDLE sthread;

DWORD WINAPI ServerRecvThread(LPVOID arg);
DWORD WINAPI ServerSendThread(LPVOID arg);
int currentPlayerNum = 0;
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


	while (1) {

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_quit("accept()");
		}
		if (currentPlayerNum < MAX_PLAYERS) {
			// id�� 0���� �ο��Ѵ�
			int id = currentPlayerNum++;
			g_clients.try_emplace(id, id, client_sock);
			rthread[id] = CreateThread(NULL, 0, ServerRecvThread, (LPVOID)&id, 0, NULL);
			if (MAX_PLAYERS == currentPlayerNum) {
				// send ������ ����, 
				// ����) �α��� ��Ŷ �۽� �κ� SendThread�� �̵� Ȯ�� ������ �� �ּ� ���� ��
				sthread = CreateThread(NULL, 0, ServerSendThread, nullptr, 0, NULL);
			}
		}
	}

	closesocket(sock);
	WSACleanup();
}

void Disconnect(int id) {

	g_clients[id].m_PlayersInfo[id].p_pos[0].x = -1;
	g_clients[id].m_PlayersInfo[id].p_pos[0].y = -1;
	g_clients[id].m_PlayersInfo[id].p_pos[1].x = -1;
	g_clients[id].m_PlayersInfo[id].p_pos[1].y = -1;
	g_clients.erase(id);
}

DWORD WINAPI ServerRecvThread(LPVOID arg)
{
	int id = *((int*)arg);
	bool retval;
	cout << "Ŭ���̾�Ʈ ����, id: " << id << endl;

	while (true) {
		retval = g_clients[id].ServerDoRecv();
		if (!retval) {
			err_display("recv()");
			break;
		}
	}

	return 0;
}

DWORD WINAPI ServerSendThread(LPVOID arg)
{
	// scene ����
	const int startSceneNum = 1;
	Scene* pScene = new Scene(startSceneNum);

	SOCKETINFO::SetScene(pScene);

	pScene->InsertPlayers(MAX_PLAYERS);
	pScene->Init();

	// ������ �÷��̾�鿡�� �α���
	for (auto& i : g_clients) {
		i.second.ServerDoSendLoginPacket(true);
	}

	// Ÿ�̸�
	LARGE_INTEGER sec;
	LARGE_INTEGER time;
	
	QueryPerformanceFrequency(&sec);
	QueryPerformanceCounter(&time);

	float fTimeElapsed = 0;

	while (true) {
		// get elapsed time
		LARGE_INTEGER tTime;
		QueryPerformanceCounter(&tTime);
		fTimeElapsed = (tTime.QuadPart - time.QuadPart) / (float)sec.QuadPart;
		time = tTime;

		// update
		pScene->Update(fTimeElapsed);
		SOCKETINFO::UpdatePlayerInfo();

		int retval = 0;
		// send to player
		// ���� �÷��̾���� �̵��� �ִٸ� ����, ������ �׽�Ʈ �ϱ� ���� true�� �����Ѵ�. ���빰�� �� ��
		if (pScene->IsPlayersUpdated()) {
			for (auto& i : g_clients) {
				 retval = i.second.ServerDoSend((char)(SERVER_PACKET_INFO::PLAYER_MOVE));
				 if (retval == SOCKET_ERROR) {
					 Disconnect(i.first);
					 break;
				 }
			}
		}

	}

	if (pScene) delete pScene;

	return 0;
}