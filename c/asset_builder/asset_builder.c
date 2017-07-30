#include "asset_builder.h"

// Constants
#define MIN_POINTS_PER_CELL 0
#define MAX_POINTS_PER_CELL 8
#define MIN_SUPPORTED_IMAGE_SIZE 32
#define MAX_SUPPORTED_IMAGE_SIZE 2048
#define MAX_IMAGE_SIZE_FOR_3D 128

// Types
typedef enum {
    DetailLevel_low = 0,
    DetailLevel_medium = 1,
    DetailLevel_high = 2,
    DetailLevel_veryhigh = 3,
    DetailLevel_unknown = -1
} DetailLevel;
static const int DETAIL_LEVELS = 4;

typedef enum {
    NoiseType_perlin = 0,
    NoiseType_perlin_ridge = 1,
    NoiseType_perlin_fbm = 2,
    NoiseType_perlin_turbulence = 3,
    NoiseType_worley_pillows = 4,
    NoiseType_worley_gems = 5,
    NoiseType_worley_caustics = 6,
    NoiseType_worley_leaves = 7,
    NoiseType_worley_ridged_pillows = 8,
    NoiseType_worley_dots = 9,
    NoiseType_worley_bubbles = 10,
    NoiseType_unknown = -1
} NoiseType;
static const int NOISE_TYPES = 11;

typedef struct {
    float z;
    int channels;
    int size;
    const char *path;
    int tile_result;
} WriteConfig;

typedef struct {
    NoiseType type;
    float lacunarity;
    float gain;
    int octaves;
    int dimensions;
    int sample_scale;
    int cells_per_dimension;
    int min_points_per_cell;
    int max_points_per_cell;
} NoiseConfig;

typedef struct {
    double sum;
    double min;
    double max;
    int64_t samples;
    double avg;
} NoiseMetrics;

typedef float (*value_func)(float x, float y, float z, const WriteConfig *config, const void *payload);
typedef float (*map_func)(float value, const NoiseMetrics *metrics);

// Constants
static const NoiseMetrics DEFAULT_NOISE_METRICS = {
        .sum = 0,
        .min = 1000000.0,
        .max = -1000000.0,
        .samples = 0,
        .avg = 0.0
};

static const WriteConfig DEFAULT_WRITE_CONFIG = {
        .path = "output/test.png",
        .size = 64,
        .channels = 1,
        .tile_result = 1,
        .z = 0.0f
};

static const NoiseConfig DEFAULT_NOISE_CONFIG = {
        .lacunarity = 2.0f,
        .gain = 0.5f,
        .octaves = 6,
        .type = NoiseType_perlin,
        .sample_scale = 8,
        .cells_per_dimension = 8,
        .min_points_per_cell = 1,
        .max_points_per_cell = 3
};

// Helpers
char* get_name_for_detail_level(DetailLevel level){
    switch (level) {
        case DetailLevel_low:
            return "low";
        case DetailLevel_medium:
            return "medium";
        case DetailLevel_high:
            return "high";
        case DetailLevel_veryhigh:
            return "veryhigh";
        default:
            return "unknown";
    }
}

char* get_name_for_noise_type(NoiseType type){
    switch (type) {
        case NoiseType_perlin_fbm:
            return "perlin_fbm";
        case NoiseType_perlin_ridge:
            return "perlin_ridge";
        case NoiseType_perlin:
            return "perlin_layer";
        case NoiseType_perlin_turbulence:
            return "perlin_turbulence";
        case NoiseType_worley_dots:
            return "worley_dots";
        case NoiseType_worley_gems:
            return "worley_gems";
        case NoiseType_worley_leaves:
            return "worley_leaves";
        case NoiseType_worley_bubbles:
            return "worley_bubbles";
        case NoiseType_worley_pillows:
            return "worley_pillows";
        case NoiseType_worley_ridged_pillows:
            return "worley_ridged_pillows";
        case NoiseType_worley_caustics:
            return "worley_caustics";
        default:
            return "unknown";
    }
}

NoiseType get_noise_type_from_string(const char* noise_type){
    for (int i = 0; i < NOISE_TYPES; i++){
        if (strcmp(noise_type, get_name_for_noise_type((NoiseType)i)) == 0){
            return (NoiseType)i;
        }
    }
    return NoiseType_unknown;
}

DetailLevel get_detail_level_from_string(const char* detail_level){
    for (int i = 0; i < DETAIL_LEVELS; i++){
        if (strcmp(detail_level, get_name_for_detail_level((DetailLevel)i)) == 0){
            return (DetailLevel)i;
        }
    }
    return DetailLevel_unknown;
}

