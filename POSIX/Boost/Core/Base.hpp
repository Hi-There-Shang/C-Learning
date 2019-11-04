//
//  Base.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/4.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Base_hpp
#define Base_hpp

#include <stdio.h>

class Base {
public:
    Base() {
        printf("初始化 \n");
    }
    
    virtual ~Base() {
        printf("析构函数 \n");
    }
};

#endif /* Base_hpp */
