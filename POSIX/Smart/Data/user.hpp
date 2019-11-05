//
//  user.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef user_hpp
#define user_hpp

#include <stdio.h>
#include "error.hpp"

using namespace std;
struct __user: Base {
    string m_name;
    __user(string name): m_name(name) {}
    ~__user() {}
};

struct book: Base {
    double m_price;
    double m_page;
    book(int price, int page): m_price(price), m_page(page) {}
};

struct teacher {
    int age;
    int height;
    teacher(int age, int height): age(age), height(height) {}
};

class student {
public:
    student(string name, string language): m_name(name), m_language(language) {}
    
private:
    string m_name;
    string m_language;
};

#endif /* user_hpp */
