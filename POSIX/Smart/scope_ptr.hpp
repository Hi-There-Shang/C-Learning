//
//  scope_ptr.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef scope_ptr_hpp
#define scope_ptr_hpp

#include <stdio.h>
#include "error.hpp"

namespace __Print {
    template <class T>
    void print(T &&value) {
        std::cout << value << std::endl;
    }
}

using namespace boost;
using namespace __Print;

void test_scope_ptr() {
    scoped_ptr<int> int_ptr1(new int(8));
    scoped_ptr<int> int_ptr2(new int(9));
    print(*int_ptr1.get());
    print(*int_ptr1.get());
    print(*int_ptr2.get());
    
//    auto v = int_ptr;
//    scoped_ptr<int> p(int_ptr);
//    int_ptr1 == int_ptr2;
}

void test_uniqe_ptr() {
    auto ptr1 = make_unique<int>(19);
    auto ptr2 = make_unique<int>(18);
    assert(ptr1 != ptr2);
    print(ptr1.get());
    print(ptr2.get());
    print(*ptr1.get());
    print(*ptr2.get());
    
}

int main() {
    test_uniqe_ptr();
    test_scope_ptr();
    return 0;
}

#endif /* scope_ptr_hpp */
