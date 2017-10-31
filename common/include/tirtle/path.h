#pragma once

#ifndef ARDUINO
#include <algorithm>
#include <vector>
#endif

#include <stdint.h>

#include "tirtle/move.h"
#include "tirtle/stream.h"

namespace tirtle {

    using coord_t = uint32_t;
    using length_t = uint16_t;
    using angle_t = uint16_t;

    // Point in the first qudrant of 2D cartesian space
    struct point {

        point();
        point(coord_t x_, coord_t y_);

        bool operator==(const point & that) const;
        bool operator!=(const point & that) const;

        point operator+(const point & that) const;

        coord_t x;
        coord_t y;
    };

    binary_istream & operator>>(binary_istream & in, point & p);
    binary_ostream & operator<<(binary_ostream & out, const point & p);

    text_ostream & operator<<(text_ostream & out, const point & loc);

    template<class T>
    struct array {

        using value_type = T;
        using iterator = T *;
        using const_iterator = const T *;
        using reference = T &;
        using const_reference = const T &;
        using pointer = T *;
        using const_pointer = const T *;

        array()
            : n(0)
            , elems(nullptr)
        {}

        explicit array(length_t n_)
            : n(n_)
            , elems(new T[n])
        {}

        array(length_t n_, const T & model)
            : array(n_)
        {
            for (auto & p : *this) {
                p = model;
            }
        }

#ifndef ARDUINO
        array(const std::vector<T> & elems_)
            : array(elems_.size())
        {
            std::copy(elems_.begin(), elems_.end(), begin());
        }
#endif

        array(const array & that)
            : array(that.length())
        {
            for (length_t i = 0; i < that.length(); ++i) {
                (*this)[i] = that[i];
            }
        }

        array(array && that)
            : n(that.n)
            , elems(that.elems)
        {
            that.n = 0;
            that.elems = nullptr;
        }

        array & operator=(const array & that)
        {
            if (length() < that.length()) {
                delete[] elems;
                elems = new T[that.length()];
            }
            n = that.n;
            for (length_t i = 0; i < that.length(); ++i) {
                (*this)[i] = that[i];
            }

            return *this;
        }

        array & operator=(array && that)
        {
            if (elems) delete[] elems;
            n = that.n;
            elems = that.elems;

            that.n = 0;
            that.elems = nullptr;

            return *this;
        }

        bool operator==(const array & that)
        {
            if (length() != that.length()) return false;

            for (length_t i = 0; i < length(); ++i) {
                if ((*this)[i] != that[i]) return false;
            }

            return true;
        }

        bool operator!=(const array & that)
        {
            return !(*this == that);
        }

        bool empty() const
        {
            return length() == 0;
        }

        length_t length() const
        {
            return n;
        }

        iterator begin()
        {
            return elems;
        }

        const_iterator begin() const
        {
            return elems;
        }

        iterator end()
        {
            return elems + n;
        }

        const_iterator end() const
        {
            return elems + n;
        }

        reference operator[](length_t i)
        {
            return elems[i];
        }

        const_reference operator[](length_t i) const
        {
            return elems[i];
        }

        pointer data()
        {
            return elems;
        }

        const_pointer data() const
        {
            return elems;
        }

        ~array()
        {
            if (elems) {
                delete[] elems;
                elems = nullptr;
            }
        }

    private:
        length_t    n;
        T           *elems;
    };

    template<class T>
    inline binary_istream & operator>>(binary_istream & in, array<T> & out)
    {
        length_t n = 0;
        in >> n;

        array<T> arr(n);
        for (auto & elem : arr) {
            in >> elem;
        }

        out = std::move(arr);

        return in;
    }

    template<class T>
    inline binary_ostream & operator<<(binary_ostream & out, const array<T> & arr)
    {
        out << arr.length();
        for (const auto & elem : arr) {
            out << elem;
        }
        return out;
    }

    template<class T>
    inline text_ostream & operator<<(text_ostream & out, const array<T> & arr)
    {
        out << "{";
        if (!arr.empty()) {
            out << arr[0];
            for (auto it = arr.begin() + 1; it < arr.end(); ++it) {
                out << ", " << *it;
            }
        }
        return out << "}";
    }

    using path = array<point>;
    using image = array<path>;

} // namespace tirtle
