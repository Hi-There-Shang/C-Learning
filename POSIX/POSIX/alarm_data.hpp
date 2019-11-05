//
//  alarm_data.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef alarm_data_hpp
#define alarm_data_hpp

#include <stdio.h>
#include "error.hpp"

struct alarm_tag {
    /// 链表
    alarm_tag *link;
    /// 锁
    pthread_mutex_t mutex;
    /// 时间戳
    time_t time;
    /// 消息
    char message[128];
    /// 休眠时间
    int seconds;
};

#endif /* alarm_data_hpp */
