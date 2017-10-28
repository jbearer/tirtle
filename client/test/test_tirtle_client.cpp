#include <iostream>
#include <vector>

#include "tirtle/tirtle_client.h"
#include "tirtle/path.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <device>" << '\n';
        return 1;
    }
    tirtle::tirtle_client client(argv[1]);

    std::vector<point_t> path1;
    path1.push_back(make_point(1, 1));
    path1.push_back(make_point(2, 2));

    std::vector<point_t> path2;
    path2.push_back(make_point(1, 2));
    path2.push_back(make_point(2, 1));
    path2.push_back(make_point(0, 0));


    std::vector<path_t> paths;
    paths.push_back(make_path(path1));
    paths.push_back(make_path(path2));
    client.load_image(std::move(paths));

    client.set_position(make_point(2, 3), 45);
}
