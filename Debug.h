
#pragma once
#ifdef DEBUG_SEED
#pragma GCC optimize ("O0")
#include "daisy_seed.h"
extern daisy::DaisySeed DEBUG_SEED;
#define D_PRINT(A,...) DEBUG_SEED.Print(A , ##__VA_ARGS__);
#else
#define D_PRINT(A,...)
#endif
