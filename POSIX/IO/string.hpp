//
//  string.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/14.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef string_hpp
#define string_hpp

#include <stdio.h>
#include <string>

using namespace std;
void test_string() {
    string name = "1234566";
    string sub_name = name.substr(0, 3);
    name.replace(1, 3, "qwert");
    
    printf("%s \n", name.c_str());
    
}
char * longestPalindrome(char * s){
    int length = strlen(s);
    char *cur = (char *)malloc(sizeof(char) * length);
    char *max = (char *)malloc(sizeof(char) * length);
    int m = 0;
    int n = 0;
    for(int i = 0; i <= length; i++) {
        int k = i;
        int j = i;
        while (k>=0 && j <= length && s[k] == s[j]) {
            k--;
            j++;
            cur[m++] = s[k];
            printf("%c \n", s[k]);
        }
        if (m > n) {
            n = m;
            m = 0;
            max = cur;
        }
        
    }
    return max;
}

int main10101010101010() {
//    std::enable_if<<#bool#>>
    test_string();
    
    char *str = "abcqdewdcba";
    char *sub = longestPalindrome(str);
    printf("%s \n", sub);
    return 0;
}

#endif /* string_hpp */
