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
    
    stock_ptr get(const typename Stock::value_type &key);
private:
    std::mutex m_mutex;
    /// 注意 这里的share_ptr 会造成循环引用,stock不会被释放
    std::map<typename Stock::value_type, stock_ptr> stock_map;
};

template <class N, class P>
typename StockFactory<N, P>::stock_ptr StockFactory<N, P>::get(const typename Stock::value_type &key) {
    stock_ptr pStock;
    lock_guard lock(m_mutex);
    boost::weak_ptr<Stock> &wStock = stock_map[key];
    pStock = wStock->lock();
    if (!pStock) {
        pStock.reset(new Stock(key));
        wStock = pStock;
    }
    return pStock;
}

#endif /* StockFactory_hpp */
