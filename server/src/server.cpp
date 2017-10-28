#include <SoftwareSerial.h>

#include "tirtle/motor_control.h"
#include "tirtle/path.h"
#include "tirtle/rpc.h"
#include "tirtle/rpc_parser.h"

#define MAX_PATHS 100
#define MAX_POINTS 100

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
              path_t *paths;
              length_t length;
              parse_load_image(BT, paths, length);
              load_image(paths, length);
            }
            break;
        case SET_POSITION:
            {
              point_t pos;
              angle_t angle;
              parse_set_position(BT, pos, angle);
              set_position(pos, angle);
            }
            break;
        default:
            Serial.print("unrecognized rpc command: ");
            Serial.println(comm);
        }
    }
}
