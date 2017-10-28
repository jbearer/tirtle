#include <Arduino.h>

#include "motor_control.h"

void set_position(point_t loc, angle_t angle)
{
    Serial.println("updated position");
    Serial.print("  x      = "); Serial.println(loc.x, DEC);
    Serial.print("  y      = "); Serial.println(loc.y, DEC);
    Serial.print("  theta  = "); Serial.println(angle, DEC);
}

void load_image(path_t *paths, length_t num_paths)
{
    Serial.println("loaded image");
    for (length_t path = 0; path < num_paths; ++path) {
        Serial.print("  path "); Serial.println(path, DEC);
        for (length_t point = 0; point < paths[path].length; ++point) {
            Serial.print("    (");
            Serial.print(paths[path].points[point].x, DEC);
            Serial.print(", ");
            Serial.print(paths[path].points[point].y, DEC);
            Serial.println(")");
        }
    }
}
