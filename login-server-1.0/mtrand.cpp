#include "mtrand.h"

unsigned long MTRand_int32::state[n] = { 0x0UL };
int MTRand_int32::p = 0;
bool MTRand_int32::init = false;

void MTRand_int32::gen_state() {
	for (int i = 0; i < (n - m); ++i)
		state[i] = state[i + m] ^ twiddle(state[i], state[i + 1]);

	for (int i = n - m; i < (n - 1); ++i)
		state[i] = state[i + m - n] ^ twiddle(state[i], state[i + 1]);

	state[n - 1] = state[m - 1] ^ twiddle(state[n - 1], state[0]);
	p = 0;
}

void MTRand_int32::seed(unsigned long s) {
	state[0] = s & 0xFFFFFFFFUL;

	for (int i = 1; i < n; ++i) {
		state[i] = 1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
		state[i] &= 0xFFFFFFFFUL;
	}

	p = n;
}

void MTRand_int32::seed(const unsigned long* array, int size) {
	seed(19650218UL);
	int i = 1, j = 0;

	for (int k = ((n > size) ? n : size); k; --k) {
		state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525UL))
			+ array[j] + j;
		state[i] &= 0xFFFFFFFFUL;
		++j; j %= size;
		if ((++i) == n) { state[0] = state[n - 1]; i = 1; }
	}

	for (int k = n - 1; k; --k) {
		state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941UL)) - i;
		state[i] &= 0xFFFFFFFFUL;
		if ((++i) == n) { state[0] = state[n - 1]; i = 1; }
	}

	state[0] = 0x80000000UL;
	p = n;
}