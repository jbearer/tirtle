#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "boost/fusion/include/adapt_struct.hpp"
#include "boost/spirit/home/x3.hpp"

#include "tirtle/tirtle_client.h"
#include "tirtle/path.h"
#include "tirtle/rpc.h"

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using ascii::space;
using x3::_attr;
using x3::_val;
using x3::uint_;

namespace ast {

    struct command
    {
        virtual void eval(tirtle::tirtle_client & client) = 0;
        virtual ~command() {}
    };

    struct set_position
        : command
    {
        tirtle::point loc;
        tirtle::angle_t angle;

        void eval(tirtle::tirtle_client & client) override
        {
            client.set_position(loc, angle);
        }
    };

    struct load_image
        : command
    {
        std::vector<tirtle::path> paths;

        void eval(tirtle::tirtle_client & client) override
        {
            client.load_image(tirtle::image(paths));
        }
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    tirtle::point,
    (tirtle::coord_t, x),
    (tirtle::coord_t, y)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::set_position,
    (tirtle::point, loc)
    (tirtle::angle_t, angle)
);

BOOST_FUSION_ADAPT_STRUCT(
    ast::load_image,
    (std::vector<tirtle::path>, paths)
);

namespace parser {

#define STRINGIFY(x) #x

#define CONCAT(x, y) x ## y

#define RULE(name, attr_t) \
    class name; \
    const x3::rule<name, attr_t> name(STRINGIFY(name)); \
    const auto CONCAT(name, _def)

    RULE(point, tirtle::point) = '(' >> uint_ >> ',' >> uint_ >> ')';

    RULE(angle, tirtle::angle_t) = uint_;

    auto wrap_array = [](auto & cxt) -> void {
        _val(cxt) = tirtle::array<typename std::remove_reference_t<decltype(_attr(cxt))>::value_type>(_attr(cxt));
    };
    RULE(path, tirtle::path) = ('{' >> (point % ',') >> '}')[wrap_array];

    RULE(set_position, ast::set_position) = "set_position" >> ('(' >> point >> ',' >> angle >> ')');

    RULE(load_image, ast::load_image) = "load_image" >> ('(' >> ('{' >> (path % ',') >> '}' >> ')'));

    // Create a pointer to the abstract base class command from a concrete subclass
    auto wrap_ptr = [](auto & cxt) {
        _val(cxt) = std::make_shared<std::remove_reference_t<decltype(_attr(cxt))>>(_attr(cxt));
    };
    RULE(command, std::shared_ptr<ast::command>) = set_position[wrap_ptr]
                                                 | load_image[wrap_ptr];

    BOOST_SPIRIT_DEFINE(point, angle, path, set_position, load_image, command);

#undef STRINGIFY
#undef CONCAT
#undef RULE

}

void eval(const std::string & input, tirtle::tirtle_client & client)
{
    std::shared_ptr<ast::command> comm;
    auto begin = input.begin();
    auto end = input.end();
    bool r = phrase_parse(begin, end, parser::command, space, comm);

    if (r && begin == end) {
        comm->eval(client);
    }
}

void repl(std::unique_ptr<tirtle::tirtle_client> client)
{
    std::string command;
    while (std::cout << ">>> ", std::getline(std::cin, command)) {
        if (command.empty()) {
            continue;
        } else {
            eval(command, *client);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <device>" << '\n';
        return 1;
    }
    auto client = tirtle::connect_tirtle(argv[1]);
    repl(std::move(client));
}
