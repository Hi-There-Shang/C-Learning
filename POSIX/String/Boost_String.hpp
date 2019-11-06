//
//  Boost_String.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/6.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Boost_String_hpp
#define Boost_String_hpp

#include "error.hpp"
#include <stdio.h>

//namespace __Print {
template <class T>
void print(T &&value) {
    std::cout << value << std::endl;
}
template <class T>
inline bool valid_number(const std::string &name) {
    T tmp;
    return boost::conversion::try_lexical_convert(name, tmp);
}

template <class T>
struct outable {
    friend std::ostream& operator<<(std::ostream &os, const T&){
        os << typeid(T).name();
        return os;
        }
        };
        
        //            }

struct demo_class: public outable<demo_class> {
//    friend std::ostream& operator<<(std::ostream &os, const demo_class &demo) {
//        os << "demo_class'name";
//        return os;
//    }
};

//using namespace __Print;
using namespace boost;
void test_boost_string() {
     /// 类型不匹配 int 类型不匹配
     ///  short类型也不匹配
     ///std::string name = "123.1";
    
    
        std::cout << lexical_cast<std::string>(demo_class()) << std::endl;
    
    std::string name = "123.1";
    int num3;
    try {
        num3 = lexical_cast<int>(name);
    } catch (std::exception &exception) {
        print(exception.what());
    }
    int num4 = 0;
    /// try_to
    if (boost::conversion::try_lexical_convert(name, num4)) {
        print("成功");
    } else {
        print("失败");
    }
    
    if (valid_number<short>(name)) {
        print("1成功");
    } else {
        print("1失败");
    }
    
    int num1 = lexical_cast<float>(name);
    int num2 = lexical_cast<double>(name);
   
//    int num3 = lexical_cast<int>(name);
//    int num4 = lexical_cast<short>(name);
    print(num1);
    print(num2);
    print(num3);
//    print(num4);
}
        
using namespace boost::xpressive;
void test_xpressive_regex() {
      cregex regex = cregex::compile("a.c");
        sregex sre = sregex::compile("a.c");
        std::string name = "abc";
        regex_match(name, sre);
        regex_match("abc", regex);
}

int main() {
        BOOST_ASSERT_MSG(false, "error");
    test_boost_string();
    return 0;
}

#endif /* Boost_String_hpp */
