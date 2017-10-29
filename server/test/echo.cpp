#include <Arduino.h>
#include <SoftwareSerial.h>

#include "tirtle/path.h"
#include "tirtle/rpc.h"
#include "tirtle/rpc_parser.h"

#define RX 0
#define TX 1

SoftwareSerial BT(RX, TX);

void setup()
{
    Serial.begin(9600);
    BT.begin(9600);
}

void loop()
{
    if (BT.available())
    {
        rpc_command comm = BT.read();
        switch (comm) {
        case LOAD_IMAGE:
            {
                Serial.println("received rpc command: LOAD_IMAGE");
                path_t *paths;
                length_t length;
                parse_load_image(BT, paths, length);
                for (length_t path = 0; path < length; ++path) {
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
            break;
        case SET_POSITION:
            {
                Serial.println("received rpc command: SET_POSITION");
                point_t pos;
                angle_t angle;
                parse_set_position(BT, pos, angle);
                Serial.print("  x     = "); Serial.println(pos.x, DEC);
                Serial.print("  y     = "); Serial.println(pos.y, DEC);
                Serial.print("  theta = "); Serial.println(angle, DEC);
            }
            break;
        default:
            Serial.print("unrecognized rpc command: ");
            Serial.println(comm);
        }
    }
}
