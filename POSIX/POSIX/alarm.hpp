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

void alart_thread() {
    alarm_tag *alarm = nullptr;
    long sleep_time = 0;
    time_t now;
    int status = 0;
    
    while (1) {
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            err_abort(status, "lock error");
        }
        alarm = alarm_list;
        
        if (alarm == nullptr) {
            sleep_time = 1;
        } else {
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
}

void test_alarm() {
    
}

#endif /* alarm_hpp */
