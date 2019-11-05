//
//  scope_ptr.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef scope_ptr_hpp
#define scope_ptr_hpp

#include <stdio.h>
#include "user.hpp"
#include "error.hpp"

namespace __Print {
    template <class T>
    void print(T &&value) {
        std::cout << value << std::endl;
    }
}

using namespace boost;
using namespace __Print;

void test_scope_ptr() {
    scoped_ptr<int> int_ptr1(new int(8));
    scoped_ptr<int> int_ptr2(new int(9));
    print(*int_ptr1.get());
    print(*int_ptr1.get());
    print(*int_ptr2.get());
    
    /// 不能使用赋值构造函数 不是使用拷贝构造函数 不能隐式转换成bool
//    auto v = int_ptr;
//    scoped_ptr<int> p(int_ptr);
//    int_ptr1 == int_ptr2;
}

void test_uniqe_ptr() {
    auto ptr1 = make_unique<int>(19);
    auto ptr2 = make_unique<int>(18);
    /// 只支持隐式转换成 bool
    assert(ptr1 != ptr2);
//    ptr1 = ptr2;
    
    print(ptr1.get());
    print(ptr2.get());
    print(*ptr1.get());
    print(*ptr2.get());
    
}

/*
 引用传递 不会增加引用计数
 */
//void couterIncrement(const boost::shared_ptr<__user> &ptr) {
//    printf("函数调用 counter = %ld \n", ptr.use_count());
//}

/*
 * 函数传值 会引用计数加1 .. 局部变量 只是在函数内部使用... 函数弹栈--->引用计数减1
 * 多线程下---> 子线程保留引用计数--->万一主线程释放.对象不会被释放???
 */
void couterIncrement(boost::shared_ptr<__user> ptr) {
    printf("函数调用 counter = %ld \n", ptr.use_count());
}

void test_share_ptr() {
    typedef boost::shared_ptr<__user> user_pointer;
    user_pointer _user = boost::make_shared<__user>("123");
    
    /// 指向同一块内存空间--->c++内存模型---> 首地址相同---> 地址偏移
    /// 栈区的share_ptr 是值类型  引用计数是1
//    std::cout << (_user) << std::endl;
//    print(_user.get());
    print(_user.use_count());
    
    /// 引用计数加一
    user_pointer ptr = _user;
    print(_user.use_count());
    
    /// 引用计数不变
    boost::weak_ptr<__user> weak_p = ptr;
    print(_user.use_count());
    
       /// weak_ptr 提升为share_ptr 引用计数加一
    auto ptr1 = weak_p.lock();
    print(_user.use_count());
       /// ptr 清空
    ptr.reset();
    print(_user.use_count());
    ptr1.reset();
      /// ptr1 清空
    print(_user.use_count());
    std::cout << _user.get()->m_name << std::endl;
      /// 引用计数为0  调用析构函数
//    _user.reset();
      couterIncrement(_user);
      print(_user.use_count());
    printf("------ \n");
}

/*
 参数值传递. 引用计数加1
 */
//void test_thread(boost::shared_ptr<__user> user) {
//    sleep(10);
//    print("子线程开始");
//    print(user.use_count());
//    print(user.get()->m_name);
//}


/*
 如果传递的是引用 要判断引用是否被释放---->
 */
void test_thread(const boost::shared_ptr<__user> &user) {
    sleep(4);
    /// 这里需要判断对象是否已经被释放了-----> 主线程可能已经把对象析构了
    if (user == nullptr) {
        print("对象已经被释放");
        return;
    }
    print("子线程开始");
    print(user.use_count());
    print(user.get()->m_name);
}

void test_thread_() {
    
}

void thread___() {
    typedef boost::shared_ptr<__user> user_pointer;
    user_pointer _user = boost::make_shared<__user>("123");
    /// 这里引用计数已经加一了---->子线程保留一份引用计数 保证对象不会被释放
    std::thread thr(test_thread, std::ref(_user));
    print(_user.use_count());
    _user.reset();
    print(_user.use_count());
    thr.join();
    print("线程执行完毕");
}

int main11111111() {
    thread___();
//    test_share_ptr();
//    test_uniqe_ptr();
//    test_scope_ptr();
    return 0;
}

#endif /* scope_ptr_hpp */
