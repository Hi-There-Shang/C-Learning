//
//  Tmp.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/7.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Tmp_hpp
#define Tmp_hpp

#include <stdio.h>
#include "error.hpp"



/*
 effective 临时对象的生成,返回优化
 */

class __Book {
    int age;
public:
    friend const __Book operator+(const __Book &lhs, const __Book &rhs);
    __Book(int age): age(age) {}
    
   inline __Book(const __Book &book) {
        printf("拷贝o构造 \n");
        this->age = book.age;
    }
    __Book& operator= (const __Book &u) {
        printf("拷贝赋值 \n");
        this->age = u.age;
        return *this;
    }
};

typedef __Book book;

/*
 参数传递 会调用拷贝构造
 */
//void test_arg(book bk) {
//
//}


/*
 引用传递 不拷贝
 */
void test_arg(const book &bk) {
    
}

/*
 参数传递一次 返回一次 拷贝构造
 */
//book retBook(book b) {
//    return b;
//}

/*
 返回拷贝
 */
const book retBook(const book &b) {
    return b;
}

/*
 返回拷贝构造?
 貌似编译器没优化???
 */
const book operator+(const book &lhs, const book &rhs) {
    return book(lhs.age + rhs.age);
}

int main() {
    book boo(12);
    test_arg(boo);
    book tmp = retBook(boo);
    
    book boo1 = tmp + boo;
    return 0;
}

#endif /* Tmp_hpp */
