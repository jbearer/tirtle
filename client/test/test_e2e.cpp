#include <chrono>
#include <memory>
#include <thread>

#include "tirtle/tirtle.h"
#include "tirtle/tirtle_client.h"
#include "tirtle/tracker.h"

int main()
{
    auto client = tirtle::connect_tirtle("HC-06");
    auto tracker = std::make_unique<tirtle::tracker>(*client);

    tirtle::tirtle tirtle;
    tirtle.forward(50); // (1, 0)
    tirtle.left(90);
    tirtle.forward(50); // (1, 1)
    tirtle.left(90);
    tirtle.forward(50); // (0, 1)
    tirtle.left(90);
    tirtle.forward(50); // (0, 0)

    tirtle.draw(*client);

    std::this_thread::sleep_for(std::chrono::seconds(60));
}
