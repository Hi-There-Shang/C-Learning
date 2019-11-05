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

int main() {
    return 0;
}

#endif /* test_share_ptr_hpp */
