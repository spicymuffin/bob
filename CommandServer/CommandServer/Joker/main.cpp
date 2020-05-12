#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h> 
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

//some constant strings to send in TCP Packets
const string vermsg = "hello";
const string hide_tosend = "hide";
const string show_tosend = "show";
const string read_tosend = "read";
const string make_error_sound_tosend = "mkerrs";
const string idle_tosend = "idle";
const string quitcl_tosend = "quitcl";
const string bruh_tosend = "bruh";
const string minw_tosend = "minw";
const string minwt_tosend = "minwt";
const string qtimer_tosend = "qtimer";

enum enteropt {
	cmd_invalid_e,
	start,
	quit,
	help,
};

enum startopt {
	cmd_invalid_s,
	makerrsound,
	read,
	show,
	hide,
	quitcl,
	bruh,
	minw,
	minwt,
};

startopt resolvestart(string input) {
	if (input == "mkerrs") return makerrsound;
	if (input == "m") return makerrsound;
	if (input == "read") return read;
	if (input == "r") return read;
	if (input == "show") return show;
	if (input == "hide") return hide;
	if (input == "quit") return quitcl;
	if (input == "bruh") return bruh;
	if (input == "b") return bruh;
	if (input == "minw") return minw;
	if (input == "w") return minw;
	if (input == "minimizewindow") return minw;
	if (input == "minwtimer") return minwt;
	if (input == "mt") return minwt;
	return cmd_invalid_s;
}

enteropt resolveenter(string input) {
	if (input == "start") return start;
	if (input == "s") return start;
	if (input == "quit") return quit;
	if (input == "help") return help;
	return cmd_invalid_e;
}

int main() {

	//=====================================

	string consoleinp;
	string toread;
	bool started = false;

	cout << "                                                  Welcome\nType help for commands list:" << endl;
	//main loop
	while (true) {
		cout << "> ";
		cin >> consoleinp;
		switch (resolveenter(consoleinp))
		{
		case start: {
			cout << "Waiting for connections..." << endl;
			started = true;
			break;
		}
		case quit: {
			char confirm;
			cout << "Are you sure? (Type 'y' to confirm)" << endl << "> ";
			cin >> confirm;
			if (confirm == 'y') {
				ExitProcess(EXIT_FAILURE);//It should not be used this way, it just quits the program instantaneously
			}
			else {
				break;
			}
		}

		case help: {
			cout << "" << endl;
			break;
		}

		default: {
			cout << "Invalid input" << endl;
			break;
		}
		}

		//if start command is applied
		if (started) {

			//intalize sockets&server
			WSADATA wsData;
			WORD ver = MAKEWORD(2, 2);
			int wsok = WSAStartup(ver, &wsData);
			if (wsok != 0) {
				cerr << "Can't Initialize winsock! Quitting.." << endl;
			}

			SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
			if (listening == INVALID_SOCKET) {
				cerr << "Can't create a socket! Quitting.." << endl;
			}

			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(54000);
			hint.sin_addr.S_un.S_addr = INADDR_ANY;

			bind(listening, (sockaddr*)&hint, sizeof(hint));

			listen(listening, SOMAXCONN);

			sockaddr_in client;
			int clientSize = sizeof(client);

			SOCKET clientSocket;

			client.sin_port = htons(54000);

			clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

			client.sin_port = htons(54000);

			char host[NI_MAXHOST]; // name
			char service[NI_MAXHOST]; // service (i. e. port) the client is connected on

			ZeroMemory(host, NI_MAXHOST);
			ZeroMemory(service, NI_MAXHOST);

			if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
				cout << host << " connected on port " << service << endl;
			}
			else {
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
				cout << host << " connected on port " << ntohs(client.sin_port) << endl;
			}
			//close listening socket after accpting client
			closesocket(listening);

			char buf[4096];

			//in-connection loop
			while (true) {

				ZeroMemory(buf, 4096);

				int bytesRecieved = recv(clientSocket, buf, 4096, 0);

				//handle some errors and disconnections
				if (bytesRecieved == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET) {
					cout << "Client disconnected. Quitting.. " << endl;
					break;
				}
				else if (bytesRecieved == SOCKET_ERROR) {
					cerr << "Error in recv(). Quitting.." << endl;
					break;
				}

				//Server online check strings

				if (string(buf, 0, bytesRecieved) == "olleh") {
					send(clientSocket, vermsg.c_str(), bytesRecieved + 1, 0);
				}
				else if (string(buf, 0, bytesRecieved) == "action") {

					//handle user input in the in-connected console
					cout << "> ";
					cin >> consoleinp;
					switch (resolvestart(consoleinp))
					{
					case makerrsound: {
						send(clientSocket, make_error_sound_tosend.c_str(), bytesRecieved + 1, 0);
						cout << "Done!" << endl;
						break;
					}

					case read: {
						send(clientSocket, read_tosend.c_str(), bytesRecieved + 1, 0);
						cout << "Read what?" << endl << "> ";
						cin >> toread;
						send(clientSocket, toread.c_str(), 4096, 0);
						break;
					}

					case show: {
						char confirm;
						cout << "Are you sure? (Type 'y' to confirm)" << endl << "> ";
						cin >> confirm;
						if (confirm == 'y') {
							send(clientSocket, show_tosend.c_str(), bytesRecieved + 1, 0);
							cout << "Debug console shown." << endl;
							break;
						}
						else {
							send(clientSocket, idle_tosend.c_str(), bytesRecieved + 1, 0);
							cout << "Aborted." << endl;
							break;
						}

					}

					case quitcl: {
						char confirm;
						cout << "Are you sure? (Type 'y' to confirm)" << endl << "> ";
						cin >> confirm;
						if (confirm == 'y') {
							send(clientSocket, quitcl_tosend.c_str(), bytesRecieved + 1, 0);
							cout << "Debug console shown." << endl;
							break;
						}
						else {
							send(clientSocket, idle_tosend.c_str(), bytesRecieved + 1, 0);
							cout << "Aborted." << endl;
							break;
						}
					}

					case hide: {
						send(clientSocket, hide_tosend.c_str(), bytesRecieved + 1, 0);
						cout << "Debug console hidden." << endl;
						break;
					}

					case bruh: {
						send(clientSocket, bruh_tosend.c_str(), bytesRecieved + 1, 0);
						cout << "maked BRUH.wav sound" << endl;
						break;
					}

					case minw: {
						send(clientSocket, minw_tosend.c_str(), bytesRecieved + 1, 0);
						cout << "Active window minimized" << endl;
						break;
					}

					case minwt: {
						int milliseconds = 0;
						int times = 0;
						send(clientSocket, minwt_tosend.c_str(), bytesRecieved + 1, 0);
						cout << "Delay in milliseconds between minimizations: " << endl;
						do {
							cin >> milliseconds;
						} while (milliseconds <= 0);
						string msec2send = to_string(milliseconds);
						send(clientSocket, msec2send.c_str(), 256, 0);
						cout << "How many times: " << endl;
						do {
							cin >> times;
						} while (times <= 0);
						string times2send = to_string(times);
						send(clientSocket, times2send.c_str(), 256, 0);
						break;
					}

					default: {
						cout << "Invalid input" << endl;
						send(clientSocket, idle_tosend.c_str(), bytesRecieved + 1, 0);
						break;
					}
					}
				}
			}
			closesocket(clientSocket);
			WSACleanup();
		}
	}
}
