#ifndef threads_h
#define threads_h

std::mutex database_mutex;

void client(void *arg)
{
	SOCKET sock = (SOCKET) arg;

	__functions *functions = new __functions();
	__database *database = new __database();
	__cryptography *cryptography = new __cryptography();

	char packet[4096 + 1];
	int bytes = 0;

	try { bytes = recv(sock, packet, 4096, 0); } catch (...) { bytes = 0; }

	if (bytes >= 200 || bytes <= 60)
	{
		closesocket(sock);
	}
	else
	{
		packet[bytes] = 0;
		std::vector<std::string> data = functions->split(cryptography->unencrypter(packet, bytes), 0x20);

		if (data.size() != 7)
		{
			closesocket(sock);
		}
		else
		{
			std::vector<std::string> sVersion = functions->split(data[4], 0x0B);
			std::string version = sVersion.size() == 2 ? sVersion[1] : "0";
			std::string pw = cryptography->unencrypter_pw(data[3]);

			if (version != "0.9.3.3021" || data[5] != "0" || data[6].substr(0, data[6].size() - 1) != functions->uppercase(md5("529D12EF5699E29548A9914C7B2AB6DFA735FA8053A6B0EDFF75E607FF3EBBED" + data[2])))
			{
				send(sock, language_srv[1].c_str(), language_srv[1].size(), 0);
			}
			else if ((data[2].size() < 5 || data[2].size() > 16) || (pw.size() < 8 || pw.size() > 16) || functions->anti_sql_injection(data[2]) != 0) /* ID Length = Min. 5, Max. 16 | PW Length = Min. 8 Max. 16 | ID can't contain chars: 0x20, 0x7C, 0x5C, 0x22, 0x25, 0x26, 0x2F, 0x27, 0x2C, 0x3B, 0x3A, 0x2E, 0x60, 0x3D */
			{
				send(sock, language_srv[2].c_str(), language_srv[2].size(), 0);
			}
			else
			{
				pw = md5(pw);
				std::string qry = "SELECT `Username`, `Password`, `Online`, `Ban`, `Rank` FROM `users` WHERE `Username`='" + data[2] + "' AND `Password`='" + pw + "';";
				
				database_mutex.lock();
				if (mysql_query(conn, qry.c_str()) != 0)
				{
					database_mutex.unlock();
					std::cout << mysql_error(conn) << "\n";
					closesocket(sock);
				}
				else
				{
					database_mutex.unlock();
					MYSQL_RES *res = mysql_store_result(conn);
					if (mysql_num_rows(res) != 1)
					{
						send(sock, language_srv[2].c_str(), language_srv[2].size(), 0);
					}
					else
					{
						MYSQL_ROW row;

						std::string r_id, r_pw;
						short r_rank, r_online, r_ban;
						
						while (row = mysql_fetch_row(res))
						{
							try
							{
								r_id = row[0];
								r_pw = functions->uppercase(row[1]);
								r_online = atoi(row[2]);
								r_ban = atoi(row[3]);
								r_rank = atoi(row[4]);
							}
							catch (...)
							{
								r_rank = 0;
								r_online = r_ban = 1;
							}
						}

						mysql_free_result(res);

						if (maintenance == 0 && r_rank != 2)
						{
							send(sock, language_srv[3].c_str(), language_srv[3].size(), 0);
						}
						else if (data[2] != r_id || pw != r_pw)
						{
							send(sock, language_srv[6].c_str(), language_srv[6].size(), 0);
						}
						else if (r_online == 1)
						{
							send(sock, language_srv[4].c_str(), language_srv[4].size(), 0);
						}
						else if (r_ban == 1)
						{
							send(sock, language_srv[5].c_str(), language_srv[5].size(), 0);
						}
						else
						{
							std::string session = functions->random(5);
							qry = "UPDATE `users` SET `users`.`Session`='" + session + "' WHERE `Username`='" + data[2] + "';";

							database_mutex.lock();
							if (mysql_query(conn, qry.c_str()) != 0)
							{
								database_mutex.unlock();
								std::cout << mysql_error(conn) << "\n";
								closesocket(sock);
							}
							else
							{
								database_mutex.unlock();
								std::string channels = "NsTeST " + session;
								for (short i = 0; i < 5; i++)
									channels += " 127.0.0.1:" + functions->integer_s(4030 + i) + ":" + functions->get_channel_color(database->get_users(conn, functions->integer_s(i + 1), database_mutex)) + ":1." + functions->integer_s(i + 1) + ".S1-Aeros";;
								channels = cryptography->encrypter(channels);
								send(sock, channels.c_str(), channels.size(), 0);
							}
						}
					}
				}
			}
		}
	}
}

#endif // threads_h