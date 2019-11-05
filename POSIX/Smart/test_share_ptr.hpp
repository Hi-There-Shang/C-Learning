//
//  test_share_ptr.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef test_share_ptr_hpp
#define test_share_ptr_hpp

#include <stdio.h>
#include "error.hpp"
#include "user.hpp"

namespace __Print {
    template <class T>
    void print(T &&value) {
        std::cout << value << std::endl;
    }
}

using namespace __Print;
using namespace std;

typedef boost::shared_ptr<book> book_pointer;

void test_weak_share() {

}

void test_thread() {
    
}


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

int main() {
    book_pointer book(new struct book(12, 100));
//    boost::shared_ptr<teacher> tea(new teacher(1,2));
    new student("123","321");
    test_thread();
    return 0;
}

#endif /* test_share_ptr_hpp */
