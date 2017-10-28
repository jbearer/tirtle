#include <Arduino.h>
#include <SoftwareSerial.h>

template<class T>
void parse(SoftwareSerial & BT, T & out)
{
    while ((size_t)BT.available() < sizeof(T)) {}
    BT.readBytes((char*)&out, sizeof(T));
}

template<>
void parse<point_t>(SoftwareSerial & BT, point_t & point)
{
    parse<coord_t>(BT, point.x);
    parse<coord_t>(BT, point.y);
}

template<>
void parse<path_t>(SoftwareSerial & BT, path_t & path)
{
    parse<length_t>(BT, path.length);
    path.points = (point_t *)malloc(path.length * sizeof(path.points[0]));
    if (!path.points) {
        Serial.println("out of memory");
        exit(1);
    }
    for (length_t i = 0; i < path.length; ++i) {
        parse<point_t>(BT, path.points[i]);
    }
}

// Parse the arguments of a load_image command
void parse_load_image(SoftwareSerial & BT, path_t *& paths, length_t & length)
{
    parse<length_t>(BT, length);

    paths = (path_t *)malloc(length * sizeof(paths[0]));
    if (!paths) {
        Serial.println("out of memory");
        exit(1);
    }
    for (length_t i = 0; i < length; ++i) {
        parse<path_t>(BT, paths[i]);
    }
}

// Parse the arguments of a set_position command
void parse_set_position(SoftwareSerial & BT, point_t & pos, angle_t & angle)
{
    parse<point_t>(BT, pos);
    parse<angle_t>(BT, angle);
}