bool is_noise_type_perlin(NoiseType type){
    switch(type){
        case NoiseType_perlin:
        case NoiseType_perlin_fbm:
        case NoiseType_perlin_ridge:
        case NoiseType_perlin_turbulence:
            return true;
        default:
            return false;
    }
}

int write_tiled_png(const WriteConfig *c, void *image_data) {
    void *tiled_image_data = calloc(sizeof(int8_t), c->size * c->size * c->channels * 9);
    size_t bytes_per_row = c->size * c->channels;
    for (int i = 0; i < c->size; i++) {
        for (int j = 0; j < 3; j++) {
            void *dest = tiled_image_data + (bytes_per_row * j) + (bytes_per_row * 3 * i);
            void *src = image_data + (bytes_per_row * i);
            memcpy(dest, src, bytes_per_row);
        }
    }

    size_t bytes_per_strip = bytes_per_row * 3 * c->size;
    for (int j = 1; j < 3; j++) {
        void *dest = tiled_image_data + (bytes_per_strip * j);
        memcpy(dest, tiled_image_data, bytes_per_strip);
    }

    int result = stbi_write_png(c->path, c->size * 3, c->size * 3, c->channels, tiled_image_data, c->channels * c->size * 3);
    free(tiled_image_data);
    return result;
}

int write_noise_png(const WriteConfig *c, value_func value, map_func map, const NoiseConfig *payload) {
    const int size_2 = c->size * c->size;
    float *values = calloc(sizeof(float), size_2);
    int8_t *image_data = calloc(sizeof(int8_t), size_2 * c->channels);

    // Calculate metrics
    NoiseMetrics metrics = DEFAULT_NOISE_METRICS;
    float v = 0;
    for (int i = 0; i < c->size; i++) {
        for (int j = 0; j < c->size; j++) {
            float v = (*value)((float)i, (float)j, (float) c->z, c, payload);
            values[(i + j * c->size)] = v;
            metrics.samples++;
            metrics.sum += v;
            metrics.min = MIN(v, metrics.min);
            metrics.max = MAX(v, metrics.max);
        }
    }
    metrics.avg = metrics.sum / metrics.samples;

    // Map & write
    float m = 0;
    int8_t pixel = 0;
    int offset = 0;
    for (int i = 0; i < c->size; i++) {
        for (int j = 0; j < c->size; j++) {
            offset = i + j * c->size;
            m = (*map)(values[offset], &metrics);
            assert(m >= 0.0f && m <= 1.0f);
            pixel = (int8_t)((int) (m * 255));
            image_data[offset] = pixel;
        }
    }
    int result = c->tile_result ?
                    write_tiled_png(c, image_data) :
                    stbi_write_png(c->path, c->size, c->size, c->channels, image_data, c->channels * c->size);

    free(image_data);
    free(values);
    return result;
}

// Map functions
float noise_map_func(float value, const NoiseMetrics *metrics){
    float delta = metrics->max - metrics->min;
    return delta > 0.0f ? ((value - metrics->min) / delta) : 0.0f;
}

// Worley functions
float worley_pillows(float f0, float f1){
    return 1.0f - clamp01(f0);
}

float worley_gems(float f0, float f1){
    return f1 - f0;
}

float worley_caustics(float f0, float f1){
    return f0;
}

float worley_leaves(float f0, float f1){
    return 1.0f - clamp01(f1);
}

float worley_ridged_pillows(float f0, float f1){
    return 1.0f - clamp01((f0 + f1) * 0.5f);
}

float worley_dots(float f0, float f1){
    return f0 > 0.5f ? 0.0f : 1.0f - (f0 / 0.5f);
}

float worley_bubbles(float f0, float f1){
    return clamp01((sinf(f0 * 3.14f * 16.0f) + 1.0f) * 0.5f);
}

