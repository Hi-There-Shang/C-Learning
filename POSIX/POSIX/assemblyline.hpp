//
//  assemblyline.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef assemblyline_hpp
#define assemblyline_hpp

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

void *pipe_stage(void *arg) {
    
    return nullptr;
}

int pipe_create(pipe_t *pipe, int stages) {
    
    int pipe_index = 0;
    stage_t **link = &pipe->head, *new_stage = nullptr, *stage = nullptr;
    printf("%p - %p \n", new_stage, stage);
    
//    int status;
    pthread_mutex_init(&pipe->mutex, NULL);
    pipe->stages = stages;
    pipe->active = 0;
    
    for (pipe_index = 0; pipe_index <= stages; pipe_index++) {
        new_stage = (stage_t *)malloc(sizeof(stage_t));
        pthread_mutex_init(&new_stage->mutex, NULL);
        pthread_cond_init(&new_stage->avail, NULL);
        pthread_cond_init(&new_stage->ready, NULL);
        new_stage->data_ready = 0;
        *link = new_stage;
        link = &new_stage->next;
    }
    *link = NULL;
    pipe->tail = new_stage;
    
    for (stage = pipe->head; stage != NULL; stage = stage->next) {
        pthread_create(&stage->thread, NULL, pipe_stage, (void *)stage);
    }
    return 0;
}

int pipe_start(pipe_t *pipe, long value) {
    return 0;
}

int pipe_result(pipe_t *pipe, long *result) {
    return 0;
}

int main() {
    
    pipe_t pipe;
//    int status;
    long result;
    long value;
    char line[128];
    
    pipe_create(&pipe, 10);
    
    printf("enter value or \"=\" for result \n");
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL)  exit(0);
        if (strlen(line) <= 1) continue;
        if (strlen(line) <= 2 && line[0] == '=') {
            if (pipe_result(&pipe, &result)) {
                printf("result = %ld \n", result);
            } else {
                printf("pipe is empty \n");
            }
        } else {
            if (sscanf(line, "%ld", &value) <= 1) {
                fprintf(stderr, "error input \n");
            } else {
                pipe_start(&pipe, value);
            }
        }
    }
    
    return 0;
}

#endif /* assemblyline_hpp */
