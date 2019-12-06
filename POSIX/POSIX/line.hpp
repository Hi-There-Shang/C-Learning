//
//  line.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/6.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef line_hpp
#define line_hpp

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

typedef struct stage_tag {
    pthread_mutex_t     mutex;
    pthread_cond_t      ready;
    pthread_cond_t      avail;
    int                 data_ready;
    long                data;
    pthread_t           thread;
    struct stage_tag    *next;
}stage_t;

typedef struct pipe_tag {
    pthread_mutex_t     mutex;
    stage_t             *head;
    stage_t             *tail;
    int                 stages;
    int                 active;
}pipe_t;


int pipe_send(stage_t *stage, long data) {
    return 0;
}

#endif /* line_hpp */
