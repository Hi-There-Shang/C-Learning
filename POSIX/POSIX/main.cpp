//
//  main.cpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#include <iostream>
#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#include <sys/_types/_offsetof.h>
#include "error.hpp"

static jmp_buf buf;
static int flags = 0;
void seconds() {
    flags++;
    longjmp(buf, flags);
    printf("111111111 \n");
}

void first() {
    if (setjmp(buf) == 3) {
        printf("hello world \n");
    } else {
        ///
        printf("递归 \n");
        seconds();
    }
    printf("end \n");
}


struct user {
    short name;
    int age;
    double prices;
};

int main(int argc, const char * argv[]) {
    {
        int status;
        char lines[128];
        int seconds = 0;
        pid_t pid;
        char message[64];
        
        /// 死循环
        while (1) {
            printf("Alarm> ");
            /// 读取用户输入
            if (fgets(lines, sizeof(lines), stdin) == NULL) {
                /// 未输入 直接退出
                exit(0);
            }
            /// 判断写入数据的长度
            if (strlen(lines) <= 1) {
                continue;
            }
            
            /// 格式化用户输入
            if (sscanf(lines, "%d %64[^\n]", &seconds, message) < 2) {
                fprintf(stderr, "error \n");
            } else {
                /// 开启子进程
                pid = fork();
                /// 如果是当前进程 就失败了
                if (pid == (pid_t) - 1) {
                    assert(false);
                }
                
                /// pid == 0 代表是子进程
                if (pid == 0) {
                    /// 睡眠
                    sleep(seconds);
                    printf("%d -- %s", seconds, message);
                } else {
                    /// 父进程 循环
                    do {
                        pid = waitpid((pid_t) - 1, NULL, WNOHANG);
                        if (pid == (pid_t) - 1) {
                            assert(false);
                        }
                    } while (pid  != 0);
                }
            }
            
        }
    }
    return 0;
    {
        char lines[512] = "1-2-3";
        int num1;
        int num2;
//        scanf("%s", lines);
        sscanf(lines, "%d-%d", &num1, &num2);
        std::cout << lines << "------" << num1 << "--------" << num2 << std::endl;
    }
    return 0;
    {
        char buff[1024];
        
        memset( buff, '\0', sizeof( buff ));
        
        fprintf(stdout, "启用全缓冲\n");
        setvbuf(stdout, buff, _IOFBF, 1024);
        
        fprintf(stdout, "这里是 runoob.com\n");
        fprintf(stdout, "该输出将保存到 buff\n");
        fflush( stdout );
        
        fprintf(stdout, "这将在编程时出现\n");
        fprintf(stdout, "最后休眠五秒钟\n");
        
        sleep(5);
    }
    
    return 0;
    
    {
        int ageOffset = offsetof(struct user, age);
        int pricesOffset = offsetof(struct user, prices);
        int nameOffset = offsetof(struct user, name);
        std::cout << sizeof(struct user) << std::endl;
        std::cout << "--------" << std::endl;
        std::cout << sizeof(short) << std::endl;
        std::cout << sizeof(int) << std::endl;
        std::cout << sizeof(long) << std::endl;
        std::cout << "--------" << std::endl;
        std::cout << ageOffset << std::endl;
        std::cout << pricesOffset << std::endl;
        std::cout << nameOffset << std::endl;
    }
    
    return 0;
    first();
    return 0;
    int seconds = 0;
    char line[128] = {};
    char message[64] = {};
    while (1) {
        printf("arlms \n");
        line[1] = '1';
        if (fgets(line, sizeof(line), stdin) == NULL) {
            exit(0);
        }
        if (strlen(line) <= 1) {
            continue;
        }
        if(sscanf(line, "%d %64[^\n]", &seconds, message) < 2) {
            fprintf(stderr, "Bad command \n");
        } else {
            sleep(seconds);
            printf("%d %s \n", seconds, message);
        }
    }
    return 0;
}
