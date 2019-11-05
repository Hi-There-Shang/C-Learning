//
//  test_share_ptr.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/5.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef test_share_ptr_hpp
#define test_share_ptr_hpp

#include <stdio.h>
#include "error.hpp"
#include "user.hpp"

namespace __Print {
    template <class T>
    void print(T &&value) {
        std::cout << value << std::endl;
    }
}

using namespace __Print;
using namespace std;

typedef boost::shared_ptr<book> book_pointer;

/*
 传递值传递 weak_ptr
 引用计数  参数传递 也会加1
  ----- 不能传递引用????
 */
void test_weak_share(boost::weak_ptr<book> weakPtr) {
    sleep(3);
     printf("子线程 use_count = %ld \n", weakPtr.use_count());
}

void test_thread() {
    book_pointer book(new struct book(12, 100));
    std::thread t(test_weak_share, book);
    printf("主线程 use_count = %ld \n", book.use_count());
    book.reset();
    printf("主线程 use_count = %ld \n", book.use_count());
    
    /*
     join 之前 会释放
     join 之后  join会卡主线程 知道子线程执行完成
     */
    t.join();
    
    printf("主线程 use_count = %ld \n", book.use_count());
    book.reset();
    printf("主线程 use_count = %ld \n", book.use_count());
    print("done");
}


typedef void(*DeleteFunc)(void *);

void delete_func(void* ptr) {
    print("delete function");
    print(ptr);
    book *_book = static_cast<book *>(ptr);
    print(_book->m_page);
    print(_book->m_price);
    delete _book;
}

void test_share_ptr_delete() {
    /// 指定了 delete 函数
    /// 在函数内部 必须delete ptr 不然会内存泄露
    /// 此 delete_func 仅作用于 new book(1,2)
    boost::shared_ptr<book> delete_ptr(new book(1,2), delete_func);
    /// 重置 指向新的内存空间
    /// 内存管理还是由share_ptr负责
    print(delete_ptr);
    delete_ptr.reset(new book(100,200));
    print(delete_ptr);
    /// reset就清空了
    delete_ptr.reset();
    print(delete_ptr);
}

void test_container() {
    typedef boost::shared_ptr<__user> book_pointer;
    /// 容器会保持一份引用计数
    /// typedef std::vector<boost::share_ptr<__user>> Books;
    ///
    /// 容器不会增加引用计数
    typedef std::vector<boost::weak_ptr<__user>> Books;
    Books books;
    for (string str : {"1","2","3","4","5"}) {
        book_pointer _book = boost::make_shared<__user>(str);
        print(_book.use_count());
        /// 传值  引用计数会加1
        /// 容器会保持一份引用计数
        /// 针对于  std::vector<boost::share_ptr<__user>> Books;
        /// 对象的生命周期延长----> effective c++
        ///
        
        /// 针对于 weak_ptr 容器不会持有引用计数
        boost::weak_ptr<__user> weak_user = _book;
        books.push_back(weak_user);
        print("引用计数");
        print(_book.use_count());
    }
}

int main() {
//    test_thread();
//    test_share_ptr_delete();
    test_container();
    return 0;
}

#endif /* test_share_ptr_hpp */
