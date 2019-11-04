//
//  main.cpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#include <iostream>
#include <sys/_types/_offsetof.h>
#include "error.hpp"
#include <pthread.h>

static jmp_buf buf;
static int flags = 0;
void seconds() {
    flags++;
    longjmp(buf, flags);
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

struct alarm_t {
    char message[64];
    int seconds;
};

void *clock_handler(void *buffer) {
    char retBuffer[128] = "执行结束";
//    pthread_t current = pthread_self();
//    std::cout << "当前线程" << current->__sig << std::endl;
    
    alarm_t *alarm = static_cast<alarm_t *>(buffer);
    int status = 0;
    status = pthread_detach(pthread_self());
    if (status != 0) {
        assert(false);
    }
    sleep(alarm->seconds);
    printf("子线程 seconds= %d %s \n", alarm->seconds, alarm->message);
//    free(alarm);
    pthread_exit(retBuffer);
}

void *thread_handler(void *buffer) {
    char *message = static_cast<char *>(buffer);
    const char *buf = "son thread";
    printf("执行子线程 %s \n", message);
    sleep(2);
    return (void *)buf;
}
#include "Mutex.hpp"

int main(int argc, const char * argv[]) {
    
    {
        using namespace Thread;
        using namespace boost;
        typedef shared_ptr<Tag> tag_share_ptr;
//        Tag tag = {PTHREAD_MUTEX_INITIALIZER, 0};
        tag_share_ptr tag = shared_ptr<Tag>(new Tag());
        int status = 0;
        status = pthread_mutex_init(&tag->mutex, NULL);
        if (status != 0) {
            err_abort(status, "mutex initial error");
        }
        status = pthread_mutex_destroy(&tag->mutex);
        if (status != 0) {
            err_abort(status, "mutex destory error");
        }
    }
    return 0;
    {
        pthread_t thread;
        char buffer[256] = "this join thread";
        int status = 0;
        
        pthread_create(&thread, NULL, thread_handler, buffer);
        void *thread_result = nullptr;
        sleep(5);
        /// join 线程就已经开始执行了
        status = pthread_join(thread, &thread_result);
        /// 等待线程执行完毕
        if (status != 0) {
            err_abort(status, "Create thread");
        }
        fprintf(stdout,"%s \n", static_cast<char *>(thread_result));
        
    }
    return 0;
    {
        /*
         api
         */
//        pthread_equal(<#pthread_t _Nullable#>, <#pthread_t _Nullable#>)
//        pthread_create(<#pthread_t  _Nullable * _Nonnull#>, <#const pthread_attr_t * _Nullable#>, <#void * _Nullable (* _Nonnull)(void * _Nullable)#>, <#void * _Nullable#>)
//        pthread_self()
//        sched_yield()
        
    }
    return 0;
    {
        pthread_key_t key;
//        pthread_setspecific(<#pthread_key_t#>, <#const void * _Nullable#>)
//        pthread_getspecific(<#pthread_key_t#>)
//        err_abort(1,"1231");
//        errno_abort("a");
    }
    return 0;
    {
        int status = 0;
        pthread_t thread = nullptr;
        status = pthread_join(thread, NULL);
        /// 获取error输出
        if (status != 0) {
            fprintf(stderr, "error = %d, info = %s \n", status, strerror(status));
        }
    }
    
    return 0;
    
    /// 线程版clock
    {
        
        pthread_t current = pthread_self();
        std::cout << "主线程" << current->__sig << std::endl;
        /// 数据
        alarm_t *alarm;
        /// 线程
        pthread_t clock_thead;
        /// stdin读取的数据
        char lines[128];
        /// 状态
        int status = 0;
        
        while (1) {
            /// 读取数据
            if (fgets(lines, sizeof(lines), stdin) == NULL) {
                assert(false);
            }
            if (strlen(lines) < 2) {
                printf("输入有误 请输入加空格: ");
                continue;
            }
            alarm = (alarm_t*)malloc(sizeof(alarm_t));
            if (alarm == nullptr) {
                assert(false);
            }
            
            
            printf("lines == %s \n", lines);
            if (sscanf(lines, "%d, %64[^\n]", &alarm->seconds, alarm->message) < 2) {
                printf("message %d === %s \n", alarm->seconds, alarm->message);
                fprintf(stderr, "error input \n");
                free(alarm);
            }
            status = pthread_create(&clock_thead, NULL, clock_handler, alarm);
            if (status != 0) {
                free(alarm);
                assert(false);
            }
            
        }
        
        
        
    }
    
    return 0;
    
    ///  进程版clock
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
