//
//  TrailingObjects.cpp
//  POSIX
//
//  Created by Mr_Shang on 2020/7/25.
//  Copyright © 2020 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "TrailingObjects.hpp"
#include <stdlib.h>

struct InProcess {
    template <typename T>
    using Pointer = T*;
};

struct OpaqueValue {};

template <typename Runtime>
struct TargetMetadata {
};

template <typename Runtime, typename T>
using TargetPointer = typename Runtime::template Pointer<T>;

template <typename Runtime, typename T>
using ConstTargetPointer = typename Runtime::template Pointer<const T>;

#define VOID_TYPE void
#define MUTABLE_VALUE_TYPE TargetPointer<Runtime, OpaqueValue>
#define TYPE_TYPE ConstTargetPointer<Runtime, Metadata>



#define FUNCTION_VALUE_WITNESS(lowerId, upperId, returnType, paramTypes) \
VALUE_WITNESS(lowerId, upperId)


void func_test() {
//    FUNCTION_VALUE_WITNESS(destroy, Destroy, VOID_TYPE, (MUTABLE_VALUE_TYPE, TYPE_TYPE));
}


template <typename Runtime>
class TargetValueWitnessTypes {
public:
//    typdef TargetPointer
};

template <typename First, typename... Rest> class AlignmentCalcHelper {
private:
  enum {
    FirstAlignment = alignof(First),
    RestAlignment = AlignmentCalcHelper<Rest...>::Alignment,
  };

public:
  enum {
    Alignment = FirstAlignment > RestAlignment ? FirstAlignment : RestAlignment
  };
};

template <typename First> class AlignmentCalcHelper<First> {
public:
  enum { Alignment = alignof(First) };
};

class TrailingObjectsBase {
public:
  template <typename T> struct OverloadToken {};
};

template <int Align>
class TrailingObjectsAligner : public TrailingObjectsBase {};

template <>
class TrailingObjectsAligner<1> : public TrailingObjectsBase {};

template <int Align, typename... MoreTys>
class TrailingObjectsImpl {
  // The main template definition is never used -- the two
  // specializations cover all possibilities.
};

struct A {
    
};

struct B {
    
};


void testAlign() {
    TrailingObjectsAligner<1> trailing;
    
}
