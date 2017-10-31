#include <iostream>
#include <memory>
#include <vector>

#include "tirtle/logging.h"
#include "tirtle/path.h"
#include "tirtle/tirtle.h"
#include "tirtle/tirtle_client.h"

using namespace tirtle;

struct mock_client
    : tirtle_client
{
    image img;

    mock_client() = default;

    void load_image(const image & img_) override
    {
        img = img_;
    }

    void set_position(const point &, angle_t) override
    {
        log::fatal("mock_client::set_position is not implemented");
    }
};

static void test(tirtle::tirtle & tirtle, const image & img)
{
    auto client = std::make_unique<mock_client>();
    tirtle.draw(*client);
    if (client->img != img) {
        log::fatal("error:\nexpected: \n", img, "\nbut got: \n", client->img);
    }
}

static void test1()
{
    tirtle::log::info("begin test1");

    tirtle::tirtle tirtle;
    tirtle.forward(1);
    tirtle.left(90);
    tirtle.forward(1);
    tirtle.left(90);
    tirtle.forward(1);
    tirtle.left(90);
    tirtle.forward(1);

    path path1(5);
    path1[0] = start_loc + point(0, 0);
    path1[1] = start_loc + point(1, 0);
    path1[2] = start_loc + point(1, 1);
    path1[3] = start_loc + point(0, 1);
    path1[4] = start_loc + point(0, 0);

    image img(1);
    img[0] = std::move(path1);

    test(tirtle, std::move(img));
}

static void test2()
{
    tirtle::log::info("begin test2");

    tirtle::tirtle tirtle;
    tirtle.left(45);
    tirtle.forward(7); // Hypotenuse of 5x5 right isoceles

    tirtle.pen_up();
    tirtle.right(135);
    tirtle.forward(5);
    tirtle.right(90);
    tirtle.pen_down();

    tirtle.forward(5);

    path path1(2);
    path1[0] = start_loc + point(0, 0);
    path1[1] = start_loc + point(5, 5);

    path path2(2);
    path2[0] = start_loc + point(5, 0);
    path2[1] = start_loc + point(0, 0);

    image img(2);
    img[0] = std::move(path1);
    img[1] = std::move(path2);

    test(tirtle, std::move(img));
}

int main()
{
    test1();
    test2();
}
