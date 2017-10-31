#include <Arduino.h>

#include "tirtle/bluetooth.h"
#include "tirtle/path.h"
#include "tirtle/robot.h"
#include "tirtle/rpc_server.h"

#define RX 0
#define TX 1

using namespace tirtle;

tirtle::bluetooth_stream bt(RX, TX);
tirtle::robot            bot;

void setup() {}

void loop()
{
    if (bt.available()) {
        rpc::serve(bt, bt, bot);
    }
    bot.step();
}
