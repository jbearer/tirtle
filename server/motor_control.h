#pragma once

#include "path.h"

#ifdef __cplusplus
extern "C" {
#endif

void set_position(point_t loc, angle_t angle);
void load_image(path_t *paths, length_t num_paths);
void step_tirtle();
void init_tirtle();

#ifdef __cplusplus
}
#endif
