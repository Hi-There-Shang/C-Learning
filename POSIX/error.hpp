//
//  error.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef error_hpp
#define error_hpp

#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <dirent.h>
#include <memory>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#include <mutex>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_unique.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/try_lexical_convert.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <boost/assert.hpp>
#include <thread>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <time.h>


#include "Base.hpp"

#define ERRORNO errno

#define err_abort(code,text)  do {                                                   \
                                   fprintf(stderr, "%s at \"%s\": %d: %s \n",        \
                                   text, __FILE__,__LINE__,strerror(code));          \
                                 }while(0)

#define errno_abort(text) do {                                                        \
                                    fprintf(stderr, "%s at \"%s\": %d: %s \n",        \
                                    text, __FILE__,__LINE__,strerror(ERRORNO));         \
                                    abort(0);                                         \
}while(0)

#define STATIC_MALLOC(x, code)    if (x == 0) {\
                               err_abort(1, code);\
                            }

namespace _Log_ {
    template <class T>
    inline void Log(T &&value) {
        std::cout << value << std::endl;
    }
}

#endif /* error_hpp */
