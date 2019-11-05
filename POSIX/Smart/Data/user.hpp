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

#endif /* user_hpp */
