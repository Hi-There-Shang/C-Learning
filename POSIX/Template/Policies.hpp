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

/// 策略实现类
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

/// 策略选择器
template <typename Setter1, typename Setter2, typename Setter3, typename Setter4>
class PolicySelector: public Discriminator<Setter1, 1>, public Discriminator<Setter2, 2>, public Discriminator<Setter3, 3>, public Discriminator<Setter4, 4>{
public:
    
};

/// 策略参数  base 基类中 定义具体的参数  通过 typename 萃取
class DefaultPolicyArgs: virtual public DefaultPolicies {
public:
};

/// 修改策略---> 经典 ---> 同一个基类---> 不同的策略 不同的实现---> 问题 在于 需要提供给 策略实现类中 所需要的参数
/// 所有的一切只是桥梁---> 实现在于 策略提供类和策略实现类
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

/// 供给端----> 不同的策略提供不同的服务----> 提供一套默认的策略服务
template <typename T1 = DefaultPolicyArgs,
          typename T2 = DefaultPolicyArgs,
          typename T3 = DefaultPolicyArgs,
          typename T4 = DefaultPolicyArgs>
class Slider {
public:
    typedef PolicySelector<T1, T2, T3, T4> Policies;
    
};

#endif /* Policies_hpp */
