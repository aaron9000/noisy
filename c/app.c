
#include "asset_builder/asset_builder.h"

int main(int argc, char **argv) {
    char* arg = "";
    char* noise_type = "perlin_fbm";
    char* detail_level = "medium";
    int image_size = 256;
    int dimensions = 2;
    int min_points_per_cell = 1;
    int max_points_per_cell = 4;
    for (int i = 0; i < argc; i++){
        arg = argv[i];
        switch (i) {
            case 1:
                noise_type = arg;
                break;
            case 2:
                detail_level = arg;
                break;
            case 3:
                sscanf(arg, "%i", &image_size);
                break;
            case 4:
                sscanf(arg, "%i", &dimensions);
                break;
            case 5:
                sscanf(arg, "%i", &min_points_per_cell);
                break;
            case 6:
                sscanf(arg, "%i", &max_points_per_cell);
                break;
        }
    }

    // TODO: accept folder / path as argument?
    printf("\n noise_type = %s\n detail_level = %s\n image_size = %i\n dimensions = %i\n min_points_per_cell = %i\n max_points_per_cell = %i\n", noise_type, detail_level, image_size, dimensions, min_points_per_cell, max_points_per_cell);
    easy_noise(noise_type, detail_level, image_size, dimensions,min_points_per_cell, max_points_per_cell, "");
}
