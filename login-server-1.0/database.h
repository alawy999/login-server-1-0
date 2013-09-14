#ifndef database_h
#define database_h

class __database
{
public:
	short connection(MYSQL *conn, char *host, char *user, char *pass, char *name);
	int get_users(MYSQL *conn, std::string srv_num, std::mutex &database_mutex);
};

#endif // database_h