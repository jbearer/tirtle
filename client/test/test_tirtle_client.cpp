#include <iostream>
#include <memory>
#include <vector>

#include "tirtle/tirtle_client.h"
#include "tirtle/path.h"

void prompt()
{
    std::cout << "> ";
}

void prompt(const std::string & str)
{
    prompt();
    std::cout << str << ' ';
}

template<class T>
void consume(T * res)
{
    if (!(std::cin >> *res)) {
        std::cerr << "unexpected end of input\n";
        std::abort();
    }
}

void consume(char c)
{
    char input;
    consume(&input);
    if (c != input) {
        std::cerr << "unexpected input '" << input << "': expected '" << c << "'\n";
        std::abort();
    }
}

void prompt_set_position(point_t & loc, angle_t & angle)
{
    prompt("x     =");
    consume(&loc.x);

    prompt("y     =");
    consume(&loc.y);

    prompt("theta =");
    consume(&angle);
}

void repl(std::unique_ptr<tirtle::tirtle_client> client)
{
    std::string command;
    while (prompt(), std::getline(std::cin, command)) {
        if (command.empty()) {
            continue;
        } else if (command == "set_position") {
            point_t loc;
            angle_t angle;
            prompt_set_position(loc, angle);
            client->set_position(loc, angle);
        } else {
            std::cerr << "unrecognized command \"" << command << "\"\n";
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
