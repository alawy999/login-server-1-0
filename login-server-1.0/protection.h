#ifndef protection_h
#define protection_h

std::map<std::string, short> counter;
std::mutex protection_mutex;

class __protection
{
private:
	short max;
public:
	__protection(short _max)
	{
		this->max = _max;
	}

	short check(std::string ip)
	{
		if (ip == "127.0.0.1") return 0;

		protection_mutex.lock();

		if (counter.find(ip) != counter.end())
		{
			if (counter[ip] > 0 && counter[ip] < this->max)
			{
				counter[ip]++;
				protection_mutex.unlock();
				return 0;
			}
			else
			{
				protection_mutex.unlock();
				return -1;
			}
		}
		else
		{
			counter.insert(std::pair<std::string, short>(ip, 1));
			protection_mutex.unlock();
			return 0;
		}
	}
};

void counter_reset(void *arg)
{
	std::cout << "PROTECTION" << "\t" << "INITIALIZED ( THREAD )\n";
	int sec = int(arg) * 1000;
	while (sec >= 0)
	{
		Sleep(sec);
		protection_mutex.lock();
		counter.clear();
		protection_mutex.unlock();
	}
}

#endif // protection_h