#ifndef mtrand_h
#define mtrand_h

class MTRand_int32 {
public:
	MTRand_int32() { if (!init) seed(5489UL); init = true; }
	MTRand_int32(unsigned long s) { seed(s); init = true; }
	MTRand_int32(const unsigned long* array, int size) { seed(array, size); init = true; }
	void seed(unsigned long);
	void seed(const unsigned long*, int size);
	unsigned long operator()() { return rand_int32(); }
	virtual ~MTRand_int32() {}
protected:
	unsigned long rand_int32();
private:
	static const int n = 624, m = 397;
	static unsigned long state[n];
	static int p;
	static bool init;
	unsigned long twiddle(unsigned long, unsigned long);
	void gen_state();
	MTRand_int32(const MTRand_int32&);
	void operator=(const MTRand_int32&) ;
};

inline unsigned long MTRand_int32::twiddle(unsigned long u, unsigned long v) {
	return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)
		^ ((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
}

inline unsigned long MTRand_int32::rand_int32() {
	if (p == n) gen_state();
	unsigned long x = state[p++];
	x ^= (x >> 11);
	x ^= (x << 7) & 0x9D2C5680UL;
	x ^= (x << 15) & 0xEFC60000UL;
	return x ^ (x >> 18);
}

class MTRand : public MTRand_int32 {
public:
	MTRand() : MTRand_int32() {}
	MTRand(unsigned long seed) : MTRand_int32(seed) {}
	MTRand(const unsigned long* seed, int size) : MTRand_int32(seed, size) {}
	~MTRand() {}
	double operator()() {
		return static_cast<double>(rand_int32()) * (1. / 4294967296.);
	}
private:
	MTRand(const MTRand&);
	void operator=(const MTRand&) ;
};

class MTRand_closed : public MTRand_int32 {
public:
	MTRand_closed() : MTRand_int32() {}
	MTRand_closed(unsigned long seed) : MTRand_int32(seed) {}
	MTRand_closed(const unsigned long* seed, int size) : MTRand_int32(seed, size) {}
	~MTRand_closed() {}
	double operator()() {
		return static_cast<double>(rand_int32()) * (1. / 4294967295.);
	}
private:
	MTRand_closed(const MTRand_closed&);
	void operator=(const MTRand_closed&) ;
};

class MTRand_open : public MTRand_int32 {
public:
	MTRand_open() : MTRand_int32() {}
	MTRand_open(unsigned long seed) : MTRand_int32(seed) {}
	MTRand_open(const unsigned long* seed, int size) : MTRand_int32(seed, size) {}
	~MTRand_open() {}
	double operator()() {
		return (static_cast<double>(rand_int32()) + .5) * (1. / 4294967296.);
	}
private:
	MTRand_open(const MTRand_open&);
	void operator=(const MTRand_open&) ;
};

class MTRand53 : public MTRand_int32 {
public:
	MTRand53() : MTRand_int32() {}
	MTRand53(unsigned long seed) : MTRand_int32(seed) {}
	MTRand53(const unsigned long* seed, int size) : MTRand_int32(seed, size) {}
	~MTRand53() {}
	double operator()() {
		return (static_cast<double>(rand_int32() >> 5) * 67108864. +
			static_cast<double>(rand_int32() >> 6)) * (1. / 9007199254740992.);
	}
private:
	MTRand53(const MTRand53&);
	void operator=(const MTRand53&) ;
};

#endif // mtrand_h