#include "common.h"

static mtwist* twister = NULL;
static int initialized_rng = 0;

int range_int(int min_inclusive, int max_inclusive){
    float r = rand_float();
    float spread = (float)max_inclusive - (float)min_inclusive;
    return (int)(r * spread) + min_inclusive;
}

void seed_rand(int seed) {
    if (twister != NULL){
        mtwist_free(twister);
    }
    twister = mtwist_new();
    mtwist_init(twister, seed);
    initialized_rng = 1;
}

float rand_float(){
    if (!initialized_rng){
        seed_rand(time(NULL));
    }
    return (float)mtwist_drand(twister);
}

float clamp01(float value){
    return MAX(MIN(value, 1.0f), 0.0f);
}

float clamp(float value, float min, float max){
    return MAX(MIN(value, max), min);
}

int clamp_int(int v, int min_inclusive, int max_inclusive){
    if (v >= min_inclusive && v <= max_inclusive){
        return v;
    }else if (v >= min_inclusive){
        return max_inclusive;
    } else {
        return min_inclusive;
    }
}

bool is_power_of_two(int x){
    return (x != 0) && ((x & (x - 1)) == 0);
}

static TicTocTimer c = {0};

void start_timer(){
    c = tic();
}

void stop_timer(){
    printf("\nstop_timer %fs\n", toc(&c));
}
