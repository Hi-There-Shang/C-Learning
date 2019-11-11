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
    node = (Node *)malloc(sizeof(Node));
    node->data = 0;
    node->previous = nullptr;
}

void insertFront(Node *node, double value) {
    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->data = value;
    newnode->previous = nullptr;
    
    Node *head = node;
    while (head->previous) {
        head = head->previous;
    }
    printf("front %f \n", newnode->data);
    head->previous = newnode;
}

void insertBack(Node **node, double value) {
//    Node *newnode = (Node *)malloc(sizeof(Node));
//    newnode->data = value;
//
//    newnode->previous = *node;
//    node = &newnode;
}

void remove(Node *node, int index) {
    
}

void clear(Node *node) {
    
}

void printNode(Node *node) {
    Node *head = node;
    while (head->previous) {
        head = head->previous;
        printf("-- %f \n", head->data);
    }
}



#endif /* Queue_hpp */
