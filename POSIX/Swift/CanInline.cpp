//
//  CanInline.cpp
//  POSIX
//
//  Created by Mr_Shang on 2020/7/28.
//  Copyright © 2020 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "CanInline.hpp"
#include <boost/type_traits.hpp>
#include <boost/function.hpp>

template <typename Runtime, typename T>
using TargetPointer = typename Runtime::template Pointer<T>;

template <typename Runtime>
struct TargetValueBuffer {
  TargetPointer<Runtime, void> PrivateData[3];
};


struct InProcess {
    static constexpr size_t PointerSize = sizeof(uintptr_t);
    //typedef unsigned long           uintptr_t;
    using StoredPointer = uintptr_t;
    using StoredSize = size_t;
    using StoredPointerDifference = ptrdiff_t;
    
    static_assert(sizeof(StoredSize) == sizeof(StoredPointerDifference),
                  "target uses differently-sized size_t and ptrdiff_t");
    
    template <typename T>
    using Pointer = T*;
};

using ValueBuffer = TargetValueBuffer<InProcess>;


template <typename Runtime>
struct TargetValueWitnessTable {
    
    using StoredSize = typename Runtime::StoredSize;
    
    static bool isValueInline(bool isBitwiseTakable, StoredSize size,
                               StoredSize alignment) {
       return (isBitwiseTakable && size <= sizeof(TargetValueBuffer<Runtime>) &&
               alignment <= alignof(TargetValueBuffer<Runtime>));
     }
};

template <typename Runtime> struct TargetValueWitnessTable;
using ValueWitnessTable = TargetValueWitnessTable<InProcess>;



template<typename T> struct _ResultOf;

template<typename R, typename...A>
struct _ResultOf<R(*)(A...)> {
    using type = R;
};

template<typename T>
struct Temp {
    typedef T Type;
};

struct A {
    
};

template<typename T> typename _ResultOf<typename Temp<T>::Type>::type some() {
    
}

void testIsInline() {
    
    boost::function<int (int,int)> Func;
    
    
    
//    ValueWitnessTable::isValueInline(<#bool isBitwiseTakable#>, <#StoredSize size#>, <#StoredSize alignment#>)
}
