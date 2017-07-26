#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <float.h>

#include "../vendor/stb.h"
#include "../vendor/sds/sds.h"
#include "../vendor/tictoc/tictoc.h"
#include "../vendor/libmtwist/mtwist.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void seed_rand(int seed);
float rand_float();
int range_int(int min_inclusive, int max_inclusive);
int clamp_int(int v, int min_inclusive, int max_inclusive);
float clamp01(float value);
float clamp(float value, float min, float max);
bool is_power_of_two(int x);
void start_timer();
void stop_timer();
