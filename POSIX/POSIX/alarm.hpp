//
//  alarm.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef alarm_hpp
#define alarm_hpp

#include <stdio.h>
#include "alarm_data.hpp"


static alarm_tag *alarm_list = nullptr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* alart_thread(void * buffer) {
    alarm_tag *alarm = nullptr;//static_cast<alarm_tag*>(buffer);
    long sleep_time = 0;
    time_t now;
    int status = 0;
    
    while (1) {
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            err_abort(status, "lock error");
        }
        alarm = alarm_list;
        printf("子线程 %p \n", alarm_list);
        if (alarm == nullptr) {
            sleep_time = 1;
        } else {
            printf("子线程 ----------- %p \n", alarm);
            alarm_list = alarm->link;
            now = time(NULL);
            if (alarm->time < now) {
                sleep_time = 0;
            } else {
                sleep_time = alarm->time - now;
                printf("waiting %ld (%ld) \"(%s)\"", alarm->time, sleep_time, alarm->message);
            }
        }
        
        status = pthread_mutex_unlock(&mutex);
        
        if (status != 0) {
            err_abort(status, "unLock error");
        }
        
        if (sleep_time > 0) {
            sleep((int)sleep_time);
        } else {
            sched_yield();
        }
        
        if (alarm != nullptr) {
            printf("waiting %ld (%ld) \"(%s)\"", alarm->time, sleep_time, alarm->message);
            free(alarm);
        }
    }
    pthread_exit(nullptr);
}

void test_alarm() {
    int status = 0;
    char line[128];
    alarm_tag *alarm, **last, *next;
    pthread_t thread;
    
    
    /// 创建线程
    status = pthread_create(&thread, NULL, alart_thread, NULL);
    if (status != 0) {
        err_abort(status, "thread error");
    }
    
    /// event loop
    while(1) {
        printf("Alarm>: ");
        
        /// 获取用户输入
        if (fgets(line, sizeof(line), stdin) == NULL) {
            exit(0);
        }
        
        if(strlen(line) < 1) continue;
        
        /// 创建数据
        alarm = (alarm_tag *)malloc(sizeof(alarm_tag));
        
        if (alarm == nullptr) {
            err_abort(1, "error init");
        }
        
        /// 格式化数据   定时器的时间 和 输入的message信息--->写入用户数据
        if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) {
            fprintf(stderr, "error input \n");
            free(alarm);
        } else {
            /// 加锁
            /// 加锁 同步数据---->全局区的 alarm_list ---->判断时间--->
            status = pthread_mutex_lock(&mutex);
            if (status != 0) {
                err_abort(status, "lock error");
            }
            
            /// 记录时间   当前时间 和 定时器的时间
            alarm->time = time(NULL) + alarm->seconds;
            
            /// 第一次 为空
            /*
             地址--->指向同一片内存空间
             */
            last = &alarm_list;
            ///
            ///
            /*
             *alarm, **last, *next;
             */
            
            printf("主线程 %p \n", alarm_list);
            next = *last; // head指针
            while (next != nullptr) {
                /// 时间判断--->时间大的往后排
                if (next->time >= alarm->time) {
                    alarm->link = next;
                    *last = alarm;
                    break;
                }
                
                last = &next->link;
                next = next->link;
            }
            
            /// 赋值
            if (next == nullptr) {
                *last = alarm;
                printf("--赋值--- %p \n", alarm_list);
                alarm->link = nullptr;
            }
            
            printf("[list: ");
            for (next = alarm_list; next != nullptr; next = next->link) {
                printf(" %ld--%ld--%s ", next->time, next->time - time(NULL), next->message);
            }
            printf(" ] \n");
            
            status = pthread_mutex_unlock(&mutex);
            if (status != 0) {
                err_abort(status, "unlock error");
            }
        }
        
    }
}

struct __book {
    int age;
    int time;
    __book *link;
};

static __book *book_list;

int main321123() {
    __book *book, **books, *next;
    book = (__book *)malloc(sizeof(__book));
    book->age = 10;
    book->time = 11;
    book->link = nullptr;
    books = &book_list;
//    next = *books;
//    *books = book;
    printf("%p \n", book_list);
    
    test_alarm();
    return 0;
}

#endif /* alarm_hpp */
