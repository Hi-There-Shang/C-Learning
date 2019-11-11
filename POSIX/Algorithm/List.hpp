//
//  List.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/11.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

#include "error.hpp"
#include <stdio.h>

using namespace _Log_;
template <typename T>
struct __node {
    typedef __node<T> Node;
    typedef T value_type;
    
    __node() {}
    __node(value_type value): value(value) {}
    
    value_type value;
    Node *previous;
    Node *next;
};

template <typename T>
struct List {
    typedef __node<T> Node;
    
    List() {
        Node *newNode = nullptr;
        try {
            newNode = new Node();
            newNode->next = nullptr;
            newNode->previous = nullptr;
        } catch (std::exception &exce) {
            Log(exce.what());
            if (newNode) {
                delete newNode;
            }
        }
    }
    
    void insertFront(T value);
    void insertBack(T value);
    void insertAt(T value, int index);
    void removeAt(int index);
    void clear();
    void printNode();
    
    int capacity;
    Node *node;
    
};

template <class T>
void List<T>::insertFront(T value) {
    Node *newNode = new Node(value);
    newNode->previous = nullptr;
    newNode->next = nullptr;
    
    Node *head = node;
    while (head->previous) {
        head = head->previous;
    }
    head->previous = newNode;
    newNode->next = head;

}

template <class T>
void List<T>::insertBack(T value) {
    Node *newNode = new Node(value);
    newNode->previous = nullptr;
    newNode->next = nullptr;
    
    Node *head = node;
    while (head->next) {
        head = head->next;
    }
    head->next = newNode;
    newNode->previous = head;
}

template <class T>
void List<T>::insertAt(T value, int index) {
    
}

template <class T>
void List<T>::removeAt(int index) {
    
}

template <class T>
void List<T>::clear() {
    
}

template <class T>
void List<T>::printNode() {
    
}

template <>
struct List<int> {
    
};

#endif /* List_hpp */
