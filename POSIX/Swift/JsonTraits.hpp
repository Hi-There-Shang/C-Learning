//
//  JsonTraits.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/16.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef JsonTraits_hpp
#define JsonTraits_hpp

#include <stdio.h>
#include <iostream>
#include <boost/container/string.hpp>

template <typename T> struct ObjectTraits {};

template <typename T> struct ScalarEnumerationTraits {};

template <typename T> struct ScalarBitSetTraits {};

template <typename T> struct ScalarTraits {};

template <typename T> struct ScalarReferenceTraits {};

template <typename T> struct NullableTraits {};

template <typename T> struct ArrayTraits {};

template <typename T, T> struct SameType;
template <typename T> struct MissingTraits;

template <class T>
struct has_ScalarEnumerationTraits {
    using Signature_enumeration = void (*)(class Output &, T &);
    
    template <class U>
    static char test(SameType<Signature_enumeration, *U::enumeration> *);
    
    template <class U>
    static double test(...);
    
public:
    static bool const value = (sizeof(test<ScalarBitSetTraits<T>>(nullptr) == 1));
};

template <class T>
struct has_ScalarTraits {
    ///  std::ostream  ---> llvm::raw_ostream
    using Signature_output = void (*)(const T&, std::ostream &);
    using Signature_mustQuote = bool (*)(std::string &);
    
    template <class U>
    static char test(SameType<Signature_output, &U::output>*, SameType<Signature_mustQuote, &U::mustQuote>);
    
    template <class U>
    static double test(...);
public:
    static bool const value = (test<ScalarTraits<T>>(nullptr, nullptr) == 1);
};

#endif /* JsonTraits_hpp */
