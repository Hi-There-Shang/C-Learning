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
    
    int status;
    pthread_mutex_lock(&stage->mutex);
    
    while (stage->data_ready) {
        status = pthread_cond_wait(&stage->ready, &stage->mutex);
        if (status != 0) {
            pthread_mutex_unlock(&stage->mutex);
            return status;
        }
    }
    stage->data = data;
    stage->data_ready = 1;
    status = pthread_cond_signal(&stage->avail);
    if (status != 0) {
        pthread_mutex_unlock(&stage->mutex);
        return status;
    }
    pthread_mutex_unlock(&stage->mutex);
    return 0;
}

void *pipe_stage(void *arg) {
    
    stage_t *stage = (stage_t *)arg;
    stage_t *next_stage = stage->next;
    
    pthread_mutex_lock(&stage->mutex);
    while (1) {
        while (stage->data_ready != 1) {
            pthread_cond_wait(&stage->avail, &stage->mutex);
        }
        pipe_send(next_stage, stage->data + 1);
        stage->data_ready = 0;
        pthread_cond_signal(&stage->ready);
    }
   
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
    pthread_mutex_lock(&pipe->mutex);
    pipe->active++;
    pthread_mutex_unlock(&pipe->mutex);
    pipe_send(pipe->head, value);
    return 0;
}

int pipe_result(pipe_t *pipe, long *result) {
    stage_t *stage = pipe->tail;
    long value;
    int empty = 0;
    int status;
    pthread_mutex_lock(&pipe->mutex);
    
    if (pipe->active <= 0) {
        empty = 1;
    } else {
        pipe->active--;
    }
    pthread_mutex_unlock(&pipe->mutex);
    
    if (empty) {
        return 0;
    }
    
    pthread_mutex_unlock(&stage->mutex);
    while (!stage->data_ready) {
        pthread_cond_wait(&stage->avail, &stage->mutex);
    }
    
    *result = stage->data;
    stage->data_ready = 0;
    pthread_cond_signal(&stage->ready);
    pthread_mutex_unlock(&stage->mutex);
    return 1;
}

int main123454321() {
    
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
            if (sscanf(line, "%ld", &value) < 1) {
                fprintf(stderr, "error input \n");
            } else {
                pipe_start(&pipe, value);
            }
        }
    }
    
    return 0;
}

#endif /* assemblyline_hpp */
