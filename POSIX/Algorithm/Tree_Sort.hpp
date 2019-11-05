//
//  Tree_Sort.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Tree_Sort_hpp
#define Tree_Sort_hpp

#include <stdio.h>

void swap(int &lhs, int &rhs) {
    int tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

void buidHeap(int array[], int currentNode ,int size) {
    if (currentNode < size) {
    int left = currentNode * 2 ;
    int right = currentNode * 2 + 1;
    int max = currentNode;
    if (left < size) {
        if (array[left] >= array[max]) {
            max = left;
        }
    }
    
    if (right < size) {
        if (array[right] >= array[max]) {
            max = right;
        }
    }
    
    if (max != currentNode) {
        int temp = array[max];
        array[max] = array[currentNode];
        array[currentNode] = temp;
        
        buidHeap(array, max ,size);
    }
    }
    
}

void tree_sort(int array[], int size) {
    for (int i = size - 1; i >= 0; i--) {
        buidHeap(array, i, size);
    }
}

void tree_heap(int array[], int size) {
    
    /// 遍历 --->每一次递归 都找到了最大的元素 放到最后
    /// 所以 size - 1....
    ///
    for (int i = 0; i < size; i++) {
        /// 构建堆
        /// 重复的构建堆
        tree_sort(array, size - i);
        
        /// 出栈的时候 取出最大值 交换
        /// 继续循环---> size - 1
        int tmp = array[0];
        int last = array[size - 1 - i];
        array[0] = last;
        array[size - 1 - i] = tmp;
    }
}

#include <iostream>
int main1234() {
    int nums[] = {4,2,1,5,6,8,10,2,1};
    tree_heap(nums, 9);
    for (int i = 0; i < 9; i++) {
        std::cout << nums[i] << std::endl;
    }
    return 0;
}

#endif /* Tree_Sort_hpp */
