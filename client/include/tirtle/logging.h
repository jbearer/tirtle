#pragma once

#include <string>

#include <errno.h>

namespace tirtle {
    namespace log {

        inline void debug()
        {
            std::cerr << "\n";
        }

        template<class Arg, class... Args>
        inline void debug(const Arg & arg, const Args&... args)
        {
            std::cerr << arg;
            debug(args...);
        }

        inline void info()
        {
            std::cerr << "\n";
        }

        template<class Arg, class... Args>
        inline void info(const Arg & arg, const Args&... args)
        {
            std::cerr << arg;
            info(args...);
        }

        inline void error()
        {
            std::cerr << "\n";
        }

        template<class Arg, class... Args>
        inline void error(const Arg & arg, const Args&... args)
        {
            std::cerr << arg;
            error(args...);
        }

        inline void perror()
        {
            std::cerr << ": " << strerror(errno) << "\n";
        }

        template<class Arg, class... Args>
        inline void perror(const Arg & arg, const Args&... args)
        {
            std::cerr << arg;
            perror(args...);
        }

        [[noreturn]] inline void fatal()
        {
            std::cerr << "\n";
            std::abort();
        }

        template<class Arg, class... Args>
        [[noreturn]] inline void fatal(const Arg & arg, const Args&... args)
        {
            std::cerr << arg;
            fatal(args...);
        }

        [[noreturn]] inline void pfatal()
        {
            std::cerr << ": " << strerror(errno) << "\n";
            std::abort();
        }

        template<class Arg, class... Args>
        [[noreturn]] inline void pfatal(const Arg & arg, const Args&... args)
        {
            std::cerr << arg;
            pfatal(args...);
        }

    }
}
