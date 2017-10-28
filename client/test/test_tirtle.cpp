#include <iostream>
#include <memory>
#include <vector>

#include "tirtle/logging.h"
#include "tirtle/path.h"
#include "tirtle/tirtle.h"
#include "tirtle/tirtle_client.h"

struct mock_client
    : tirtle::tirtle_client
{
    std::vector<path_t> img;

    mock_client() = default;

    void load_image(const std::vector<path_t> & paths) override
    {
        img = paths;
    }

    void set_position(point_t, angle_t) override
    {
        tirtle::log::fatal("mock_client::set_position is not implemented");
    }
};

static std::ostream & operator<<(std::ostream & out, const std::vector<path_t> & paths)
{
    for (size_t path = 0; path < paths.size(); ++path) {
        out << "path " << path << '\n';
        for (size_t point = 0; point < paths[path].length; ++point) {
            out << "  " << paths[path].points[point] << '\n';
        }
    }
    return out;
}

static void test(tirtle::tirtle & tirtle, const std::vector<path_t> & paths)
{
    auto client = std::make_unique<mock_client>();
    tirtle.draw(*client);
    if (client->img != paths) {
        tirtle::log::fatal("error:\nexpected: \n", paths, "\nbut got: \n", client->img);
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

    std::vector<point_t> path1;
    path1.push_back(make_point(0, 0));
    path1.push_back(make_point(1, 0));
    path1.push_back(make_point(1, 1));
    path1.push_back(make_point(0, 1));
    path1.push_back(make_point(0, 0));

    std::vector<path_t> paths;
    paths.push_back(make_path(path1));

    test(tirtle, paths);
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

    std::vector<point_t> path1;
    path1.push_back(make_point(0, 0));
    path1.push_back(make_point(5, 5));

    std::vector<point_t> path2;
    path2.push_back(make_point(5, 0));
    path2.push_back(make_point(0, 0));

    std::vector<path_t> paths;
    paths.push_back(make_path(path1));
    paths.push_back(make_path(path2));

    test(tirtle, paths);
}

int main()
{
    test1();
    test2();
}
