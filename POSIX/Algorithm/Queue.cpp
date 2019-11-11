//
//  Queue.cpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/9.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "Queue.hpp"

int main() {
    __node node = {0, nullptr};
    InitialNode(&node);
    
    printf("%p \n", &node);
    
    __node *head = &node;
    insertBack(&head, 10);
    printf("%p \n", *head);
    insertBack(&head, 11);
    insertBack(&head, 12);
    insertBack(&head, 13);
    
    printNode(&node);
    return 0;
}
