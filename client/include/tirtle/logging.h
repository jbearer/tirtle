#pragma once

#include <string>

#include <errno.h>
#include <string.h>

#include "tirtle/stdio_stream.h"

namespace tirtle {
    namespace log {

        inline void debug()
        {
            cerr << "\n";
        }

        template<class Arg, class... Args>
        inline void debug(const Arg & arg, const Args&... args)
        {
            cerr << arg;
            debug(args...);
        }

        inline void info()
        {
            cerr << "\n";
        }

        template<class Arg, class... Args>
        inline void info(const Arg & arg, const Args&... args)
        {
            cerr << arg;
            info(args...);
        }

        inline void error()
        {
            cerr << "\n";
        }

        template<class Arg, class... Args>
        inline void error(const Arg & arg, const Args&... args)
        {
            cerr << arg;
            error(args...);
        }

        inline void perror()
        {
            cerr << ": " << strerror(errno) << "\n";
        }

        template<class Arg, class... Args>
        inline void perror(const Arg & arg, const Args&... args)
        {
            cerr << arg;
            perror(args...);
        }

        [[noreturn]] inline void fatal()
        {
            cerr << "\n";
            std::abort();
        }

        template<class Arg, class... Args>
        [[noreturn]] inline void fatal(const Arg & arg, const Args&... args)
        {
            cerr << arg;
            fatal(args...);
        }

        [[noreturn]] inline void pfatal()
        {
            cerr << ": " << strerror(errno) << "\n";
            std::abort();
        }

        template<class Arg, class... Args>
        [[noreturn]] inline void pfatal(const Arg & arg, const Args&... args)
        {
            cerr << arg;
            pfatal(args...);
        }

    }
}
