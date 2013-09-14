#ifndef functions_h
#define functions_h

class __functions
{
public:
	std::vector<std::string> split(const std::string str, char delim);
	std::string uppercase(std::string str);
	short anti_sql_injection(std::string str);
	std::string random(short num);
	std::string integer_s(int num);
	std::string get_channel_color(int users);
};

#endif // functions_h