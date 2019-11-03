//
//  Observer.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Observer_hpp
#define Observer_hpp

#include <stdio.h>


class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() = default;
};

#endif /* Observer_hpp */
