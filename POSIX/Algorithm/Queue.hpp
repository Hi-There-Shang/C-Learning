//
//  Queue.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/9.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include <stdio.h>
#include "error.hpp"

struct __node {
    double data;
    __node *previous;
};

typedef __node Node;

void InitialNode(Node *node) {
    node = malloc(sizeof(Node));
}

int main() {
    
    return 0;
}

#endif /* Queue_hpp */
