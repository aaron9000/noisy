#include "worley.h"
#include "rand_lookup.h"

static int seed = 123;
static int offset_x[27] = {
    0, -1, 0, 0, 0, 0, 1, -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1
};
static int offset_y[27] = {
    0, 0, -1, 0, 0, 1, 0, -1, 0, 0, 1, -1, -1, 1, 1, -1, 0, 0, 1, -1, -1, 1, 1, -1, -1, 1, 1
};
static int offset_z[27] = {
    0, 0, 0, -1, 1, 0, 0, 0, -1, 1, 0, -1, 1, -1, 1, 0, -1, 1, 0, -1, 1, -1, 1, -1, 1, -1, 1
};
static float offset_x_float[27] = {
    0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static float offset_y_float[27] = {
    0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f
};
static float offset_z_float[27] = {
    0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f
};

static inline int random_in_range(int min_inclusive, int max_inclusive, float r){
    float spread = (float)max_inclusive - (float)min_inclusive;
    return (int)(r * spread) + min_inclusive;
}

static inline float wrap_int(int x, int interval){
	if (x < 0){
        return interval - (abs(x) % interval);
	}else{
        return x % interval;
    }
}

static inline float wrap_float(float x, float interval){
	if (x < 0){
        return interval - fmod(fabs(x), interval);
	}else{
        return fmod(x, interval);
    }
}

static inline float get_edge_distance(float local_pos, int offset){
    if (offset == 0){
        return 0.0f;
    }else if (offset > 0){
        return 1.0f - local_pos;
    }else{
        return local_pos;
    }
}

float worley(float x, float y, float z, int cells_per_dimension, int min_points_per_cell, int max_points_per_cell, const worley_func worley_function){

    int index, num_points, offset_cell_x, offset_cell_y, offset_cell_z, stride;
	float dx, dy, dz, px, py, pz, distance_squared;
    float last_f0 = FLT_MAX;
    float f0 = FLT_MAX;
    float f1 = FLT_MAX;
    int cell_x_int = wrap_int((int)x, cells_per_dimension);
    int cell_y_int = wrap_int((int)y, cells_per_dimension);
    int cell_z_int = wrap_int((int)z, cells_per_dimension);
	float cell_x_float = wrap_float(x, (float)cells_per_dimension);
	float cell_y_float = wrap_float(y, (float)cells_per_dimension);
    float cell_z_float = wrap_float(z, (float)cells_per_dimension);
    float wx = wrap_float(cell_x_float, 1.0f);
    float wy = wrap_float(cell_y_float, 1.0f);
    float wz = wrap_float(cell_z_float, 1.0f);
    int cells_squared = cells_per_dimension * cells_per_dimension;

    // Calculate distance to edges (cell rejection optimization)
    float cell_edge_distances_squared[27] = {0};
    float edge_distance, ex, ey, ez;
    for (int e = 0; e < 27; e++){
        ex = get_edge_distance(wx, offset_x[e]);
        ey = get_edge_distance(wy, offset_y[e]);
        ez = get_edge_distance(wz, offset_z[e]);
        cell_edge_distances_squared[e] = ex * ex + ey * ey + ez * ez;
    }

    // Loop through self and neighbors
    for (int i = 0; i < 27; i++){

        if (f1 <= cell_edge_distances_squared[i]) continue;

		offset_cell_x = wrap_int(cell_x_int + offset_x[i], cells_per_dimension);
		offset_cell_y = wrap_int(cell_y_int + offset_y[i], cells_per_dimension);
        offset_cell_z = wrap_int(cell_z_int + offset_z[i], cells_per_dimension);
        index = offset_cell_x +
                (offset_cell_y * cells_per_dimension) +
                (offset_cell_z * cells_squared);
        index = (seed + (index * 4)) % RAND_LOOKUP_COUNT;
		num_points = random_in_range(min_points_per_cell, max_points_per_cell, rand_lookup[index]);

        for (int j = 0; j < num_points; j++){
            stride = index + (j * 10);
			px = rand_lookup[(stride + 1) % RAND_LOOKUP_COUNT] + offset_x_float[i];
			py = rand_lookup[(stride + 2) % RAND_LOOKUP_COUNT] + offset_y_float[i];
            pz = rand_lookup[(stride + 3) % RAND_LOOKUP_COUNT] + offset_z_float[i];

            dx = wx - px;
            distance_squared = dx * dx;
            if (distance_squared > f1) continue;

            dy = wy - py;
            distance_squared += dy * dy;
            if (distance_squared > f1) continue;

            dz = wz - pz;
            distance_squared += dz * dz;
            if (distance_squared > f1) continue;

            last_f0 = f0;
			f0 = distance_squared < f0 ? distance_squared : f0;
            f1 = (distance_squared > f0 && distance_squared < f1) ? distance_squared : f1;
            f1 = (last_f0 > f0 && last_f0 < f1) ? last_f0 : f1;
        }
    }

    if (worley_function == NULL){
        return 1.0f - clamp01(sqrtf(f0));
    }else{
        return clamp01((*worley_function)(sqrtf(f0), sqrtf(f1)));
    }
}

void worley_randomize_seed(){
    seed = abs((int)time(NULL) % RAND_LOOKUP_COUNT);
}

void worley_change_seed(int new_seed){
    seed = abs(new_seed % RAND_LOOKUP_COUNT);
}

void generate_random_numbers(){
	printf("\n");
    printf("float rand_lookup[%i] = {\n", (int)RAND_LOOKUP_COUNT);
	for (int i = 0; i < RAND_LOOKUP_COUNT; i++){
		printf("%ff,\n", rand_float());
	}
    printf("};\n");
}

// Javascript Helper
// function generateNeighbors(){
// 	var offsets = [-1, 0, 1];
// 	var tuples = {};
// 	for (var i = 0; i < 3; i++){
// 		for (var j = 0; j < 3; j++){
// 			for (var k = 0; k < 3; k++){
// 				var dx = offsets[i];
// 				var dy = offsets[j];
// 				var dz = offsets[k];
// 				var dist = dx * dx + dy * dy + dz * dz;
// 				var pos = {dx, dy, dz};
// 				var d = dist.toString();
// 				if (tuples[d]){
// 					tuples[d].push(pos)
// 				} else {
// 					tuples[d] = [pos];
// 				}
// 			}
// 		}
// 	}
//
// 	var points = (tuples["0"]).concat(tuples["1"]).concat(tuples["2"]).concat(tuples["3"]);
//
// 	var xs = "";
// 	var ys = "";
// 	var zs = "";
// 	for (var p = 0; p < points.length; p++){
// 		xs += points[p].dx + ", ";
// 		ys += points[p].dy + ", ";
// 		zs += points[p].dz + ", ";
// 	}
// 	console.log(xs);
// 	console.log(ys);
// 	console.log(zs);
// }
// generateNeighbors();
