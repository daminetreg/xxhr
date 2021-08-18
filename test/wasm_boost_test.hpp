#pragma once


#ifdef __EMSCRIPTEN__
	#include <signal.h>
	extern "C" {
		int sigaltstack(const stack_t *__restrict, stack_t *__restrict) { return 0; } //XXX: Check in newer emcc version if we can remove this.
	}
#endif