#include "../vendor/minunit.h"
#include "../asset_builder/asset_builder.h"

MU_TEST(test_common) {

    // MIN / MAX
    mu_check(MIN(0.5f, 0.25f) == 0.25f);
    mu_check(MIN(50, 25) == 25);
    mu_check(MAX(0.5f, 0.25f) == 0.5f);
    mu_check(MAX(50, 25) == 50);

    // seed_rand
    seed_rand(time(NULL));

    // rand_float
    int samples = 1000;
    float sum = 0.0f;
    for (int i = 0; i < samples; i++){
        sum += rand_float();
    }
    float avg = sum / (float)samples;
    mu_check(avg > 0.45f && avg < 0.55f);

    // range_int
    int r = 0;
    for (int j = 0; j < 10; j++){
        r = range_int(4, 12);
        mu_check(r >= 4 && r <= 12);
    }

    // clamp_int
    mu_check(clamp_int(5, 1, 10) == 5);
    mu_check(clamp_int(50, 1, 10) == 10);
    mu_check(clamp_int(-50, 1, 10) == 1);

    // clamp01
    mu_check(clamp01(0.5f) == 0.5f);
    mu_check(clamp01(2.0f) == 1.0f);
    mu_check(clamp01(-1.0f) == 0.0f);

    // clamp
    mu_check(clamp_int(5.0f, 1.0f, 10.0f) == 5.0f);
    mu_check(clamp_int(50.0f, 1.0f, 10.0f) == 10.0f);
    mu_check(clamp_int(-50.0f, 1.0f, 10.0f) == 1.0f);

    // is_power_of_two
    mu_check(is_power_of_two(0) == false);
    mu_check(is_power_of_two(3) == false);
    mu_check(is_power_of_two(2) == true);
    mu_check(is_power_of_two(8) == true);
    mu_check(is_power_of_two(1) == true);
}


MU_TEST(test_asset_builder) {

    // build a variety of different noise (review by eye)
    mu_check(write_test_pngs());

    // invalid inputs
    mu_check(!easy_noise("foo", "low", 1, 4, 32, 2));
    mu_check(!easy_noise("perlin_fbm", "foo", 1, 4, 32, 2));
    mu_check(!easy_noise("perlin_fbm", "low", 1, 4, 32, 4));
    mu_check(!easy_noise("perlin_fbm", "low", 1, 4, 256, 3));
    mu_check(!easy_noise("perlin_fbm", "low", 1, 4, 4096, 2));
    mu_check(!easy_noise("perlin_fbm", "low", 1, 4, 123, 2));
}


int main() {
    MU_RUN_TEST(test_common);
    MU_RUN_TEST(test_asset_builder);
    MU_REPORT();
    return 0;
}
