//
//  Foo.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Foo_hpp
#define Foo_hpp

#include "Observable.hpp"
#include <stdio.h>

class Foo: public Observer {
public:
    virtual void update();
    void obser(Observable *s) {
        s->registerObserver(this);
    }
};
#endif /* Foo_hpp */
