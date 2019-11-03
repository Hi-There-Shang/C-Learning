//
//  Counter.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Counter_hpp
#define Counter_hpp

#include <boost/noncopyable.hpp>
#include <stdio.h>
#include <mutex>

class Counter: boost::noncopyable {
public:
    Counter(): value(0) {}
    int getValue() const {
        return value;
    }
    
    int getAndIncrease() {
        std::lock_guard<std::mutex> lock(_mutex);
        ++value;
        return value;
    }
    
private:
    int value;
    std::mutex _mutex;
};

#endif /* Counter_hpp */
