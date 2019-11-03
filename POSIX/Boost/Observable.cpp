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
void Observable::notifiyObserver() {
    std::for_each(observers.begin(), observers.end(), [&](auto observer){
        observer->update();
    });
}
