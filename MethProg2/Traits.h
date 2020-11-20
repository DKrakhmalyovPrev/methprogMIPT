#pragma once
#include "TypeList.h"

template <typename T, typename U>
class Conversion
{
    using Small = char;
    struct Big { char x[2]; };
    static Small Test(const U&);
    static Big Test(...);
    static T makeT();

public:
    static constexpr bool exists = sizeof(Small) == sizeof(Test(makeT()));
    static constexpr bool equal = false;
};


template <typename T>
class Conversion<T, T>
{
public:
    static constexpr bool exists = true;
    static constexpr bool equal = true;
};

template <bool one, bool two>
struct ANDNOT
{
    static constexpr bool res = false;
};

template <>
struct ANDNOT<true, false>
{
    static constexpr bool res = true;
};

template <typename T, typename U>
struct isBase
{
    static constexpr bool res = ANDNOT< Conversion<T, U>::exists, Conversion<T, U>::equal >::res;
};


template <class T, T v>
struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class Cond, class T, class U>
struct conditional { using type = T; };

template <class T, class U>
struct conditional<false_type, T, U>
{
    using type = U;
};

template <class Cond, class T, class U>
using conditional_t = typename conditional<Cond, T, U>::type;



template <class T, class U>
struct AND
{
    using res = true_type;
};

template <class U>
struct AND< false_type, U>
{
    using res = false_type;
};

template <class T>
struct AND<T, false_type>
{
    using res = false_type;
};



template <class H, class ...T>
struct conjunction
{
    using res = typename conditional_t < H, typename conjunction<T...>::res, false_type>;
};

template <class T>
struct conjunction<T>
{
    using res = T;
};

template < class T> struct remove_reference { using type = T; };
template < class T> struct remove_reference<T&> { using type = T; };
template < class T> struct remove_reference<T&&> { using type = T; };

template < class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class H, class ...T>
struct disjunction
{
    using res = typename conditional_t< H, true_type, typename disjunction<T...>::res>;
};

template <class T>
struct disjunction<T>
{
    using res = T;
};

template <class T, class U>
struct is_same
{
    using res = false_type;
};

template <class T>
struct is_same<T, T>
{
    using res = true_type;
};

template < class T, class U>
using is_same_t = typename is_same<T, U>::res;


template <class T>
struct is_float
    : disjunction<is_same_t<T, float>, is_same_t<T, double>, is_same_t<T, long double>>::res
{};

template < class T>
constexpr bool is_float_v = is_float<T>::value;

template <class T>
true_type detect_poly(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));

template <class T>
false_type detect_poly(...);

template <class T>
struct is_poly : decltype(detect_poly<T>(nullptr)){};

template < class T>
constexpr bool is_poly_t = is_poly<T>::value;