// Values functions
float noise_value_func(float x, float y, float z, const WriteConfig *config, const void *payload) {
    NoiseConfig noise_config = *((NoiseConfig *) payload);
    int d = noise_config.sample_scale;
    float rel_x = (x / (float) config->size) * (float) d;
    float rel_y = (y / (float) config->size) * (float) d;
    float rel_z = (z / (float) config->size) * (float) d;
    switch (noise_config.type) {
        case NoiseType_perlin_fbm:
            return stb_perlin_fbm_noise3(rel_x, rel_y, rel_z, noise_config.lacunarity, noise_config.gain, noise_config.octaves, d, d, d);
        case NoiseType_perlin:
            return stb_perlin_noise3(rel_x, rel_y, rel_z, d, d, d);
        case NoiseType_perlin_ridge:
            return stb_perlin_ridge_noise3(rel_x, rel_y, rel_z, noise_config.lacunarity, noise_config.gain, noise_config.gain, noise_config.octaves, d, d, d);
        case NoiseType_perlin_turbulence:
            return stb_perlin_turbulence_noise3(rel_x, rel_y, rel_z, noise_config.lacunarity, noise_config.gain, noise_config.octaves, d, d, d);
        case NoiseType_worley_gems:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_gems);
        case NoiseType_worley_dots:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_dots);
        case NoiseType_worley_leaves:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_leaves);
        case NoiseType_worley_pillows:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_pillows);
        case NoiseType_worley_ridged_pillows:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_ridged_pillows);
        case NoiseType_worley_bubbles:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_bubbles);
        case NoiseType_worley_caustics:
            return worley(rel_x, rel_y, rel_z, noise_config.cells_per_dimension, noise_config.min_points_per_cell, noise_config.max_points_per_cell, &worley_caustics);
        default:
            return 0.0f;
    }
};

// Public interfaces
bool easy_noise(const char* noise_type, const char* detail_level, int image_size, int dimensions, int min_points_per_cell, int max_points_per_cell, const char* folder){

    // Validation
    NoiseType n = get_noise_type_from_string(noise_type);
    bool is_perlin = is_noise_type_perlin(n);
    if (n == NoiseType_unknown){
        printf("\neasy_noise: invalid noise_type: %s\n", noise_type);
        return false;
    }
    int detail_value = get_detail_level_from_string(detail_level);
    if (detail_value == DetailLevel_unknown){
        printf("\neasy_noise: invalid detail_level: %s\n", detail_level);
        return false;
    }
    detail_value = pow(2, detail_value + 1);
    detail_value = is_perlin ? detail_value * 2 : detail_value;
    if (image_size < MIN_SUPPORTED_IMAGE_SIZE || image_size > MAX_SUPPORTED_IMAGE_SIZE || !is_power_of_two(image_size)){
        printf("\neasy_noise: invalid image_size: %i\n", image_size);
        return false;
    }
    if (dimensions < 2 || dimensions > 3){
        printf("\neasy_noise: invalid dimensions: %i\n", dimensions);
        return false;
    }
    if (dimensions == 3 && image_size > MAX_IMAGE_SIZE_FOR_3D){
        printf("\neasy_noise: only can use 3 dimensions with image_size <= 128\n");
        return false;
    }

    WriteConfig config = DEFAULT_WRITE_CONFIG;
    config.size = image_size;
    config.tile_result = 0;
    char* normalized_folder = folder != NULL ? folder : "";

    NoiseConfig noise_config = DEFAULT_NOISE_CONFIG;
    noise_config.type = n;
    noise_config.cells_per_dimension = detail_value;
    noise_config.min_points_per_cell = clamp_int(min_points_per_cell, MIN_POINTS_PER_CELL, MAX_POINTS_PER_CELL);
    noise_config.max_points_per_cell = clamp_int(max_points_per_cell, noise_config.min_points_per_cell, MAX_POINTS_PER_CELL);
    noise_config.sample_scale = detail_value;

    int z_slices = dimensions == 2 ? 1 : image_size;
    for (int i = 0; i < z_slices; i++){
        config.z = (float)i;
        sds path = sdsnew("");
        path = sdscatfmt(path, "%s%s_%s_%i_%i.png", normalized_folder, noise_type, detail_level, image_size, i);
        config.path = path;
        int result = write_noise_png(&config, &noise_value_func, &noise_map_func, &noise_config);
        assert(result);
        sdsfree(path);
    }
    return true;
}

// Test method
bool write_test_pngs() {
    char* output_folder = "output/";

    // Test all variations of 2D noise @ 128
    for (int i = 0; i < NOISE_TYPES; i++){
        for (int j = 0; j < DETAIL_LEVELS; j++){
            assert(easy_noise(get_name_for_noise_type(i), get_name_for_detail_level(j), 128, 2, 1, 4, output_folder));
        }
    }

    // Test some 3D noise @ 32
    assert(easy_noise("perlin_layer", "low", 32, 3, 1, 4, output_folder));

    // Test a fairly expensive noise variation and profile
    start_timer();
    assert(easy_noise("worley_pillows", "low", 1024, 2, 1, 4, output_folder));
    stop_timer();

    return true;
}
