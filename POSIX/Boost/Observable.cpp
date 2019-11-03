//
//  Observerable.cpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "Observable.hpp"

void Observable::registerObserver(Observer *observer) {
    this->observers.push_back(observer);
}
void Observable::unregister(Observer *observer) {
    this->observers.emplace_back(observer);
}

void Observable::registerObserver(boost::weak_ptr<Observer> *ptr) {
    this->observers_ptr.push_back(ptr);
}

void Observable::unregister(boost::weak_ptr<Observer> *ptr) {
    this->observers_ptr.emplace_back(ptr);
}

void Observable::notifiyObserver() {
    
    std::for_each(observers_ptr.begin(), observers_ptr.end(), [&](auto observer_ptr){
        boost::shared_ptr<Observer> obj(observer_ptr->lock());
        if (obj) {
            obj->update();
        } else {
            observers_ptr.emplace_back(observer_ptr);
        }
    });
    
    /// 非线程安全
//    std::for_each(observers.begin(), observers.end(), [&](auto observer){
//        observer->update();
//    });
}
