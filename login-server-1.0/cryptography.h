#ifndef cryptography_h
#define cryptography_h

class __cryptography
{
public:
	std::string unencrypter(char *packet, int bytes);
	std::string encrypter(std::string packet);
	std::string unencrypter_pw(std::string pw);
};

#endif // cryptography_h