//
//  Counter.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/6.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef Counter_hpp
#define Counter_hpp

#include <stdio.h>
#include "error.hpp"

#define ASSERT(status) if (status != 0) {  \
                          err_abort(status, "lock error");  \
                       }

#define HELLO 1

#ifdef HELLO
#define SPIN 10000000
#endif

pthread_mutex_t __mutex = PTHREAD_MUTEX_INITIALIZER;
long __counter = 0;
time_t __end_time;

void* __counter_thread(void *buffer)
{
    int status = 0;
    int spin = 0;
    
    while (time(NULL) <= __end_time) {
        status = pthread_mutex_lock(&__mutex);
        ASSERT(status);
        for (spin = 0; spin < SPIN; spin++) {
            __counter++;
        }
        status = pthread_mutex_unlock(&__mutex);
        ASSERT(status);
        sleep(1);
    }
    printf("counter == %ld \n", __counter);
    return nullptr;
}

void* __monitor_thread(void *buffer)
{
    int status = 0;
    int misses = 0;
    
    while (time(NULL) < __end_time) {
        sleep(3);
        status = pthread_mutex_trylock(&__mutex);
        if (status != EBUSY) {
            printf("monitor Counter == %ld \n", __counter/SPIN);
            status = pthread_mutex_unlock(&__mutex);
            ASSERT(status);
        } else {
            misses++;
        }
    }
    printf("misses count = %d \n", misses);
    return nullptr;
}

void test_counter() {
    int status = 0;
    pthread_t counter_thread;
    pthread_t monitor_thread;
    
    __end_time = time(NULL) + 60;
    status = pthread_create(&counter_thread, NULL, __counter_thread, NULL);
    ASSERT(status);
    status = pthread_create(&monitor_thread, NULL, __monitor_thread, NULL);
    ASSERT(status);
    status = pthread_join(counter_thread, NULL);
    ASSERT(status);
    status = pthread_join(monitor_thread, NULL);
    ASSERT(status);
}

int main() {
    test_counter();
    return 0;
}

#endif /* Counter_hpp */
