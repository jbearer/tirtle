#pragma once

#include "tirtle/path.h"
#include "tirtle/stream.h"

namespace tirtle {
    namespace rpc {

        struct set_position
        {
            point       loc;
            angle_t     angle;

            set_position();
            set_position(point, angle_t);
        };

        struct load_image
        {
            image img;

            load_image();
            load_image(image);
        };

        struct command
        {
            using id_underlying_type = uint8_t;
            enum command_id : id_underlying_type
            {
                SET_POSITION,
                LOAD_IMAGE,
                INVALID
            };

            struct command_args // should be union, but load_image has non-trivial destructor
            {
                set_position    set_position_args;
                load_image      load_image_args;
            };

            command_id      id;
            command_args    args;

            command();
            command(set_position);
            command(load_image);
        };

        binary_ostream & operator<<(binary_ostream &, const command &);
        binary_istream & operator>>(binary_istream &, command &);

        struct ok {};

        struct invalid_command
        {
            command::command_id     id;

            invalid_command();
            explicit invalid_command(command::command_id);
        };

        struct response
        {
            using id_underlying_type = uint8_t;
            enum response_id : id_underlying_type
            {
                OK,
                INVALID_COMMAND,
                INVALID
            };

            struct response_args // should be union, but load_image has non-trivial destructor
            {
                ok                  ok_args;
                invalid_command     invalid_command_args;
            };

            response_id     id;
            response_args   args;

            response();
            response(ok);
            response(invalid_command);
        };

        binary_ostream & operator<<(binary_ostream &, const response &);
        binary_istream & operator>>(binary_istream &, response &);

    } // namespace rpc
} // namespace tirtle
