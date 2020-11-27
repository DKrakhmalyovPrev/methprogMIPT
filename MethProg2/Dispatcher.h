#pragma once
#include <iostream>
#include "TypeList.h"
#include <unordered_map>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <map>


class Animal
{
    virtual void voice() = 0;
};

class Dog
    :public virtual Animal
{
    virtual void voice() override
    {
        std::cout << "ֳאג" << std::endl;
    }
};

class Cat
    :public virtual Animal
{
    virtual void voice() override
    {
        std::cout << "ּÿף" << std::endl;
    }
};

class CatDog
    :public virtual Cat,
    public virtual Dog
{
    virtual void voice() override
    {
        std::cout << "ּÿףֳאג" << std::endl;
    }
};

class Meeting
{
public:
    static void meet(Dog* one, Dog* two)
    {
        std::cout << "Bark Bark" << std::endl;
    }

    static void meet(Dog* one, Cat* two)
    {
        std::cout << "Bark Meow" << std::endl;
    }

    static void meet(Cat* one, Dog* two)
    {
        std::cout << "Bark Meow" << std::endl;
    }

    static void meet(Cat* one, Cat* two)
    {
        std::cout << "Meow Meow" << std::endl;
    }

    static void meet(Dog* one, CatDog* two)
    {
        std::cout << "Who are you?" << std::endl;
    }

    static void meet(CatDog* one, Dog* two)
    {
        std::cout << "Who are you?" << std::endl;
    }
    static void meet(Cat* one, CatDog* two)
    {
        std::cout << "Who are you?" << std::endl;
    }

    static void meet(CatDog* one, Cat* two)
    {
        std::cout << "Who are you?" << std::endl;
    }

    static void meet(CatDog* one, CatDog* two)
    {
        std::cout << "Who are you?" << std::endl;
    }


    static void meet(...)
    {
        std::cout << "Something bad happend" << std::endl;
    }
};

template
<
    class Executor,
    class BaseLhs,
    class TypesLhs,
    class BaseRhs = BaseLhs,
    class TypesRhs = TypesLhs,
    typename ResultType = void
>
class StaticDispatcher
{
    typedef typename TypesLhs::Head Head;
    typedef typename TypesLhs::Tail Tail;
public:
    static ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
    {
        if (typeid(Head) == typeid(*lhs))
        {
            Head* p1 = dynamic_cast<Head*>(lhs);
            return StaticDispatcher<Executor, BaseLhs,
                TypesLhs, BaseRhs, TypesRhs, ResultType >::DispatchRhs(
                    p1, rhs);
        }
        else
        {
            return StaticDispatcher<Executor, BaseLhs,
                Tail, BaseRhs, TypesRhs, ResultType >::Go(
                    lhs, rhs);
        }
    }
    template <class SomeLhs>
    static ResultType DispatchRhs(SomeLhs* lhs, BaseRhs* rhs)
    {
        typedef typename TypesRhs::Head Head;
        typedef typename TypesRhs::Tail Tail;
        if (typeid(Head) == typeid(*rhs))
        {
            Head* p2 = dynamic_cast<Head*>(rhs);
            return Executor::meet(lhs, p2);
        }
        else
        {
            return StaticDispatcher<Executor, SomeLhs,
                TypesLhs, BaseRhs, Tail, ResultType >::DispatchRhs(
                    lhs, rhs);
        }
    }
};

template
<
    class Executor,
    class BaseLhs,
    class BaseRhs,
    class TypesRhs,
    typename ResultType
>
class StaticDispatcher< Executor, BaseLhs, Nulltype, BaseRhs, TypesRhs, ResultType>
{
public:
    static ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
    {
        return ResultType();
    }

    template <class SomeLhs>
    static ResultType DispatchRhs(SomeLhs* lhs, BaseRhs* rhs)
    {
        return ResultType();
    }
};

template
<
    class Executor,
    class BaseLhs,
    class TypesLhs,
    class BaseRhs,
    typename ResultType
>
class StaticDispatcher< Executor, BaseLhs, TypesLhs, BaseRhs, Nulltype, ResultType>
{
    typedef typename TypesLhs::Head Head;
    typedef typename TypesLhs::Tail Tail;
public:
    static ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
    {
        return ResultType();
    }
    template <class SomeLhs>
    static ResultType DispatchRhs(SomeLhs* lhs, BaseRhs* rhs)
    {
        return ResultType();
    }
};


void MeetDog(Animal* one, Animal* two)
{
    std::cout << "Bark Bark" << std::endl;
}

void MeetCatDog(Animal* one, Animal* two)
{
    std::cout << "Meow Bark" << std::endl;
}

void MeetCat(Animal* one, Animal* two)
{
    std::cout << "Meow Meow" << std::endl;
}


struct HashIndex
{
    size_t operator() (const std::pair<std::type_index, std::type_index>& pr) const
    {
        static std::hash<std::type_index> z;

        return z(pr.first) * 31 + z(pr.second);
    }
};

template
<
    class BaseLhs,
    class BaseRhs,
    typename ResultType = void,
    typename CallbackType = ResultType (*)(BaseLhs*, BaseRhs*)
>
class BasicDispatcher
{
    using KeyType = std::pair<std::type_index, std::type_index>;
    using MappedType = CallbackType;
    using MapType = std::unordered_map<KeyType, MappedType, HashIndex>;

    MapType callbackMap;

public:

    template <class TypeLhs, class TypeRhs>
    void add(CallbackType fun)
    {
        callbackMap.emplace(KeyType(typeid(TypeLhs), typeid(TypeRhs)), fun);
    }

    template <class TypeLhs, class TypeRhs>
    void remove()
    {
        callbackMap.erase(KeyType(typeid(TypeLhs), typeid(TypeRhs)));
    }

    ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
    {
        KeyType k(typeid(*lhs), typeid(*rhs));
        typename MapType::iterator it = callbackMap.find(k);
        if (it == callbackMap.end())
        {
            std::cerr << "Error. Types not found";
        }

        return (it->second)(lhs, rhs);
    }
};