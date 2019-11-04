//
//  Boost_array.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Boost_array_hpp
#define Boost_array_hpp

#include <boost/array.hpp>
#include <boost/unordered/unordered_set.hpp>
#include <stdio.h>
#include <iostream>

void testArray() {
    boost::array<int, 6> arr;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    
//    std::for_each(arr.begin(), arr.end(), [&](auto value){
//        std::cout << value << std::endl;
//    });
    
    std::for_each(arr.cbegin(), arr.cend(), [&](auto value){
        std::cout << value << std::endl;
    });
    
    if (boost::array<int, 6>::empty()) {
        std::cout << "kong" << std::endl;
    }
}

void testUnorder_set() {
    boost::unordered_set<int> set;
    set.insert(1);
    set.emplace_hint(set.begin(), 2);
    set.insert(3);
    auto iter = set.find(1);
    set.erase(1);
    set.emplace(5);
    std::cout << *iter << std::endl;
    std::for_each(set.cbegin(), set.cend(), [&](auto value){
        std::cout << value << std::endl;
    });
}

int main123456() {
    testUnorder_set();
    return 0;
    testArray();
    return 0;
}

#endif /* Boost_array_hpp */
