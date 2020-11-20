#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <array>
#include "ABclasses.h"
#include "TypeList.h"
#include "Traits.h"
#include "Hierarchy.h"
#include "Dispatcher.h"


int main()
{
    //TypeList<int, int, float, A> a;

    //const int v = Length< TypeList<int, int, float, A, A>>::value;

    //TypeAt<2, TypeList<int, int, float, A>>::res c = 1.f;

    //size_t i = IndexOf<TypeList<int, float, double>, double>::value;

    //Add<char, 1, TypeList<int, float, double>>::result d;

    //Remove<char, TypeList<int, char, float, char, double>>::result e;

    //Replace<TypeList<int, float, double>, double, char>::res rp;

    //MostDerived<TypeList<BB, BA1, BB21, BA2, BA, BB2, BB1>, BB>::res md;

    ////Replace<TypeList<BB, BA1, BB21, BA2, BA, BB2, BB1>, MostDerived<TypeList<BB, BA1, BB21, BA2, BA, BB2, BB1, BA>, BA>::res, BB>::res;

    //constexpr bool isb = isBase<BA1, BA>::res;
    //constexpr bool isc = Conversion<BA1, BA>::exists;

    //std::cout << Conversion<BA1, BA>::exists << std::endl;

    //DerivedToFront<TypeList<BB, BA1, BB21, BA2, BA, BB2, BB1>>::res dtf;

    //GenScatterHierarchy<TypeList<int, float, double>, Holder> sch;

    //GenLinearHierarchy<TypeList<int, float, double>, DerHolder> lh;

    //(static_cast<Holder<float>&>(sch))._obj = 5.1;
    //(static_cast<Holder<int>&>(sch))._obj = 13;


    //float fgs = (static_cast<Holder<float>&>(sch))._obj;

    //int igs = Field<0>(sch)._obj;
    //std::cout << fgs << " " << igs << std::endl;

    //integral_constant<int, 4> ic4;

    //conditional_t<false_type, A, BA> cond; 

    //AND<true_type, true_type>::res tr;
    //AND<false_type, true_type>::res fs;

    //disjunction<false_type, false_type, false_type, false_type>::res cj;

    //bool isf = is_float<float>::value;
    //std::cout << is_float_v<float> << " " << is_float_v<int> << " " << is_float_v<BA> << std::endl;

    //std::cout << is_poly_t<A> << " " << is_poly_t<BA1> << " " << is_poly_t<BB1> << " " << is_poly_t<BB> << std::endl;

    // (a&&b)V(!a&&c)V(a&&d&&c)V(!a&&!c&&!e)
    // Как реализовать is_float, чтобы он умер распознавать кастомные типы с плавающей точкой
    // Облегчённый вариант -- эти типы точно отнаследованы от float

    Animal* cat1 = new Cat;
    Animal* dog1 = new Dog;
    Animal* dog2 = new Dog;
    Animal* cat2 = new Cat;
    Animal* catdog1 = new CatDog;

    //DoubleDispatchMeet(ac, bc);
    //DoubleDispatchMeet(bc, cc);
    //DoubleDispatchMeet(dc, ac);
    //DoubleDispatchMeet(bc, smth);

    std::cout << std::endl << typeid(*cat1).name() << " " << typeid(Cat).name() << std::endl;
    std::cout << std::endl;

    StaticDispatcher<Meeting, Animal, TypeList< Dog, Cat, CatDog > >::Go(catdog1, cat1);

    BasicDispatcher<Animal, Animal> bdAnim;
    bdAnim.add<Dog, Dog>(MeetDog);
    bdAnim.add<Cat, Dog>(MeetCatDog);
    //bdAnim.add<Dog, Cat>(MeetCatDog);
    bdAnim.add<Cat, Cat>(MeetCat);

    bdAnim.Go(cat1, dog1);
    bdAnim.Go(cat1, cat2);

    // Написать BasicDispatcher на unordered_map
    // Подумать над реализацией симметричного StaticDispatcher

    // TypeList <dog, cat, catdog>
    // meet (d,d), (d,c), (d,cd), (c,c), (c,cd), (cd,cd)
    // 
}