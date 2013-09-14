#include <windows.h>
#include <sstream>
#include <mutex>

#include "mysql.h"
#include "database.h"

short __database::connection(MYSQL *conn, char *host, char *user, char *pass, char *name)
{
	return !mysql_real_connect(conn, host, user, pass, name, 0, 0, 0) ? -1 : 0;
}

int __database::get_users(MYSQL *conn, std::string srv_num, std::mutex &database_mutex)
{
	std::string qry = "SELECT * FROM `users` WHERE `Online`='" + srv_num + "'";
	database_mutex.lock();
	if (mysql_query(conn, qry.c_str()) != 0)
	{
		database_mutex.unlock();
		return 0;
	}
	database_mutex.unlock();
	MYSQL_RES *res = mysql_store_result(conn);
	return (int) mysql_num_rows(res);
}