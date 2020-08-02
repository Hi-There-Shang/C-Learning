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
#include <iostream>
#include <string.h>
#include <vector>
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


struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int l = 0;
        int r = 0;
        
        ListNode *left  = l1;
        ListNode *right = l2;
        ListNode *lLast = NULL;
        ListNode *rLast = NULL;
        while(left != NULL) {
            lLast = left;
            left = left->next;
            l++;
        }
        while(right != NULL){
            rLast = right;
            right = right->next;
            r++;
        }
        if(l <= r) {
            while(l <= r) {
                ListNode *newNode = new ListNode(0);
                lLast->next = newNode;
                l++;
            }
        } else {
            while(r <= l) {
                ListNode *newNode = new ListNode(0);
                rLast->next = newNode;
                r++;
            }
        }
        
        bool flag = 0;
        ListNode *lHead = l1;
        ListNode *rHead = l2;
        
        ListNode *retHead = new ListNode(0);
        
        while(r--) {
            if (lHead == nullptr || rHead == nullptr || r < 0) {
                break;
            }
            ListNode *newNode = new ListNode(0);
            
            if(flag == 1) {
                newNode->val = (lHead->val + rHead->val)%10 + 1;
            } else {
                newNode->val = (lHead->val + rHead->val)%10;
            }
            
            retHead->next = newNode;
             printf("%d---- \n", newNode->val);
            retHead = newNode;
            
            int tmpFlag = lHead->val + rHead->val + flag;
            flag = ((tmpFlag / 10) > 0) ? 1 : 0;
            lHead = lHead->next;
            rHead = rHead->next;
        }
       
        return retHead->next;
    }
};

class Solution1 {
public:
    std::string longestPalindrome(std::string s) {
        int length = s.length();
        int left = 0;
        int right = 0;
        int len = 1;
        int maxLen = 0;
        int maxStart = 0;
        
        for(int i = 0; i < length; i++) {
            left = i - 1;
            right = i + 1;
            
            while(left >= 0 && s.at(i) == s.at(left)) {
                left--;
                len++;
            }
            
            while(right < length && s.at(i) == s.at(right)) {
                right++;
                len++;
            }
            
            while(left >=0 && right < length && s.at(left) == s.at(right)) {
                len = len + 2;
                left--;
                right++;
            }
            
            if(len < maxLen) {
                maxLen = len;
                maxStart = left;
            }
            len = 0;
        }
        return s.substr(maxStart + 1, maxStart + maxLen + 1);
    }
};

void all(int nums[5], int k, int n) {
    if (k == n) {
        for (int i = 0; i < 3; i++) {
            printf("%d-", nums[i]);
        }
        printf("\n");
        return;
    }
    
    for (int i = 0; i < 3 ; i++) {
        
        /// 交换二个数
        std::swap(nums[i], nums[k]);
        
        /// 依次增加---->
        all(nums, k + 1, 3);
        
        /// 替换回来
        std::swap(nums[i], nums[k]);
        
        std::string s;
//        s.substr()
    }
}
using namespace std;
class Solution2 {
public:
   
    std::vector<int> path;
    vector<vector<int>> res;
    vector<int> candidates;
    
    void DFS(int satrt, int target) {
        if(target == 0) {
            res.push_back(path);
            return;
        }
        
        for (int i = satrt;
             i < this->candidates.size() && (target - this->candidates[i]) >= 0;
             i++) {
            path.push_back(this->candidates[i]);
            DFS(i, target - this->candidates[i]);
            path.pop_back();
        }
    }
    
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        std::sort(candidates.begin(), candidates.end());
        this->candidates = candidates;
        DFS(0, target);
        return res;
    }
};


/*
 冒泡排序
 */


void bubbleSort(int nums[], int size) {
    
    for (int i = 0; i < size; i++) {
        
        for (int j = i + 1; j < size; j++) {
            if (nums[i] > nums[j]) {
                std::swap(nums[i], nums[j]);
            }
        }
    }
    
    for (int i = 0; i < size; i++) {
        printf("%d  ", nums[i]);
    }
}

void quickSort() {
    
}

/// 正向遍历 和 逆向遍历  ----> 条件 是什么----->
void insertSort(int nums[], int size) {
  
    for (int i = 0; i < size; i++) {
        
        /// 这里没明白.....
        for (int j = size - 1; (j > 0 && nums[j - 1] > nums[j]); j--) {
            std::swap(nums[i], nums[j]);
        }
    }
    
    for (int i = 0; i < size; i++) {
        printf("%d  ", nums[i]);
    }
}

