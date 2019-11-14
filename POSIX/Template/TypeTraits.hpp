//
//  TypeTraits.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/13.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef TypeTraits_hpp
#define TypeTraits_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

struct NullType;
struct EmptyType {};

template <class T>
class TypeTraits {
private:
    template <class U>
    struct PointerTraits {
        enum {result = false};
        typedef NullType PointeeType;
    };
    
    template <class U>
    struct PointerTraits<U*> {
        enum {result = true};
        typedef U PointeeType;
    };
    
public:
    enum {isPointer = PointerTraits<T>::result};
    typedef typename PointerTraits<T>:: PointeeType PointeeType;
};


template <class T, class U>
struct TypeList {
    typedef T Head;
    typedef U Tail;
};

#define TYPELIST_1(T1) TypeList<T1, NullType>
#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T1)>
#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4)>

template <class T> struct Length;

template <>
struct Length<NullType> {
    enum {value = 0};
};

template <class T, class U>
struct Length<TypeList<T, U>> {
    enum {value = Length<U>::value + 1};
};

template <class TList, unsigned int i> struct TypeAt;

template <class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0> {
    typedef Head Result;
};

template <class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i> {
    typedef typename TypeAt<Tail, i - 1>::Reslut Result;
};

template <class TList, class T> struct IndexOf;

template <class T>
struct IndexOf<NullType, T> {
    enum {value = -1};
};

template <class Tail, class T>
struct IndexOf<TypeList<T, Tail>, T> {
    enum {value = 0};
};

template <class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T> {
private:
    enum {temp = IndexOf<Tail, T>::value};
public:
    enum {value = temp == -1 ? -1 : temp + 1};
};

template <bool> struct ComplieTimeChecker;

#define STATIC_CHECK(x)  {char unnamed[(x) ? 1 : -1];}
#define STATIC_COMPLIE(expr) (ComplieTimeError<(expr) != 0>())
#define DESTON "Cl"
#define Log sizeof(int);

#define STATIC_CHECKER(expr, msg)  \
{\
class ERROR_##msg {}; \
auto value = ComplieTimeChecker<(expr)>(ERROR_##msg());\
(void)sizeof(value);  \
}

#define CLASS(name)  \
class A_##name {};


template <int v>
struct Int2Type {
    enum {value = v};
};

template <bool flag, class T, class U>
struct Select {
    typedef T Result;
};

template <class T, class U>
struct Select<false, T, U>  {
    typedef U Result;
};

template <class To, class From>
To safe_reinterpret_Cast(From from) {
    //    assert(sizeof(From) <= sizeof(To));
    STATIC_CHECKER((sizeof(From) <= sizeof(To)), 3);
    return reinterpret_cast<To>(from);
}

template <bool> struct ComplieTimeError;

template <>
struct ComplieTimeError<true> { };

template <bool>
struct ComplieTimeChecker {
    ComplieTimeChecker(...) {}
};

template <>
struct ComplieTimeChecker<false> { };


//void test_vector() {
//    bool reuslt = TypeTraits<std::vector<int>::iterator>::isPointer;
//    printf("%d --- %s \n", reuslt ,reuslt ? "fast" : "smart");
////    typedef TypeAt<TypeList<int, TYPELIST_2(float, double)>, 1>::Result Result1;
////    typedef TypeAt<TypeList<int, double>, >::Result Result2;
////    std::cout << typeid(Result1).name() << std::endl;
////    std::cout << typeid(Result2).name() << std::endl;
//}

//int main____() {
//    test_vector();
//    return 0;
//}

#endif /* TypeTraits_hpp */
