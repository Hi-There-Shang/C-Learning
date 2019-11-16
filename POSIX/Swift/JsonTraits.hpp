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
#include <boost/regex.hpp>
#include <boost/xpressive/basic_regex.hpp>
#include <boost/container/string.hpp>

typedef boost::container::string StringRef;

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
    using Signature_mustQuote = bool (*)(StringRef);
    
    template <class U>
    static char test(SameType<Signature_output, &U::output>*, SameType<Signature_mustQuote, &U::mustQuote>);
    
    template <class U>
    static double test(...);
public:
    static bool const value = (test<ScalarTraits<T>>(nullptr, nullptr) == 1);
};

template <class T>
struct has_ScalarReferenceTratis {
    using Signature_StringRef = StringRef (*)(const T&);
    using Signature_mustQuout = bool (*)(StringRef);
    
    template <class U>
    static char test(SameType<Signature_StringRef, &U::stringRef>*, SameType<Signature_mustQuout, &U::mustQuote>*);
    
    template <class U>
    static bool test(...);
    
public:
    static bool const value = (test<ScalarReferenceTraits<T>>(nullptr, nullptr) == 1);
};

template <class T>
struct has_ObjectTraits
{
    using Signature_mapping = void (*)(class Output &, T &);
    
    template <typename U>
    static char test(SameType<Signature_mapping, &U::mapping>*);
    
    template <typename U>
    static double test(...);
    
public:
    static bool const value = (sizeof(test<ObjectTraits<T> >(nullptr)) == 1);
};


template <class T>
struct has_ObjectValidateTraits
{
    using Signature_validate = StringRef (*)(class Output &, T &);
    
    template <typename U>
    static char test(SameType<Signature_validate, &U::validate>*);
    
    template <typename U>
    static double test(...);
    
public:
    static bool const value = (sizeof(test<ObjectTraits<T> >(nullptr)) == 1);
};

template <class T>
struct has_ArrayMethodTraits
{
    using Signature_size = size_t (*)(class Output &, T &);
    
    template <typename U>
    static char test(SameType<Signature_size, &U::size>*);
    
    template <typename U>
    static double test(...);
    
public:
    static bool const value =  (sizeof(test<ArrayTraits<T> >(nullptr)) == 1);
};

template<typename T>
struct has_ArrayTraits : public std::integral_constant<bool,
has_ArrayMethodTraits<T>::value > { };

// Test if NullableTraits<T> is defined on type T.
template <class T>
struct has_NullableTraits
{
    using Signature_isNull = bool (*)(T &);
    
    template <typename U>
    static char test(SameType<Signature_isNull, &U::isNull> *);
    
    template <typename U>
    static double test(...);
    
public:
    static bool const value =
    (sizeof(test<NullableTraits<T>>(nullptr)) == 1);
};

inline bool isNumber(StringRef S) {
    /static const char DecChars[] = "0123456789";
    
    if (S.find_first_not_of(DecChars) == StringRef::npos)
        return true;
    using Regex = boost::xpressive::sregex;
    Regex reg = Regex::compile("^(\\.[0-9]+|[0-9]+(\\.[0-9]*)?)([eE][-+]?[0-9]+)?$");
//    boost::regex_match(<#const std::basic_string<charT, ST, SA> &s#>, <#const basic_regex<charT, traits> &e#>)
//    boost::regex_match(S, reg);
//    if (boost::regex_match(S, reg)) {
//        return true;
//    }
    return false;
}

#endif /* JsonTraits_hpp */
