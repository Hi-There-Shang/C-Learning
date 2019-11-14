//
//  FunctorImpl.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/14.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef FunctorImpl_hpp
#define FunctorImpl_hpp

#include "TypeTraits.hpp"

template <typename R, typename TList> class FunctorImpl;

template <typename R>
class FunctorImpl<R, NullType> {
public:
    virtual R operator()() = 0;
    virtual FunctorImpl *clone() const = 0;
    virtual ~FunctorImpl() {}
};

template <typename R, typename P1>
class FunctorImpl<R, TypeList<P1, NullType>> {
public:
    virtual R operator()() = 0;
    virtual FunctorImpl *clone() const = 0;
    virtual ~FunctorImpl() {}
};

template <typename R, typename P1, typename P2>
class FunctorImpl<R, TypeList<P1, TypeList<P2, NullType>>> {
public:
    virtual R operator()() = 0;
    virtual FunctorImpl *clone() const = 0;
    virtual ~FunctorImpl() {}
};

#endif /* FunctorImpl_hpp */
