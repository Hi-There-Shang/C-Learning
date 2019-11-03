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
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include "Stock.hpp"

template <class N>
class StockFactory: boost::noncopyable {
public:
    typedef Stock<N> Stock;
    typedef std::lock_guard<std::mutex> lock_guard;
//    typedef boost::shared_ptr<Stock> stock_ptr;
    
    boost::shared_ptr<Stock> get(typename Stock::Name_Traits::value_type key);
private:
    std::mutex m_mutex;
    /// 注意 这里的share_ptr 会造成循环引用,stock不会被释放
    std::map<typename Stock::Name_Traits::value_type, boost::weak_ptr<Stock>> stock_map;
};

template <class N>
boost::shared_ptr<typename StockFactory<N>::Stock> StockFactory<N>::get(typename Stock::Name_Traits::value_type key) {
    
    boost::shared_ptr<Stock> pStock;
    
    lock_guard lock(m_mutex);

    boost::weak_ptr<Stock> &wStock = stock_map[key];
    pStock = wStock.lock();
//
    if (!pStock) {
        pStock.reset(new Stock(key));
        wStock = pStock;
        stock_map.insert({key, wStock});
    }
    return pStock;
}

template <class T>
StockFactory<T>* makeFactory(T name) {
    return new StockFactory<T>();
}

int main123434() {
//    auto factory = makeFactory<int>(1);
//    std::string key = "world";
//    auto ptr = factory->get(key);
    return 0;
}

#endif /* StockFactory_hpp */
