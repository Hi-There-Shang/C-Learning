//
//  TestEnum.cpp
//  POSIX
//
//  Created by Mr_Shang on 2020/8/2.
//  Copyright © 2020 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "TestEnum.hpp"
#include <boost/type_traits.hpp>
#include <iostream>

enum class SpecializationPass : uint8_t {
  AllocBoxToStack,
  ClosureSpecializer,
  CapturePromotion,
  CapturePropagation,
  FunctionSignatureOpts,
  GenericSpecializer,
};

static inline char encodeSpecializationPass(SpecializationPass Pass) {
  return char(uint8_t(Pass)) + '0';
}

template <typename T>
static void logger(T value) {
    
    std::cout << value << std::endl;
}

static void test() {
    SpecializationPass pass = SpecializationPass::AllocBoxToStack;
    auto value = encodeSpecializationPass(pass);
    logger(value);
    pass = SpecializationPass::ClosureSpecializer;
     value = encodeSpecializationPass(pass);
    logger(value);
     pass = SpecializationPass::CapturePromotion;
     value = encodeSpecializationPass(pass);
    logger(value);
     pass = SpecializationPass::CapturePropagation;
     value = encodeSpecializationPass(pass);
    logger(value);
    pass = SpecializationPass::FunctionSignatureOpts;
    value = encodeSpecializationPass(pass);
    logger(value);
    
    
    
    
}

int main() {
    test();
    
}