/// 基准位
void insertSort1(int nums[], int size) {
    int flag = 0;
    for (int i = 0; i < size; i++) {
        /// 取第一个数当做基准
        flag = nums[i];
    }
    
    for (int i = 0; i < size; i++) {
        printf("%d  ", nums[i]);
    }
}


void selectSort(int nums[], int size) {
    
    int min= 0;
    for (int i = 0; i < size; i++) {
        
        /// 找到最小的 index  默认第一个最小
        min = i;
        
        for (int j = i + 1; j < size; j++) {
            
            /// 与后面的比较  交换index
            if (nums[min] > nums[j]) {
                
                /// 记录index
                min = j;
            }
            
            /// 替换
            /// i是最小的 min比i的大
            std::swap(nums[i], nums[min]);
        }
    }
    
    for (int i = 0; i < size; i++) {
        printf("%d  ", nums[i]);
    }
}

int minCount(int used) {
    int un_used = 1024 - used;
    int res = 0;
    int min = 1024;
    for(int i = 0; i <= un_used/64; i++) {
        for(int j = 0; j <= un_used/16; j++) {
            for(int k = 0; k <= un_used/4; k++) {
                for(int m = 0; m <= un_used; m++) {
                    if((i * 64 + j * 16 + k * 4 + m) == un_used) {
                        res = i + j + k + m;
                        min = res < min ? res : min ;
                    }
                }
            }
        }
    }
    return min;
}

void swap(int i, int j, int **nums) {
    int tmp = *nums[i];
    *nums[i] = *nums[j];
    *nums[j] = tmp;
}

int findThird(int size, int *nums) {
    int res[size];
    int j = 0;
    for(int i = 0; i < size; i++) {
        for(int j = i + 1; j < size; j++) {
            if(nums[i] > nums[j]) {
                swap(i, j, &nums);
            }
        }
    }
    
    for(int i = 1; i < size; i++) {
        if(nums[i] == nums[i - 1]) {
            continue;
        }
        j++;
        if (j== 2) {
            return nums[i];
        }
    }
    
    return 0;
}

class Solution111 {
public:
    vector<int> multiply(const vector<int>& A) {
        int size = (int)A.size();
        vector<int> res;
        for(int i = size - 1; i >= 0; i--) {
            int tmp = bElement(i, A);
            res.push_back(tmp);
        }
        return res;
    }
    
    int bElement(int i, const vector<int>& A) {
        
        int j = i;
        int res = 1;
        while(j >= 0) {
            res *= A[j];
            j--;
        }
        
        return res;
    }
};

int main___1__() {
    
    vector<int> v{1,2,3,4,5};
    
    Solution111 s;
    
    vector<int> rs = s.multiply(v);
    for (int i = 0; i < rs.size(); i++) {
        printf("%d  \t", rs[i]);
    }

    return 0;
    
    int nums[11] = {1,1,1,2,3,4,4,5,6,6,7};
    int count = findThird(11, nums);
     printf("%d \n", count);
    return 0;
//   int min =  minCount(123);
//    printf("%d \n", min);
//    int nums[] = {2,3,4,5,7,1};
    
    insertSort(nums, 6);
    
    return 0;
    std::vector<int> vec{1,1,3,6,7};
    
    Solution2 s2;
    vector<vector<int>> res = s2.combinationSum(vec, 7);
    
    for(vector<int> path:res) {
        for(int i: path) {
            printf("%d-", i);
        }
        printf("\n");
    }
    
    return 0;
    
    int _nums[3] = {1,2,3};
    
    all(_nums, 0, 3);
    
    return 0;
//    Solution1 s;
//    std::string st = s.longestPalindrome("bababad");
//
//    printf("%s \n", st.c_str());
   
    
    return 0;
    
    Solution solution;
    ListNode *l1 = new ListNode(2);
    ListNode *l2 = new ListNode(4);
    ListNode *l3 = new ListNode(3);

    ListNode *r1 = new ListNode(5);
    ListNode *r2 = new ListNode(6);
    ListNode *r3 = new ListNode(4);
    
    l1->next = l2;
    l2->next = l3;
    l3->next = nullptr;
    r1->next = r2;
    r2->next = r3;
    r3->next = nullptr;
    
   ListNode *node =  solution.addTwoNumbers(l1, r1);

    while (node != nullptr) {
        printf(" --  %d \n", node->val);
        node = node->next;
    }
}

#endif /* rwlock_hpp */
