#include <algorithm>
#include <sstream>
#include <vector>
#include <time.h>

#include "functions.h"
#include "mtrand.h"

std::vector<std::string> __functions::split(const std::string str, char delim)
{
	std::vector<std::string> tmp;
	std::stringstream ss(str);
	std::string temp;
	while (std::getline(ss, temp, delim))
		tmp.push_back(temp);
	return tmp;
}

std::string __functions::uppercase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

short __functions::anti_sql_injection(std::string str)
{
	char sql_injection_chars [] = { 0x20, 0x7C, 0x5C, 0x22, 0x25, 0x26, 0x2F, 0x27, 0x2C, 0x3B, 0x3A, 0x2E, 0x60, 0x3D };
	for (int i = 0; i < sizeof(sql_injection_chars); i++)
		for (unsigned int j = 0; j < str.size(); j++)
			if (str[j] == sql_injection_chars[i]) { return 1; }
	return 0;
}

std::string __functions::random(short num)
{
	static char temp[32 + 1];
	MTRand_int32 irand((unsigned long) time(0));

	if (num <= 0) return "0";
	else if (num > 32) num = 32;

	for (short i = 0; i < num; i++)
		temp[i] = '0' + irand() % 10;

	temp[num] = 0;

	return std::string(temp);
}

std::string __functions::integer_s(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

std::string __functions::get_channel_color(int users)
{
	/* 50 -> 99 = GREEN | 100 -> 149 = ORANGE | 150 -> 200 = RED | OTHERS -> ... = WHITE */
	if (users >= 50 && users < 100)
	{
		return "4";
	}
	else if (users >= 100 && users < 150)
	{
		return "12";
	}
	else if (users >= 150 && users <= 200)
	{
		return "19";
	}
	else
	{
		return "0";
	}
}