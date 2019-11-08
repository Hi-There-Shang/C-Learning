//
//  Policies.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/8.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Policies_hpp
#define Policies_hpp

#include <stdio.h>

template <typename Base, int D>
class Discriminator: public Base {
public:
    typedef typename Base::P1 Policy1;
    typedef typename Base::P2 Policy2;
    typedef typename Base::P3 Policy3;
    typedef typename Base::P4 Policy4;
    void choose() {
        switch (D) {
            case 1:
                break;
            case 2:
                break;
                
            default:
                break;
        }
    }
};

template <typename Setter1, typename Setter2, typename Setter3, typename Setter4>
class PolicySelector: public Discriminator<Setter1, 1>, public Discriminator<Setter2, 2>, public Discriminator<Setter3, 3>, public Discriminator<Setter4, 4>{
public:
    
};

class DefaultPolicy1 {
public:
    
};

class DefaultPolicy2 {
public:
    
};

class DefaultPolicy3 {
public:
    
};

class DefaultPolicy4 {
public:
    
};

class DefaultPolicies {
public:
    typedef DefaultPolicy1 P1;
    typedef DefaultPolicy2 P2;
    typedef DefaultPolicy3 P3;
    typedef DefaultPolicy4 P4;
};

class DefaultPolicyArgs: virtual public DefaultPolicies {
public:
};

template <typename Policy>
class Policy_is: virtual public DefaultPolicies {
public:
    typedef Policy P1;
};

template <typename Policy>
class Policy1_is: virtual public DefaultPolicies {
public:
    typedef Policy P1;
};

template <typename Policy>
class Policy2_is: virtual public DefaultPolicies {
public:
    typedef Policy P2;
};

template <typename Policy>
class Policy3_is: virtual public DefaultPolicies {
public:
    typedef Policy P3;
};
template <typename Policy>
class Policy4_is: virtual public DefaultPolicies {
public:
    typedef Policy P4;
};

template <typename T1 = DefaultPolicyArgs,
          typename T2 = DefaultPolicyArgs,
          typename T3 = DefaultPolicyArgs,
          typename T4 = DefaultPolicyArgs>
class Slider {
public:
    typedef PolicySelector<T1, T2, T3, T4> Policies;
    
};

#endif /* Policies_hpp */
