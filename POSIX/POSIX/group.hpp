//
//  group.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/4.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef group_hpp
#define group_hpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define Size  4

typedef struct work_tag {
    struct work_tag *next;
    char            *path;
    char            *string;
}work_t;

struct crew_tag;

typedef struct worker_tag {
    struct worker_tag   *next;
    int                 index;
    pthread_t           thread;
    struct crew_tag *crew;
}worker_t;

typedef struct crew_tag {
    int                 crew_size;
    worker_t            crew[Size];
    long                work_count;
    work_t              *first;
    work_t              *last;
    pthread_t           thread;
    pthread_mutex_t     mutex;
    pthread_cond_t      go;
    pthread_cond_t      done;
}crew_t;

size_t path_max;
size_t name_max;

void *worker_routine(void *arg) {
    worker_t *mine = (worker_t *)arg;
    crew_t *crew = mine->crew;
    work_t *work;
    work_t *new_work;
    
    struct stat filestat;
    struct dirent *entry;
    int status;
    entry = (struct dirent *)malloc(sizeof(struct dirent) + name_max);
    
    pthread_mutex_lock(&crew->mutex);
    
    while (crew->work_count == 0) {
        pthread_cond_wait(&crew->go, &crew->mutex);
    }
    
    pthread_mutex_unlock(&crew->mutex);
    
    printf("start index = %d \n", mine->index);
    
    while (1) {
        pthread_mutex_lock(&crew->mutex);
        printf("crew %d top: first is %p, count is %ld \n",mine->index, crew->first, crew->work_count);
        
        while (crew->first == NULL) {
            pthread_cond_wait(&crew->go, &crew->mutex);
        }
        
        printf("crew %d top: woke is %p, count is %ld \n",mine->index, crew->first, crew->work_count);
        
        work = crew->first;
        crew->first = work->next;
        
        if (crew->first == NULL) {
            crew->last = NULL;
        }
        
        printf("crew %d top: leave is %p, last is %p ,count is %ld \n",mine->index, work ,crew->first, crew->work_count);
        
        pthread_mutex_unlock(&crew->mutex);
        
        lstat(work->path, &filestat);
        if (S_ISLNK(filestat.st_mode)) {
            printf("this is link \n");
        } else if (S_ISDIR(filestat.st_mode)) {
            DIR *directory;
            struct dirent *result;
            
            directory = opendir(work->path);
            if (directory == NULL) {
                fprintf(stderr, "can not open dir \n");
                continue;
            }
            
            while (1) {
                status = readdir_r(directory, entry, &result);
                if (status != 0) {
                    fprintf(stderr, "can not read dir \n");
                    break;
                }
                
                if (result == NULL) {
                    fprintf(stderr, "can not get dir \n");
                    break;
                }
                
                if (strcmp(entry->d_name, ".") == 0) {
                    continue;
                }
                
                if (strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                
                new_work = (work_t *)malloc(sizeof(work_t));
                new_work->path = (char *)malloc(sizeof(path_max));
                
                strcpy(new_work->path, work->path);
                strcat(new_work->path, "/");
                strcat(new_work->path, entry->d_name);
                new_work->string = work->string;
                new_work->next = nullptr;
                
                pthread_mutex_lock(&crew->mutex);
                
                if (crew->first == NULL) {
                    crew->first = new_work;
                    crew->last = new_work;
                } else {
                    crew->first->next = new_work;
                    crew->last = new_work;
                }
                
                crew->work_count++;
                printf("crew %d top: add is %p, first is %p ,count is %ld \n",mine->index, new_work ,crew->first, crew->work_count);
                pthread_cond_signal(&crew->go);
                pthread_mutex_unlock(&crew->mutex);
                
                closedir(directory);
            }
        } else if (S_ISREG(filestat.st_mode)) {
            FILE *search;
            char buffer[512];
        }
    }
    
    return nullptr;
}

int main() {
    return 0;
}
#endif /* group_hpp */
