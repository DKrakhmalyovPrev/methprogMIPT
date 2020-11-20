#pragma once




class Nulltype {};

template <typename ...Args>
struct TypeList
{
    using Head = Nulltype;
    using Tail = Nulltype;
};

typedef TypeList<> EmptyTypeList;

template <typename H, typename ...T>
struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};


template <typename H>
struct TypeList<H>
{
    using Head = H;
    using Tail = EmptyTypeList;
};



//...................................................................//


template <typename T>
struct Length
{
    static const int value = 1 + Length<T::Tail>::value;
};

template <>
struct Length<Nulltype>
{
    static const int value = 0;
};

//...................................................................//

template <size_t ind, typename ...T>
struct TypeAt {
    using res = Nulltype;
};

template <size_t ind, typename ...T>
struct TypeAt<ind, TypeList<T...>>
{
    using res = typename TypeAt<ind - 1, typename TypeList<T...>::Tail>::res;
};

template <size_t ind>
struct TypeAt<ind, Nulltype>
{
    using res = Nulltype;
};

template <typename ...T>
struct TypeAt<0, TypeList<T...>>
{
    using res = typename TypeList<T...>::Head;
};

//...................................................................//

template <typename U, typename ...T>
struct IndexOf
{
    static const size_t value = 1;
};

template <typename ...K, typename U>
struct IndexOf<U, TypeList<K...>>
{
    static const size_t value = 1 + IndexOf<U, typename TypeList<K...>::Tail>::value;
};

template <typename T, typename ...K>
struct IndexOf<TypeList<T, K...>, T>
{
    static const size_t value = 0;
};

//...................................................................//

template < typename U, size_t ind, typename ...T>
struct Add;


template < typename U, size_t ind, typename ...K>
struct Add<U, ind, TypeList<K...> >
{
    using result = TypeList<typename TypeList<K...>::Head, typename Add< U, ind - 1, typename TypeList<K...>::Tail>::result>;
};

template <typename U, typename T>
struct Add<U, 0, T>
{
    using result = TypeList <U, T>;
};

template <typename U, size_t ind>
struct Add<U, ind, TypeList<Nulltype>>
{
    using result = U;
};

//...................................................................//

template <typename U, typename ...T>
struct Remove;

template <typename U, typename ...T>
struct Remove<U, TypeList<T...>>
{
    using result = TypeList<typename TypeList<T...>::Head, typename Remove<U, typename TypeList<T...>::Tail>::result>;
};

template <typename T, typename ...K>
struct Remove<T, TypeList<T, K...>>
{
    using result = typename Remove<T, typename TypeList<T, K...>::Tail>::result;
};

template <typename T>
struct Remove< T, EmptyTypeList>
{
    using result = Nulltype;
};

//...................................................................//

template <class T, class U, class V> struct Replace;

//template<class U, class V>
//struct Replace<Nulltype, U, V>
//{
//    using res = Nulltype;
//};

template <class U, class V>
struct Replace<TypeList<U>, U, V>
{
    using res = TypeList<V>;
};

template <class T, class U, class V>
struct Replace<TypeList<T>, U, V>
{
    using res = TypeList<T>;
};

template <class ...T, class U, class V>
struct Replace<TypeList<U, T...>, U, V>
{
    using res = TypeList<V, T...>;
};

template <class ...T, class U, class V>
struct Replace<TypeList<T...>, U, V>
{
    using res = TypeList< typename TypeList<T...>::Head,
        typename Replace<typename TypeList<T...>::Tail, U, V>::res >;
};



//...................................................................//

template<bool is, typename T, typename U> struct Select;

template<typename T, typename U>
struct Select<true, T, U>
{
    using res = T;
};

template<typename T, typename U>
struct Select<false, T, U>
{
    using res = U;
};

//...................................................................//