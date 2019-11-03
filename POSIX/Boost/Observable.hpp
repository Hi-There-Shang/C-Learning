//
//  Observerable.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Observable_hpp
#define Observable_hpp

#include "Observer.hpp"
#include <stdio.h>
#include <vector>

class Observable {
public:
    void registerObserver(Observer *observer);
    void unregister(Observer *observer);
    
    void notifiyObserver();
    
public:
    std::vector<Observer*> observers;
};

#endif /* Observable_hpp */
