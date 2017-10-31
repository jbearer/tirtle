#pragma once

#include "tirtle/rpc.h"
#include "tirtle/stream.h"

namespace tirtle {
    namespace rpc {

        template<class server>
        void serve(binary_istream & in, binary_ostream & respond, server & s)
        {
            using id = command::command_id;

            command comm;
            in >> comm;
            switch (comm.id) {
            case id::SET_POSITION:
                {
                    const auto & args = comm.args.set_position_args;
                    s.set_position(std::move(args.loc), std::move(args.angle));
                }
                break;
            case id::LOAD_IMAGE:
                {
                    const auto & args = comm.args.load_image_args;
                    s.load_image(std::move(args.img));
                }
                break;
            default:
                respond << invalid_command(comm.id);
                return;
            }

            respond << ok();
        }
    }

}
