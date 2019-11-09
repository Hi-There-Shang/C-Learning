#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "person.c"

int main() {
//    pipe(<#int *#>)
//    mmap(NULL, <#size_t#>, <#int#>, <#int#>, <#int#>, <#off_t#>)
//    creat("<#const char *#>", O_RDWR);
    int ret = system("ls -l");
    printf("--- %d \n", ret);
//   int fd = open("", O_RDONLY|O_WRONLY|O_RDWR);
//    char buffer[128];
//    read(fd, buffer, sizeof(buffer));

//    openat(<#int#>, <#const char *#>, <#int, ...#>)
//    fdopen(<#int#>, <#const char *#>)
//    fopen(<#const char *restrict __filename#>, <#const char *restrict __mode#>)
//    funopen(<#const void *#>, <#int (* _Nullable)(void *, char *, int)#>, <#int (* _Nullable)(void *, const char *, int)#>, <#fpos_t (* _Nullable)(void *, fpos_t, int)#>, <#int (* _Nullable)(void *)#>)
    printf("hello world \n");
    struct person p = {1, "123"};
    printf("%d---%s", p.age, p.name);
    return 0;
}
