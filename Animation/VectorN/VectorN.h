#pragma once
#include <initializer_list>
#include <utility>
#include <tuple>

using namespace std;

template<class T, size_t N>
struct SVectorN
{
    SVectorN(const T (&another)[N])
    {
        copy(another, another + N, components);
    };

    template<typename U>
    SVectorN(const U&& u)
        : components{ u }
    {
    };

    template<typename Arg1, typename Arg2, typename... Args>
    SVectorN(Arg1&& arg1, Arg2&& arg2, Args&&... args)
        : components{forward<Arg1>(arg1), forward<Arg2>(arg2), forward<Args>(args)...}
    {
    }

private:
    size_t Size{N};
    T components[N] = {};
};


template<class T, size_t N> SVectorN(const T(&)[N]) -> SVectorN<T, N>;
template<typename U> SVectorN(const U&& u) -> SVectorN<U, 1>;
template<typename Arg1, typename Arg2, typename... Args> SVectorN(Arg1&& arg1, Arg2&& arg2, Args&&... args)-> SVectorN<Arg1, 2 + sizeof...(Args)>;