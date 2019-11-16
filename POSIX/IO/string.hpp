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

int main() {
//    std::enable_if<<#bool#>>
    test_string();
    return 0;
}

#endif /* string_hpp */
