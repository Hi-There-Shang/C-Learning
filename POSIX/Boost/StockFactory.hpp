//
//  StockFactory.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/3.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef StockFactory_hpp
#define StockFactory_hpp

#include <stdio.h>
#include <mutex>
#include <boost/shared_ptr.hpp>
#include <map>
#include "Stock.hpp"

template <class N, class P>
class StockFactory {
public:
    typedef Stock<N, P> Stock;
    typedef std::lock_guard<std::mutex> lock_guard;
    typedef boost::shared_ptr<Stock> stock_ptr;
    
    boost::shared_ptr<Stock> get(typename Stock::Name_Traits::value_type key);
private:
    std::mutex m_mutex;
    /// 注意 这里的share_ptr 会造成循环引用,stock不会被释放
    std::map<typename Stock::Name_Traits::value_type, stock_ptr> stock_map;
};

template <class N, class P>
boost::shared_ptr<typename StockFactory<N, P>::Stock> StockFactory<N, P>::get(typename Stock::Name_Traits::value_type key) {
    boost::shared_ptr<Stock> pStock;
//    lock_guard lock(m_mutex);
//
//    stock_ptr &wStock = stock_map.at(key);
////    pStock = wStock->lock();
//
//    if (!pStock) {
//        pStock.reset(new Stock(key));
////        wStock = pStock;
//    }
    return pStock;
}

StockFactory<std::string, void>* makeFactory(const std::string &name) {
    return new StockFactory<std::string, void>();
}

//int main() {
////    auto factory = makeFactory("hello");
////    std::string key = "world";
////    factory->get(key);
//    return 0;
//}

#endif /* StockFactory_hpp */
