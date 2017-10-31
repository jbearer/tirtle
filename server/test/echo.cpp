#include <Arduino.h>
#include <SoftwareSerial.h>

#include "tirtle/bluetooth.h"
#include "tirtle/path.h"
#include "tirtle/rpc_server.h"
#include "tirtle/serial.h"
#include "tirtle/stream.h"

using namespace tirtle;

#define RX 0
#define TX 1

struct echo_server
{
    void load_image(const image & img) const
    {
        serial::out << "load_image(" << img << ")" << endl;
    }

    void set_position(const point & loc, angle_t theta) const
    {
        serial::out << "set_position(" << loc << ", " << theta << ")" << endl;
    }
};

bluetooth_stream bt(RX, TX);
echo_server server;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (bt.available()) rpc::serve(bt, bt, server);
}
