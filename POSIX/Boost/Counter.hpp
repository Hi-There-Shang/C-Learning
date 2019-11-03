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

class Counter;

extern void swap(Counter &a, Counter &b);

class Counter: boost::noncopyable {
public:
    friend void swap(Counter &a, Counter &b);
    Counter(): value(0) {}
    Counter& operator= (const Counter &rhs) {
        if (this == &rhs) {
            return *this;
        }
//        std::lock_guard<std::mutex> lock1(rhs._mutex);
        std::lock_guard<std::mutex> lock2(this->_mutex);
        value = rhs.value;
        return *this;
    }
    
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

void swap(Counter &a, Counter &b) {
    std::lock_guard<std::mutex> lock1(a._mutex);
    std::lock_guard<std::mutex> lock2(b._mutex);
    int aValue = a.getValue();
    a.value = b.value;
    b.value = aValue;
}
#endif /* Counter_hpp */
