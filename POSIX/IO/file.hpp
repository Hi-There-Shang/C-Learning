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
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

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
    int status = stat(fileName, &st);
    if (status == 0) {
        printf("%d \n", st.st_mode);
    }
    
    /// 刷新缓冲区
//    sync()
//    fsync(<#int#>)
    
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
}

int main() {
    testUnix();
    return 0;
}

#endif /* file_hpp */
