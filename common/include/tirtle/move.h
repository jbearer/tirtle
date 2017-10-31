#pragma once

#ifdef ARDUINO

namespace std {

    template<class T>
    struct remove_reference
    {
        using type = T;
    };

    template<class T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template<class T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template <class T>
    typename remove_reference<T>::type && move(T && arg)
    {
      return static_cast<typename remove_reference<T>::type &&>(arg);
    }

}

#else

#include <utility>

#endif
