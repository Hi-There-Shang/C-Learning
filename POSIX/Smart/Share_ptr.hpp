//
//  Share_ptr.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Share_ptr_hpp
#define Share_ptr_hpp

#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include "Traits.hpp"
#include <vector>
#include <iostream>

template <typename T>
class vector_wrapper {
public:
    typedef boost::shared_ptr<T> weak_ptr;
    typedef std::vector<weak_ptr> vector;
    typedef Traits<T> Traits;
    typedef typename Traits::value_type value_type;
    
    vector_wrapper(): container(0) {}
    void push(typename Traits::value_type value);
    weak_ptr pop();
    
private:
    vector container;
};

template <class T>
void vector_wrapper<T>::push(typename Traits::value_type value) {
    boost::shared_ptr<value_type> sValue(new value_type(value));
//    boost::weak_ptr<value_type> wValue = sValue;
    container.push_back(sValue);
}

//template <class T>
//typename vector_wrapper<T>::weak_ptr vector_wrapper<T>::pop() {
//    auto wValue = container.pop_back();
//    return wValue;
//}

struct user {
    std::string name;
    
    user(std::string _name): name(_name) {
        std::cout << "初始化" << std::endl;
    }
    
    user& operator=(const user& u) {
        if (this == &u) {
            return *this;
        }
        std::cout << "拷贝赋值函数" << std::endl;
        name = u.name;
        return *this;
    }
    
    user(const user &u) {
        name = u.name;
        std::cout << "拷贝构造函数" << std::endl;
    }
    
    ~user() {
        std::cout << "析构函数" << std::endl;
    }
};
        
int main1111() {
        user u("1");
        user u1("2");
        user u2("3");
        user u3("4");
        user u5("5");
        vector_wrapper<user> wrapper;
        wrapper.push(u);
        wrapper.push(u1);
        wrapper.push(u2);
        wrapper.push(u3);
        
//        std::cout << wrapper.pop()->name << std::endl;
        
    return 0;
}

#endif /* Share_ptr_hpp */
