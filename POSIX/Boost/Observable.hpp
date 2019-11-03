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
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Observable {
public:
    void registerObserver(Observer *observer);
    void registerObserver(boost::weak_ptr<Observer> *ptr);
    void unregister(Observer *observer);
    void unregister(boost::weak_ptr<Observer>*ptr);
    
    void notifiyObserver();
    
public:
    std::vector<Observer*> observers;
    std::vector<boost::weak_ptr<Observer> *> observers_ptr;
};

#endif /* Observable_hpp */
