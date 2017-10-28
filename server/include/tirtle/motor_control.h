#pragma once

#include "tirtle/path.h"

#ifdef __cplusplus
extern "C" {
#endif

void set_position(point_t loc, angle_t angle);
void load_image(path_t *paths, length_t num_paths);

#ifdef __cplusplus
}
#endif
