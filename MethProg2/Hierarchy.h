#pragma once
#include "TypeList.h"
#include "Traits.h"
template <typename T>
struct Holder
{
    T _obj;
    size_t sz = sizeof(T);
};

template <typename T, typename Base>
struct DerHolder :
    public Base
{
    T _obj;
};

//...................................................................//

template <class TList, class T>
struct MostDerived;

template <class T>
struct MostDerived<Nulltype, T>
{
    using res = T;
};

template <class ...TList, class T>
struct MostDerived<TypeList<TList...>, T>
{
    using Cand = typename MostDerived<typename TypeList<TList...>::Tail, T>::res;

    using res = typename Select<isBase<typename TypeList<TList...>::Head, Cand>::res,
        typename TypeList<TList...>::Head, Cand>::res;
};

template<class T> struct DerivedToFront
{
    using res = Nulltype;
};

template<>
struct DerivedToFront<TypeList<Nulltype>>
{
    using res = Nulltype;
};

template <typename H>
struct DerivedToFront<TypeList<H>>
{
    using res = H;
};

template <typename ...TList>
struct DerivedToFront<TypeList<TList...>>
{
private:

    using TheMostDerived = typename MostDerived<typename TypeList<TList...>::Tail, typename TypeList<TList...>::Head>::res;

    using Temp = typename Replace< typename TypeList<TList...>::Tail, TheMostDerived, typename TypeList<TList...>::Head >::res;

    using L = typename DerivedToFront<Temp>::res;

public:
    using res = TypeList<TheMostDerived, L>;

};

//...................................................................//

template <class TList, template <class> class Unit>
struct GenScatterHierarchy;

template <class ...TList, template <class> class Unit>
struct GenScatterHierarchy<TypeList<TList...>, Unit> :
    public Unit<typename TypeList<TList...>::Head>,
    public GenScatterHierarchy<typename TypeList<TList...>::Tail, Unit>
{
    using TL = TypeList<TList...>;
    using LeftBase = Unit<typename TypeList<TList...>::Head>;
    using RightBase = GenScatterHierarchy<typename TypeList<TList...>::Tail, Unit>;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template <class T, template <class> class Unit>
struct GenScatterHierarchy<TypeList<T>, Unit> :
    public Unit<T>
{
    using LeftBase = T;
    using RightBase = Nulltype;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template <template <class> class Unit>
struct GenScatterHierarchy<TypeList<Nulltype>, Unit>
{
    using LeftBase = Nulltype;
    using RightBase = Nulltype;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

//...................................................................//
template<class TList, template <class T, class Base> class Unit, class Root = Nulltype>
struct GenLinearHierarchy;

template<class ...TList, template <class T, class Base> class Unit, class Root>
struct GenLinearHierarchy<TypeList<TList...>, Unit, Root> :
    public Unit< typename TypeList<TList...>::Head, GenLinearHierarchy<typename TypeList<TList...>::Tail, Unit, Root>>
{

};

template<class TList, template <class T, class Base> class Unit, class Root>
struct GenLinearHierarchy<TypeList<TList>, Unit, Root> :
    public Unit<TList, Root>
{

};

template <class H, unsigned int i> struct FieldHelper;

template <class H>
struct FieldHelper<H, 0>
{
    typedef typename H::TL::Head ElementType;
    typedef typename H::template Rebind<ElementType>::Result UnitType;

    static UnitType& Do(H& obj)
    {
        UnitType& leftBase = obj;
        return leftBase;
    }
};

template <class H, unsigned int i>
struct FieldHelper
{
    typedef typename TypeAt<i, typename H::TList>::Result ElementType;
    typedef typename H::template Rebind<ElementType>::Result UnitType;

    typedef typename H::RightBase RightBase;

    static UnitType& Do(H& obj)
    {
        RightBase& rightBase = obj;
        return FieldHelper<RightBase, i - 1>::Do(rightBase);
    }
};

template <int i, class H>
typename FieldHelper<H, i>::UnitType&
Field(H& obj)
{
    return FieldHelper<H, i>::Do(obj);
}
