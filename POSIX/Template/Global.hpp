//
//  Global.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/10.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Global_hpp
#define Global_hpp

#include <stdio.h>
#include "error.hpp"

template <typename T1, typename T2>
class Function: public Base {
public:
    Function(T1 t1, T2 t2): value1(t1), value2(t2) {}
    virtual void Logger() const {
        std::cout <<  value1 << value2 << std::endl;
    }
private:
    T1 value1;
    T2 value2;
};

template <typename T>
class Function<T, T> {
public:
    
};

template <typename T>
class Function<T*, T*> {
public:
    
};

void test_func() {
    
}

#define Row 8
#define Col 8
static int kings[Row][Col] = {0};

bool isValid(int i, int j) {
    
    /// 当前位置有king
    if (kings[i][j] == 1) {
        for (int k = 0; k < Col; k++) {
            if (kings[i][k] == 0 && k != j) {
                return true;
            }
        }
        
        for (int k = 0; k < Col; k++) {
            if (kings[k][j] == 0 && k != i) {
                return true;
            }
        }
        
        return false;
    }
    
    /// 越界
    if (i >= Row - 1) {
        return false;
    }
    
    /// 越界
    if (j >= Col - 1) {
        return false;
    }
    
    /// 对角线的处理
    for (int m = 0; m < Col; m++) {
        for (int k = 0; k < Row; k++) {
//            if (m == i) {
//                if (kings[m][k] == 1 && k != j) {
//                    return false;
//                }
//            }
//
//            if (k == j) {
//                if (kings[m][k] == 1 && m != i) {
//                    return false;
//                }
//            }
            
            if ((i + j) == (m + k)) {
                /// 对角线
                if (kings[m][k] == 1) {
                    return false;
                }
                
                /// 对角线
                if (kings[k][m] == 1) {
                    return false;
                }
            }
            
            if (abs(m - k) == abs(i - j)) {
                /// 对角线
                if (kings[m][k] == 1) {
                    return false;
                }
                
                /// 对角线
                if (kings[k][m] == 1) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

static int count = 0;

void findBestKingPosition(int k, int m) {
    
    if (m == Col - 1) {
        count++;
        printf("---------------------\n");
        for (int i = 0; i < Col; i++) {
            printf("-\t");
            for (int j = 0; j < Col; j++) {
                if (kings[i][j] == 1) {
                    printf("*\t");
                } else {
                    printf("-\t");
                }
//
            }
             printf("\n");
        }
        printf("---------------------\n");
        return;
    }
    
    for (int i = 0; i < 8; i++) {
        
        if (isValid(i, m)) {
            kings[i][m] = 1;
            findBestKingPosition(i, m + 1);
            kings[i][m] = 0;
        }
        
    }
}

int main() {
    findBestKingPosition(0, 0);
    printf("%d  \n", count);
    return 0;
}

#endif /* Global_hpp */
