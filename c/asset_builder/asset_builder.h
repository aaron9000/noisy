#pragma once
#include "common.h"
#include "worley.h"

bool easy_noise(const char* noise_type, const char* detail_level, int image_size, int dimensions, int min_points_per_cell, int max_points_per_cell, const char* folder);
bool write_test_pngs();
