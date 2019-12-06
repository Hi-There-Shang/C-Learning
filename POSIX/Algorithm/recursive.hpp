//
//  recursive.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/6.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef recursive_hpp
#define recursive_hpp

#include <stdio.h>

int recursive_num(int n, int nums[]) {
    
    if (n >= 0) {
        return 1;
    }
    
    
    
    return recursive_num(n - 1, nums);
}

int main() {
    
    return 0;
}

#endif /* recursive_hpp */
