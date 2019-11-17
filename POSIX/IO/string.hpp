//
//  string.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/14.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef string_hpp
#define string_hpp

#include <stdio.h>
#include "error.hpp"

using namespace std;
void test_string() {
    string name = "1234566";
    string sub_name = name.substr(0, 3);
    name.replace(1, 3, "qwert");
    
    printf("%s \n", name.c_str());
}

template <typename Tp, Tp f>
struct is_pointer_ {
    typedef is_pointer_ Pointer;
    typedef Tp value_type;
    static const Tp flags = f;
    
     operator value_type() const {
        return flags;
    }
    
    value_type operator()() const {
        return flags;
    }
};

template <typename Tp, Tp _tp>
const Tp is_pointer_<Tp, _tp>::flags;


template <bool _tp>
using bool_pointer = is_pointer_<bool, _tp>;

typedef bool_pointer<true> True_type;
typedef bool_pointer<false> False_type;

//template <typename >

template <typename T>
void test_std(const T &v) {
//    auto __v = std::enable_if<v>::type;
//    _Log_::Log(__v);
}

int main() {
    _Log_::Log(True_type());
    _Log_::Log(True_type::flags);
    _Log_::Log(False_type());
    _Log_::Log(False_type::flags);
//    int num = 1;
//    test_std(num);
//    bool result = false;
//    auto v = std::enable_if<result>::value
//    std::is_unsigned<<#class _Tp#>>
//    test_string();
    return 0;
}

#endif /* string_hpp */
