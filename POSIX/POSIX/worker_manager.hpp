//
//  worker_manager.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/14.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef worker_manager_hpp
#define worker_manager_hpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

typedef struct workq_ele_tag {
    struct workq_ele_tag *next;
    void*                data;
}workq_ele_t;

typedef struct workq_tag {
    pthread_mutex_t      mutex;
    pthread_cond_t       cv;
    pthread_attr_t       attr;
    workq_ele_t          *first;
    workq_ele_t          *last;
    int                  valid;
    int                  quit;
    int                  parallelism;
    int                  counter;
    int                  idle;
    void                 (*engine)(void *arg);
}workq_t;

#define WORKQ_VALID 0xdec1992;

extern int workq_init(workq_t *wq, int threads, void (*engine)(void *arg));
extern int workq_destory(workq_t *wq);
extern int workq_add(workq_t *wq, void *data);


int workq_init(workq_t *wq, int threads, void (*engine)(void *arg)) {
    wq = (workq_t *)malloc(sizeof(workq_t));
    pthread_attr_init(&wq->attr);
    pthread_attr_setdetachstate(&wq->attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&wq->mutex, NULL);
    pthread_cond_init(&wq->cv, NULL);
    wq->quit = 0;
    wq->last = wq->first = NULL;
    wq->parallelism = threads;
    wq->counter = 0;
    wq->idle = 0;
    wq->engine = engine;
    wq->valid = WORKQ_VALID;
    return 0;
}



#endif /* worker_manager_hpp */
