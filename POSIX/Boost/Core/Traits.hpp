//
//  Traits.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Traits_hpp
#define Traits_hpp

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

template <class T>
struct Traits {
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        refenerce;
    typedef const T&  const_refenerce;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
};

#endif /* Traits_hpp */
