//
//  Stock.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Stock_hpp
#define Stock_hpp

#include <stdio.h>
#include "Traits.hpp"

template <class Name>
class Stock {
public:
    typedef Traits<Name> Name_Traits;
    Stock(typename Name_Traits::value_type __name): name(__name) {}
    
private:
    typename Name_Traits::value_type name;
};


#endif /* Stock_hpp */
