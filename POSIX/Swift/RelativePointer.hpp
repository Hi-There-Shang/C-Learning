//
//  RelativePointer.hpp
//  POSIX
//
//  Created by Mr_Shang on 2019/11/16.
//  Copyright © 2019 S.S.S.S.S.S.S.S. All rights reserved.
//

#ifndef RelativePointer_hpp
#define RelativePointer_hpp

#include <stdio.h>
#include <cstdint>
#include <assert.h>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_signed.hpp>

#define CHECK_OFFSET(Offset) static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "must be signed offset")

template <typename BaseTy, typename Offset>
static inline uintptr_t applyRelativeOffset(BaseTy *basePtr, Offset offset) {
    CHECK_OFFSET(Offset);
//    static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "must be signed offset");
    auto base = reinterpret_cast<uintptr_t>(basePtr);
    return base + offset;
}

template <typename Offset, typename A, typename B>
static inline Offset measureRelativeOffset(A *referent, B *base) {
//    static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "must be signed offset");
    CHECK_OFFSET(Offset);
    
    auto distance = (uintptr_t)referent - (uintptr_t)base;
    typedef typename boost::make_signed<Offset>::type Offset_Type;
    auto truncatedDistance = (Offset)(Offset_Type)distance;
    assert((uintptr_t)truncatedDistance == (uintptr_t)distance && "pointers are too far apart to fit in offset type");
    return truncatedDistance;
}

#endif /* RelativePointer_hpp */
