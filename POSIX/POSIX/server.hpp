//
//  server.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/7.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef server_hpp
#define server_hpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

static int Client_threads_size = 4;
#define Read_flag            1
#define Write_flag           2
#define Quit_flag            3


typedef struct request_tag {
    struct request_tag      *next;
    int                     operation;
    int                     synchronous;
    int                     done_flag;
    pthread_cond_t          done;
    char                    prompt[32];
    char                    text[32];
}request_t;

typedef struct tty_server_tag {
    request_t               *first;
    request_t               *last;
    int                     running;
    pthread_mutex_t         mutex;
    pthread_cond_t          request;
}tty_server_t;

tty_server_t tty_server = {
    .first      = NULL,
    .last       = NULL,
    .running    = false,
    .mutex      = PTHREAD_MUTEX_INITIALIZER,
    .request    = PTHREAD_COND_INITIALIZER
};

int client_threads;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  client_done  = PTHREAD_COND_INITIALIZER;

void* tty_server_routine(void *arg) {
    
//    static pthread_mutex_t prompt_mutex = PTHREAD_MUTEX_INITIALIZER;
    request_t *request;
    int operation;
    size_t len;
//    int status = 0;
    printf("--- first %p \n", tty_server.first);
    while (1) {
        pthread_mutex_lock(&tty_server.mutex);
         printf("--- first %p \n", tty_server.first);
        
        while (tty_server.first == NULL) {
            printf("first == NULL 等待 \n");
            pthread_cond_wait(&tty_server.request, &tty_server.mutex);
        }
        printf("first == NULL 等待完成 \n");
        request = tty_server.first;
        tty_server.first = request->next;
        if (tty_server.first == NULL) {
            tty_server.first = NULL;
            tty_server.last  = NULL;
        }
        pthread_mutex_unlock(&tty_server.mutex);
        
        operation = request->operation;
        switch (operation) {
            case Read_flag:
                if (strlen(request->prompt) > 0) {
                    printf("------ prompt = %s \n", request->prompt);
                }
                if (fgets(request->text, sizeof(128), stdin) == NULL) {
                    request->text[0] = '\0';
                }
                len = strlen(request->text);
                if (len > 0 && request->text[len - 1] == '\n') {
                    request->text[len - 1] = '\0';
                }
                
                if (request->synchronous) {
                    pthread_mutex_lock(&tty_server.mutex);
                    request->done_flag = 1;
                    pthread_cond_signal(&request->done);
                    pthread_mutex_unlock(&tty_server.mutex);
                } else {
                    free(request);
                }
                
                break;
            case Write_flag:
                puts(request->text);
                break;
            case Quit_flag:
                break;
                
            default:
                break;
        }
    }

    return NULL;
}

void tty_server_request(int operation, int sync, const char *prompt, char *string) {
    request_t *request;
    
    pthread_mutex_lock(&tty_server.mutex);
    if (!tty_server.running) {
        pthread_t thread;
        pthread_attr_t attr;
        printf("创建线程 执行 \n");
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        tty_server.running = 1;
        pthread_create(&thread, &attr, tty_server_routine, NULL);
        pthread_attr_destroy(&attr);
    }
    pthread_mutex_unlock(&tty_server.mutex);
    sleep(1);
    
    pthread_mutex_lock(&tty_server.mutex);
    request = (request_t *)malloc(sizeof(request_t));
    request->next = NULL;
    request->operation = operation;
    request->synchronous = sync;
    
    if (sync) {
        request->done_flag = 0;
        pthread_cond_init(&request->done, NULL);
    }
    
    if (prompt != NULL) {
        strncmp(request->prompt, prompt, 32);
    } else {
        request->prompt[0] = '\0';
    }
    
    if (string != NULL && operation == Write_flag) {
        printf("1111--- string %s \n", string);
        strncmp(request->text, string, 128);
    } else {
        request->text[0] = '\0';
    }
    
    if (tty_server.first == NULL) {
        tty_server.first = request;
        tty_server.last = request;
    } else {
        tty_server.last->next = request;
        tty_server.last = request;
    }
    
    pthread_cond_signal(&tty_server.request);
    
    if (sync) {
        while (!request->done_flag) {
            printf("done_flag == 0 等待 \n");
            pthread_cond_wait(&request->done, &tty_server.mutex);
        }
        printf("done_flag == 0 等待完成 \n");
        
        if (operation == Read_flag) {
            if (strlen(request->text) > 0) {
                strcpy(string, request->text);
            } else {
                string[0] = '\0';
            }
        }
        pthread_cond_destroy(&request->done);
        free(request);
    }
    
    pthread_mutex_unlock(&tty_server.mutex);
}

void* client_routine(void *arg) {
    int *my_number = (int *)arg;
    printf("--- %d \n", *my_number);
    int loops;
    char prompt[32];
    char string[128];
    char formatted[128];
//    int status;
    sprintf(prompt, "Client %d > ", *my_number);
    while (1) {
        printf("prompt = %s string = %s \n", prompt, string);
        tty_server_request(Read_flag, 1, prompt, string);
        if (strlen(string) == 0) {
            break;
        }
        printf("string -- %s \n", string);
        for (loops = 0; loops < 4; loops++) {
            sprintf(formatted, "(%d#%d) %s ",*my_number, loops, string);
            tty_server_request(Write_flag, 0, NULL, formatted);
            sleep(1);
        }
    }
    
    pthread_mutex_lock(&tty_server.mutex);
    client_threads--;
    if (client_threads <= 0) {
        pthread_cond_signal(&client_done);
    }
    pthread_mutex_unlock(&tty_server.mutex);
    return NULL;
}

int main123321123321() {
    pthread_t thread;
    int count;
//    int status;
    
    pthread_setconcurrency(Client_threads_size);
    client_threads = Client_threads_size;
    for (count = 0; count < Client_threads_size; count++) {
        pthread_create(&thread, NULL, client_routine, (void *)&count);
    }
    
    pthread_mutex_lock(&client_mutex);
    
    while (client_threads > 0) {
        pthread_cond_wait(&client_done, &client_mutex);
    }
    
    pthread_mutex_unlock(&client_mutex);
    printf("All clients done \n");
    tty_server_request(Quit_flag, 1, NULL, NULL);
    return 0;
}

#endif /* server_hpp */
