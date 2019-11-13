//
//  file.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/12.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef file_hpp
#define file_hpp

#include <stdio.h>
#include "error.hpp"

void testUnix() {
    const char *fileName = "/Users/shangchengcheng/Desktop/123.txt";
    int fileFd = open(fileName, O_RDONLY|O_CREAT|O_WRONLY);
    
    char message[256] = "1234567";
    if (fileFd == -1) {
        printf("创建失败 \n");
    } else {
        ssize_t size = write(fileFd, message, sizeof(message));
        printf("%zd \n",size);
        printf("创建成功 \n");
    }
    char buffer[256];
    ssize_t size = read(fileFd, buffer, sizeof(buffer));
    printf("%zd \n", size);
    printf("---%s \n", buffer);
//    openat(<#int#>, <#const char *#>, <#int, ...#>)
//    int creatFd = creat(fileName, 0x0000|0x0001|0x0002);
    
    struct stat st;
    /// 获取文件属性
    int status = stat(fileName, &st);
    if (status == 0) {
        printf("%d \n", st.st_mode);
    }
    
    /// 时间
    ///
    /*
     __darwin_time_t tv_sec;   秒
     long            tv_nsec;  纳秒
    */
//    struct timespec time;
    /// 修改文件的修改时间
//    futimens(<#int __fd#>, <#const struct timespec *__times#>)
    
    printf("%ld -- %ld \n", st.st_ctimespec.tv_nsec, st.st_ctimespec.tv_sec);
    /// 刷新缓冲区
//    sync()
//    fsync(<#int#>)
    
    /// 改变文件属性
//    fcntl(<#int#>, <#int, ...#>)
    
    /// 访问权限
//    access(<#const char *#>, <#int#>)
    /// 文件截断
//    truncate(<#const char *#>, <#off_t#>)
    /// file 关联
//    link(<#const char *#>, <#const char *#>)
//    linkat(<#int#>, <#const char *#>, <#int#>, <#const char *#>, <#int#>)
    /// 重命名
//    rename(<#const char *__old#>, <#const char *__new#>)
    
    /// 创建目录
//    mkdir(<#const char *#>, <#mode_t#>)
    
    /// 打开文件
    DIR *dir = opendir(fileName);
    
//    fdopendir(<#int#>)
    struct dirent *ent = readdir(dir);
    
}

#define FTW_F   1
#define FTW_D   2
#define FTW_DNR 3
#define FTW_NS  4

typedef int (*MyFunc)(const char *, const struct stat *, int);
static MyFunc myFunc;

static long nreg;
static long ndir;
static long nblk;
static long nchr;
static long nfifo;
static long nslink;
static long nsock;
static long ntot;


static char *fullpath;
static size_t pathLen = 10;

static int doPath(MyFunc func) {
    struct stat statbuffer;
    struct dirent *dirp;
    DIR           *dir;
    int status;
    size_t n;
    
    status = lstat(fullpath, &statbuffer);
    if (status < 0) {
        return func(fullpath, &statbuffer, FTW_NS);
    }
    
    if (S_ISDIR(statbuffer.st_mode) == 0) {
        return func(fullpath, &statbuffer, FTW_F);
    }
    
    if ((status = func(fullpath, &statbuffer, FTW_D) != 0)) {
        return status;
    }
    
    n = strlen(fullpath);
    
    
    return 0;
};

static int myftw(char *filePath, MyFunc func) {
    fullpath = (char *)malloc(pathLen);
    STATIC_MALLOC(fullpath == NULL,"malloc error");
    if (pathLen <= strlen(filePath)) {
        pathLen = strlen(filePath) * 2;
        fullpath = (char *)realloc(fullpath, pathLen);
        STATIC_MALLOC(fullpath == NULL,"realloc error");
        
    }
    strcpy(fullpath, filePath);
    return (doPath(func));
};

static int func(const char *filePath, const struct stat *statBuffer, int mode) {
    
    return 0;
}

void read_file() {
    char *path = "";
    int status = myftw(path, (MyFunc)func);
    if (status < 0) {
        
    }
}

int main12323213123123123131() {
    testUnix();
    return 0;
}

#endif /* file_hpp */
