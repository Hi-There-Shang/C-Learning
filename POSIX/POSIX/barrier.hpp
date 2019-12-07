//
//  barrier.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/7.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef barrier_hpp
#define barrier_hpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

typedef struct barrier_tag_s {
    pthread_mutex_t     mutex;
    pthread_cond_t      barrier_cond;
    int                 valid;
    int                 threshold;
    int                 counter;
    unsigned long       cycle;
}barrier_t;

#define BARRIER_VALID 0xdbcafe

#define BARRIER_INITIALIZER(cnt)                  \
{                                                 \
    .mutex           = PTHREAD_MUTEX_INITIALIZER, \
    .barrier_cond    = PTHREAD_COND_INITIALIZER,  \
    .valid           = BARRIER_VALID,             \
    .threshold       = cnt,                       \
    .counter         = cnt,                       \
    .cycle           = 0                          \
}

extern int barrier_init(barrier_t *, int);
extern int barrier_destory(barrier_t *);
extern int barrier_wait(barrier_t *);

int barrier_init(barrier_t *barrier, int counter) {
    barrier = (barrier_t *)malloc(sizeof(barrier_t));
    barrier->threshold = counter;
    barrier->counter   = counter;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->barrier_cond, NULL);
    barrier->valid = BARRIER_VALID;
    return 0;
}

int barrier_destory(barrier_t *barrier) {
    int status;
    int status2;
    
    if (barrier == NULL) {
        return EINVAL;
    }
    
    if (barrier->valid != BARRIER_VALID) {
        return EINVAL;
    }
    
    status = pthread_mutex_lock(&barrier->mutex);
    
    if (barrier->threshold != barrier->counter) {
        pthread_mutex_unlock(&barrier->mutex);
        return EBUSY;
    }
    
    barrier->valid = 0;
    pthread_mutex_unlock(&barrier->mutex);
    
    status = pthread_mutex_destroy(&barrier->mutex);
    status2 = pthread_cond_destroy(&barrier->barrier_cond);
    
    return status != 0 ? status : status2;
}

int barrier_wait(barrier_t *barrier) {
    
    unsigned long cycle;
    int cancel;
    int tmp;
    
    if (barrier->valid != BARRIER_VALID) {
        return EINVAL;
    }
    
    pthread_mutex_lock(&barrier->mutex);
    
    cycle = barrier->cycle;
    
    if (--barrier->counter == 0) {
        barrier->cycle++;
        barrier->counter = barrier->threshold;
        pthread_cond_broadcast(&barrier->barrier_cond);
    } else {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancel);
        
        while (cycle == barrier->cycle) {
            pthread_cond_wait(&barrier->barrier_cond, &barrier->mutex);
        }
        
        pthread_setcancelstate(cancel, &tmp);
    }
    pthread_mutex_unlock(&barrier->mutex);
    return 0;
}

void initial() {
    //barrier_t barrier = BARRIER_INITIALIZER(3);
}

#endif /* barrier_hpp */
