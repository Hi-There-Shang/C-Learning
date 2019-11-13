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


#include <vector>

void test_vector() {
    bool reuslt = TypeTraits<std::vector<int>::iterator>::isPointer;
    printf("%d --- %s \n", reuslt ,reuslt ? "fast" : "smart");
}

int main() {
    test_vector();
    return 0;
}

#endif /* TypeTraits_hpp */
