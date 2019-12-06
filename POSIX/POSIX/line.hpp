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


int pipe_send_(stage_t *stage, long data) {
    
    pthread_mutex_lock(&stage->mutex);
    
    /*
      data_ready == 1 死循环
     */
    while (stage->data_ready) {
        pthread_cond_wait(&stage->ready, &stage->mutex);
    }
    
    stage->data = data;
    stage->data_ready = 1;
    pthread_cond_signal(&stage->avail);
    
    return 0;
}

int pipe_start_(pipe_t *pipe, long value) {
    
    pthread_mutex_lock(&pipe->mutex);
    pipe->active++;
    pthread_mutex_unlock(&pipe->mutex);
    pipe_send_(pipe->head, value);
    
    return 0;
}

int pipe_result_(pipe_t *pipe, long *result) {
   
    return 0;
}

void *pipe_stage_(void *arg) {
    
    stage_t *stage = (stage_t *)arg;
    stage_t *next_stage = stage->next;
    
    pthread_mutex_lock(&stage->mutex);
    
    while (1) {
        
        while (stage->data_ready != 1) {
            pthread_cond_wait(&stage->avail, &stage->mutex);
        }
        
        pipe_send_(next_stage, stage->data + 1);
        
        stage->data_ready = 0;
        pthread_cond_signal(&stage->ready);
    }
   
    pthread_mutex_unlock(&stage->mutex);
    
    return NULL;
}

int pipe_create_(pipe_t *pipe, int stages) {
    int stage_index = 0;
    pipe = (pipe_t *)malloc(sizeof(pipe_t));
    stage_t **link = &pipe->head;
    stage_t *new_stage = nullptr;
    /*
     pthread_mutex_t     mutex;
     stage_t             *head;
     stage_t             *tail;
     int                 stages;
     int                 active;
     */
    pthread_mutex_init(&pipe->mutex, NULL);
    pipe->stages = stages;
    pipe->active = 0;
    
    for (stage_index = 0; stage_index < stages; stage_index++) {
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
    
    for (stage_t *head = pipe->head; head != NULL; head = head->next) {
        pthread_create(&head->thread, NULL, pipe_stage_, (void *)head);
    }
    
    return 0;
}

int main() {
    pipe_t pipe;
    long result;
    long value;
    char line[128];
    
    pipe_create_(&pipe, 10);
    
    printf("enter value or \"=\" for result \n");
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL)  exit(0);
        if (strlen(line) <= 1) continue;
        if (strlen(line) <= 2 && line[0] == '=') {
            if (pipe_result_(&pipe, &result)) {
                printf("result = %ld \n", result);
            } else {
                printf("pipe is empty \n");
            }
        } else {
            if (sscanf(line, "%ld", &value) < 1) {
                fprintf(stderr, "error input \n");
            } else {
                pipe_start_(&pipe, value);
            }
        }
    }
    
    return 0;
}

typedef struct _object_ {
    int data;
    struct _object_ *next;
}object;

typedef struct _manager_ {
    object *head;
    object *tail;
}manager;

void test_pointer_link() {
    
    manager *ma = (manager *)malloc(sizeof(manager));
    
//    object *o1 = (object *)malloc(sizeof(object));
//    object *o2 = (object *)malloc(sizeof(object));
//    object *o3 = (object *)malloc(sizeof(object));
    
    object **link = &ma->head;
    printf("ma->head = %p \n", ma->head);
    printf("&ma->head = %p \n", &ma->head);
    printf("link = %p \n", link);
    printf("*link = %p \n", *link);
    
    for (int i = 0; i < 3; i++) {
        object *o = (object *)malloc(sizeof(object));
        o->data = i + 1;
        o->next = NULL;
        
        /// 这是赋值的地方
        *link = o;
        printf("之前 i = %d  head = %p tail = %p link = %p  *link = %p , o = %p \n", i + 1, ma->head, ma->tail, link , *link, o);
        
        /// 这是获取地址 --> &o->next
        link = &o->next;
        printf("之后 i = %d  head = %p tail = %p link = %p  *link = %p , o = %p \n", i + 1, ma->head, ma->tail, link , *link, o);
    }
    
    object *head = ma->head;
    
    while (head != NULL) {
        printf("data = %d ---- %p\n", (head)->data, head);
        head = (head)->next;
    }
}
#endif /* line_hpp */
