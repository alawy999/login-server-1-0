#include <iostream>
#include <sstream>
#include <string>

#include "cryptography.h"

std::string __cryptography::unencrypter(char *packet, int bytes)
{
	std::string dec;
	for (int i = 0; i < bytes; i++)
		dec += (packet[i] - 0xF) ^ 0xC3;
	return dec;
}

std::string __cryptography::encrypter(std::string packet)
{
	std::string enc;
	for (unsigned int i = 0; i < packet.size(); i++)
		enc += packet[i] + 15;
	return enc += 0x19;
}

std::string __cryptography::unencrypter_pw(std::string pw)
{
	int rm = pw.size() % 4;
	pw = pw.erase(0, rm != 0 ? rm : 4);
	std::string dec;
	for (unsigned int i = 1; i < pw.size(); i += 2)
		dec += pw[i];
	pw = dec;
	dec.clear();
	for (unsigned int i = 0; i < pw.size(); i++)
	{
		if (i != 0 && i % 2 == 0)
			dec += " ";
		dec += pw[i];
	}
	std::stringstream ss(dec);
	pw = dec = "";
	int convert;
	while (ss >> std::hex >> convert) dec.push_back(convert);
	return dec;
}