//
//  Mutex.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/4.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Mutex_hpp
#define Mutex_hpp

#include <stdio.h>
#include "error.hpp"

namespace Thread {
struct Tag: Base {
    pthread_mutex_t mutex;
    int  value;
    Tag(): Base() {
//        printf("初始化 \n");
    }
    ~Tag() {
//        printf("析构函数 \n");
    }
};
}

#endif /* Mutex_hpp */
