#include <sstream>

#include "cryptography.h"
#include "language.h"

std::string *__language::assign(std::string pro_sec)
{
	std::string *temp = new std::string[7];

	temp[0] = "fail Your ip has been blocked due to repeated login attempts.";
	temp[0] += 0x0D;
	temp[0] += "You will be unlocked in a span of " + pro_sec + " seconds.";

	temp[1] = "fail The client is not the current version.";
	temp[1] += 0x0D;
	temp[1] += "Please, download and install the updated client from the website.";

	temp[2] = "fail Check your ID and Password.";

	temp[3] = "fail In this moment the server is in maintenance.";

	temp[4] = "fail This ID is currently in use.";

	temp[5] = "fail Your account has been banned !";
	temp[5] += 0x0D;
	temp[5] += "Please, contact the Team of Game.";

	temp[6] = "fail Enter your data respecting the correct case.";

	__cryptography *cryptography = new __cryptography();

	for (short i = 0; i < 7; i++)
		temp[i] = cryptography->encrypter(temp[i]);

	return temp;
}