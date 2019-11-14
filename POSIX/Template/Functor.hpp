//
//  Functor.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/14.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Functor_hpp
#define Functor_hpp

#include "FunctorImpl.hpp"
#include <memory>

template <typename R, typename TList>
class Functor {
public:
    typedef TList ParamList;
    typedef typename TypeAt<ParamList, 0>::Result Param1;
    typedef typename TypeAt<ParamList, 1>::Result Param2;
public:
    Functor() {};
    Functor(const Functor& func) {
        if (*this == func) {
            return this;
        }
        spImpl_ = func.spImpl_;
        return this;
    }
    
    Functor& operator=(const Functor& func) {
        this->spImpl_ = func.spImpl_;
        return *this;
    }
    
    explicit Functor(std::auto_ptr<FunctorImpl<R, TList>> impl) {
        this->spImpl_ = impl;
    }
    
public:
    R operator()() {
        return (*spImpl_)();
    }
    
    R operator()(Param1 p1) {
        return (*spImpl_)(p1);
    }
    
    R operator()(Param1 p1, Param2 p2) {
        return (*spImpl_)(p1, p2);
    }
    
private:
    typedef FunctorImpl<R, TList> Impl;
    std::auto_ptr<Impl> spImpl_;
    
};

int main() {
    Functor<double, TYPELIST_3(int, double, float)> functor;
//    functor(1,2);
    return 0;
}
#endif /* Functor_hpp */
