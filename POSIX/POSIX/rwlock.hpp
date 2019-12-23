//
//  rwlock.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/23.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef rwlock_hpp
#define rwlock_hpp

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

typedef struct rwlock_tag {
    pthread_mutex_t mutex;
    pthread_cond_t  write;
    pthread_cond_t  read;
    int             valid;
    int             r_active;
    int             w_active;
    int             r_wait;
    int             w_wait;
    
}rwlock_t;

#define RWLOCK_VALID 0xfacade
#define RXLOCK_INTIIAL \
{\
   .mutex = PTHREAD_MUTEX_INITIALIZER, \
   .write = PTHREAD_COND_INITIALIZER, \
   .read  = PTHREAD_COND_INITIALIZER, \
   .valid = RWLOCK_VALID,           \
   .r_active = 0,\
   .w_active = 0,\
   .r_wait   = 0,\
   .w_wait   = 0 \
}

extern int rwlock_init(rwlock_t *rwlock);
extern int rwlock_destory(rwlock_t *rwlock);
extern int rwlock_read(rwlock_t *);
extern int rwlock_try_read(rwlock_t *);
extern int rwlock_read_unlock(rwlock_t *);
extern int rwlock_write(rwlock_t *);
extern int rwlock_try_write(rwlock_t *);
extern int rwlock_write_unlock(rwlock_t *);


int rwlock_init(rwlock_t *rwlock) {
    
    rwlock = (rwlock_t *)malloc(sizeof(rwlock_t));
    rwlock->r_active = 0;
    rwlock->w_active = 0;
    pthread_mutex_init(&rwlock->mutex, NULL);
    pthread_cond_init(&rwlock->read, NULL);
    pthread_cond_init(&rwlock->write, NULL);
    rwlock->valid = RWLOCK_VALID;
    return 0;
}

int rwlock_destory(rwlock_t *rwlock) {
    
    int status;
    
    if (rwlock->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    status = pthread_mutex_unlock(&rwlock->mutex);
    
    if (status != 0) {
        return status;
    }
    
    if (rwlock->r_active > 0 || rwlock->w_active > 0) {
        pthread_mutex_unlock(&rwlock->mutex);
        return EBUSY;
    }
    
    if (rwlock->r_wait > 0 || rwlock->w_wait > 0) {
        pthread_mutex_unlock(&rwlock->mutex);
        return EBUSY;
    }
    
    rwlock->valid = 0;
    
    pthread_mutex_destroy(&rwlock->mutex);
    pthread_cond_destroy(&rwlock->read);
    pthread_cond_destroy(&rwlock->write);
    
    return 0;
}

static void rwlock_read_cleanup(void *arg) {
    rwlock_t *rwlock = (rwlock_t *)arg;
    rwlock->r_wait--;
    pthread_mutex_unlock(&rwlock->mutex);
}

static void rwlock_write_cleanup(void *arg) {
    rwlock_t *rwlock = (rwlock_t *)arg;
    rwlock->w_wait--;
    pthread_mutex_unlock(&rwlock->mutex);
}

int rwlock_read(rwlock_t *rwl) {
    int status;
    if (rwl->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    status = pthread_mutex_lock(&rwl->mutex);
    
    /*
     如果 write active 大于0  代表有写正在进行  就需要等待
     r_wait++
     */
    if (rwl->w_active) {
        rwl->r_wait++;
        pthread_cleanup_push(rwlock_read_cleanup, (void*)rwl);
        /*
         如果有 write 激活状态 就等待 read 条件
         */
        while (rwl->w_active) {
            status = pthread_cond_wait(&rwl->read, &rwl->mutex);
        }
        pthread_cleanup_pop(0);
    }
    
    if (status == 0) {
        rwl->r_active++;
    }
    pthread_mutex_unlock(&rwl->mutex);
    return status;
}

int rwlock_try_read(rwlock_t *rwl) {
    int status;
    if (rwl->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    status = pthread_mutex_lock(&rwl->mutex);
    
    /*
     如果写正在进行 就返回busy
     否则read_active++
     */
    if (rwl->w_active) {
        status = EBUSY;
    } else {
        rwl->r_active++;
    }
    
    pthread_mutex_unlock(&rwl->mutex);
    
    return status;
}

int rwlock_read_unlock(rwlock_t *rwl) {
    int status;
    if (rwl->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    status = pthread_mutex_lock(&rwl->mutex);
    rwl->r_active--;
    
    /*
     如果不存在读进行  并且 写 等待大于0  发信号
     */
    if (rwl->r_active == 0 && rwl->w_wait > 0) {
        pthread_cond_signal(&rwl->write);
    }
    pthread_mutex_unlock(&rwl->mutex);
    return status;
}

int rwlock_write(rwlock_t *rwl) {
    int status;
    if (rwl->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    status = pthread_mutex_lock(&rwl->mutex);
    /*
     如果存在write_active 写
     并且 read_active > 0
     w_wait++
     */
    if (rwl->w_active || rwl->r_active > 0) {
        rwl->w_wait++;
        pthread_cleanup_push(rwlock_write_cleanup, (void *)rwl);
        /*
         
         */
        while (rwl->w_active || rwl->r_active > 0) {
            pthread_cond_wait(&rwl->write, &rwl->mutex);
        }
        pthread_cleanup_pop(0);
        rwl->w_wait--;
    }
    pthread_mutex_unlock(&rwl->mutex);
    return status;
}

int rwlock_try_write(rwlock_t *rwl) {
    int status;
    if (rwl->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    
    status = pthread_mutex_lock(&rwl->mutex);
    if (status != 0) {
        return status;
    }
    
    if (rwl->w_active || rwl->r_active > 0) {
        status = EBUSY;
    } else {
        rwl->w_active = 1;
    }
    
    pthread_mutex_unlock(&rwl->mutex);
    
    return status;
}

int rwlock_write_unlock(rwlock_t *rwl) {
    int status;
    if (rwl->valid != RWLOCK_VALID) {
        return EINVAL;
    }
    
    status = pthread_mutex_lock(&rwl->mutex);
    rwl->w_active = 0;
    if (rwl->r_wait > 0) {
        status = pthread_cond_broadcast(&rwl->read);
    } else if (rwl->w_wait > 0) {
        pthread_cond_signal(&rwl->write);
    }
    pthread_mutex_unlock(&rwl->mutex);
    return status;
}

typedef struct thread_tag {
    int             thread_num;
    pthread_t       thread;
    int             updates;
    int             reads;
    int             interval;
}thread_t;

typedef struct data_tag {
    rwlock_t lock;
    int      data;
    int      updates;
}data_t;

#define ITERATIONS 10000

thread_t threads[5];
data_t data[15];

void *thread_routine(void *arg) {
    thread_t *self = (thread_t *)arg;
    
    int repeats = 0;
    int iteration;
    int element = 0;
    int status;
    
    for (iteration = 0; iteration < ITERATIONS; iteration++) {
        if (iteration % self->interval == 0) {
            status = rwlock_write(&data[element].lock);
            data[element].data = self->thread_num;
            data[element].updates++;
            self->updates++;
            rwlock_write_unlock(&data[element].lock);
        } else {
            status = rwlock_read(&data[element].lock);
            self->reads++;
            if (data[element].data == self->thread_num) {
                repeats++;
            }
            rwlock_read_unlock(&data[element].lock);
        }
        element++;
        if (element >= 15) {
            element = 0;
        }
    }
    
    if (repeats > 0) {
        printf("------------------ \n");
    }
    
    return nullptr;
}

#endif /* rwlock_hpp */
