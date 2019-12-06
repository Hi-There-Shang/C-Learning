//
//  signal.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/12/6.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef signal_hpp
#define signal_hpp

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static void sig_int(int sig) {
    printf("error ---sig == %d \n", sig);
}

int main1234566543() {
    char buffer[512];
    pid_t pid;
    int status;
    if (signal(SIGINT, sig_int) == SIG_ERR) {
        fprintf(stderr, "error \n");
    }
    
    printf("%%");
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';
        
        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error \n");
        } else if (pid == 0) {
            execlp(buffer, buffer, (char *)0);
            fprintf(stdout, "buffer = %s ,,--- %d \n", buffer, getpid());
            exit(127);
        }
        
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            fprintf(stderr, "error pid child \n");
        }
        printf("--- %d \n", getpid());
        printf("%%");
    }
    return 0;
}

#endif /* signal_hpp */
