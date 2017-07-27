#pragma once
#include "common.h"

typedef float (*worley_func)(float f0, float f1);

float worley(float x, float y, float z,
    int buckets_per_dimension,
    int min_points_per_cell,
    int max_points_per_cell,
    const worley_func worley_function);

void worley_randomize_seed();

void worley_change_seed(int new_seed);

//void generate_random_numbers();
