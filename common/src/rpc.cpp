#include "tirtle/move.h"
#include "tirtle/path.h"
#include "tirtle/rpc.h"
#include "tirtle/stream.h"

using namespace tirtle;
using namespace rpc;

set_position::set_position() = default;

set_position::set_position(point loc_, angle_t angle_)
    : loc(std::move(loc_))
    , angle(std::move(angle_))
{}

namespace tirtle {
    namespace rpc {
        binary_ostream & operator<<(binary_ostream & out, const set_position & p)
        {
            return out << p.loc << p.angle;
        }

        binary_istream & operator>>(binary_istream & in, set_position & p)
        {
            return in >> p.loc >> p.angle;
        }
    }
}

load_image::load_image() = default;

load_image::load_image(image img_)
    : img(std::move(img_))
{}

namespace tirtle {
    namespace rpc {

        binary_ostream & operator<<(binary_ostream & out, const load_image & l)
        {
            return out << l.img;
        }

        binary_istream & operator>>(binary_istream & in , load_image & l)
        {
            return in >> l.img;
        }

    }
}

command::command()
    : id(INVALID)
{}

command::command(set_position p)
    : id(SET_POSITION)
{
    args.set_position_args = std::move(p);
}

command::command(load_image l)
    : id(LOAD_IMAGE)
{
    args.load_image_args = std::move(l);
}

namespace tirtle {
    namespace rpc {

        binary_ostream & operator<<(binary_ostream & out , command::command_id id)
        {
            return out << static_cast<command::id_underlying_type>(id);
        }

        binary_istream & operator>>(binary_istream & in, command::command_id & id)
        {
            command::id_underlying_type prim_id = 1;
            in >> prim_id;
            id = static_cast<command::command_id>(prim_id);
            return in;
        }

    }
}

binary_ostream & tirtle::rpc::operator<<(binary_ostream & out, const command & comm)
{
    switch (comm.id) {
    case command::command_id::SET_POSITION:
        out << comm.id << comm.args.set_position_args;
        break;
    case command::command_id::LOAD_IMAGE:
        out << comm.id << comm.args.load_image_args;
        break;
    default:
        // TODO make logging better so we can log on arduino
        break;
    }
    return out;
}

binary_istream & tirtle::rpc::operator>>(binary_istream & in, command & comm)
{
    in >> comm.id;
    switch (comm.id) {
    case command::command_id::SET_POSITION:
        in >> comm.args.set_position_args;
        break;
    case command::command_id::LOAD_IMAGE:
        in >> comm.args.load_image_args;
        break;
    default:
        comm.id = command::command_id::INVALID;
        break;
    }
    return in;
}

namespace tirtle {
    namespace rpc {

        binary_ostream & operator<<(binary_ostream & out, const ok &)
        {
            return out;
        }

        binary_istream & operator>>(binary_istream & in, ok &)
        {
            return in;
        }

    }
}

invalid_command::invalid_command()
    : id(command::command_id::INVALID)
{}

invalid_command::invalid_command(command::command_id id_)
    : id(id_)
{}

namespace tirtle {
    namespace rpc {

        binary_ostream & operator<<(binary_ostream & out, const invalid_command & ic)
        {
            return out << ic.id;
        }

        binary_istream & operator>>(binary_istream & in , invalid_command & ic)
        {
            return in >> ic.id;
        }

    }
}

response::response()
    : id(INVALID)
{}

response::response(ok k)
    : id(OK)
{
    args.ok_args = std::move(k);
}

response::response(invalid_command ic)
    : id(INVALID_COMMAND)
{
    args.invalid_command_args = std::move(ic);
}

namespace tirtle {
    namespace rpc {

        binary_ostream & operator<<(binary_ostream & out, response::response_id id)
        {
            return out << static_cast<response::id_underlying_type>(id);
        }

        binary_istream & operator>>(binary_istream & in, response::response_id & id)
        {
            response::id_underlying_type prim_id = 0;
            in >> prim_id;
            id = static_cast<response::response_id>(prim_id);
            return in;
        }

    }
}

binary_ostream & tirtle::rpc::operator<<(binary_ostream & out, const response & res)
{
    switch (res.id) {
    case response::response_id::OK:
        out << res.id << res.args.ok_args;
        break;
    case response::response_id::INVALID_COMMAND:
        out << res.id << res.args.invalid_command_args;
        break;
    default:
        // TODO make logging better so we can log on arduino
        break;
    }
    return out;
}

binary_istream & tirtle::rpc::operator>>(binary_istream & in, response & res)
{
    in >> res.id;
    switch(res.id) {
    case response::response_id::OK:
        in >> res.args.ok_args;
        break;
    case response::response_id::INVALID_COMMAND:
        in >> res.args.invalid_command_args;
        break;
    default:
        res.id = response::response_id::INVALID;
        break;
    }
    return in;
}
