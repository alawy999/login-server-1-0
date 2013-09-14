#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <mutex>
#include <map>
#include <process.h>
#include <mysql.h>
#include <vector>

#include "database.h"
#include "main.h"
#include "protection.h"
#include "cryptography.h"
#include "functions.h"
#include "mtrand.h"
#include "language.h"
#include "MD5.h"
#include "threads.h"

#pragma comment(lib, "ws2_32.lib")

void destroy(short exit_num);

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4003

#define DATABASE_IP "127.0.0.1"
#define DATABASE_USER "root"
#define DATABASE_PASS "123456"
#define DATABASE_NAME "nos"

int main()
{
	system("TITLE Login server 1.0");

	std::cout
		<< "============================================================\n"
		<< " AUTHENTICATION SERVER                                      \n"
		<< "============================================================\n";

	// SETTINGS

	__language *language = new __language();

	maintenance = -1;
	language_srv = language->assign("60"); /* 60 = protection seconds to wait first of reset the counter */

	// PROTECTION

	__protection *protection = new __protection(3);
	
	_beginthread((void(__cdecl*)(void*)) counter_reset, 0, (void*) 60); /* 60 = protection seconds to wait first of reset the counter */

	// DATABASE

	clock_t start = clock();
	
	__database *database = new __database();
	
	conn = mysql_init(0);
	if (database->connection(conn, DATABASE_IP, DATABASE_USER, DATABASE_PASS, DATABASE_NAME) != 0)
	{
		std::cout << "DATABASE" << "\t" << "ERROR\n";
		destroy(1);
	}

	clock_t stop = clock();
	
	std::cout << "DATABASE" << "\t" << "CONNECTED IN " << (double) (stop - start) / 1000 << " SEC ( " << DATABASE_IP << " 3306 )\n";

	// SERVER

	start = clock();

	WSAData wsa;
	WSAStartup(0x0201, &wsa);

	SOCKET sock_ls = socket(2, 1, 0), sock = 0;

	SOCKADDR_IN sock_addr;
	sock_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	sock_addr.sin_family = 2;
	sock_addr.sin_port = htons(SERVER_PORT);

	bind(sock_ls, (SOCKADDR*) &sock_addr, sizeof(sock_addr));

	stop = clock();

	std::cout << "SERVER  " << "\t" << "STARTED IN " << (double) (stop - start) / 1000 << " SEC ( " << SERVER_IP << " " << SERVER_PORT << " )\n\n";

	_beginthread((void(__cdecl*)(void*)) commands, 0, (void*) 0);

	int ls_result = 0, sin_size = 0;

	while (ls_result >= 0)
	{
		if ((ls_result = listen(sock_ls, 0x7FFFFFFF)) >= 0)
		{
			sin_size = sizeof(sock_addr);
			sock = accept(sock_ls, (SOCKADDR*) &sock_addr, &sin_size);

			if (protection->check(inet_ntoa(sock_addr.sin_addr)) == 0)
			{
				_beginthread((void(__cdecl*)(void*)) client, 0, (void*) sock);
			}
			else
			{
				send(sock, language_srv[0].c_str(), language_srv[0].size(), 0);
			}

			sock = 0;
		}
		else std::cout << "COULD NOT LISTEN ON " << SERVER_IP << ":" << SERVER_PORT << "\n";
	}

	WSACleanup();
	destroy(0);
}

void destroy(short exit_num)
{
	system("pause");
	exit(exit_num);
}