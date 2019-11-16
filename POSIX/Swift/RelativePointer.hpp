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

template <typename Offset>
void static_check(const Offset &offset) {
    static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "offset type should be signed integer");
}

template <typename ValueTy, bool Nullable = false, typename Offset = int32_t>
class RelativeIndirectPointer {
private:
    static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "offset type should be signed integer");
    
    Offset RelativeOffset;
    RelativeIndirectPointer() = delete;
    RelativeIndirectPointer(RelativeIndirectPointer &&) = delete;
    RelativeIndirectPointer(const RelativeIndirectPointer &) = delete;
    RelativeIndirectPointer &operator=(RelativeIndirectPointer &&)
    = delete;
    RelativeIndirectPointer &operator=(const RelativeIndirectPointer &)
    = delete;
    
public:
    const ValueTy* get() const {
        if (Nullable && RelativeOffset == 0) {
            return nullptr;
        }
        uintptr_t address = applyRelativeOffset(this, RelativeOffset);
        return *reinterpret_cast<const ValueTy * const *>(address);
    }
    
    bool isNull() const {
        return RelativeOffset == 0;
    }
    
    operator const ValueTy* () const& {
        return get();
    }
    
    const ValueTy *operator ->() const& {
        return get();
    }
};

template <typename ValueTy, bool Nullable = false, typename Offset = int32_t>
class RelativeIndirectablePointer {
private:
    static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "offset type should be signed integer");
    
    Offset RelativeOffsetPlusIndirect;
    RelativeIndirectablePointer() = delete;
    RelativeIndirectablePointer(RelativeIndirectablePointer &&) = delete;
    RelativeIndirectablePointer(const RelativeIndirectablePointer &) = delete;
    RelativeIndirectablePointer &operator=(RelativeIndirectablePointer &&)
    = delete;
    RelativeIndirectablePointer &operator=(const RelativeIndirectablePointer &)
    = delete;
public:
    RelativeIndirectablePointer(ValueTy *absolute)
     :RelativeOffsetPlusIndirect(
      (Nullable && absolute == nullptr) ? 0 : measureRelativeOffset<Offset>(absolute, this)
                                 ) {
         if (!Nullable) {
             assert(absolute != nullptr &&
                    "constructing non-nullable relative pointer from null");
         }
     }
    
    RelativeIndirectablePointer& operator =(ValueTy *absolute) {
        if (!Nullable) {
            assert(absolute != nullptr &&
                   "constructing non-nullable relative pointer from null");
        }
        RelativeOffsetPlusIndirect = Nullable && absolute == nullptr ? 0 : measureRelativeOffset<Offset>(absolute, this);
        return *this;
    }
    
    const ValueTy* get() const {
        static_assert(alignof(ValueTy) >=2 && alignof(Offset) >= 2, "alignment of value and offset must be at least 2 to make room for indirectable flag");
        if (Nullable && RelativeOffsetPlusIndirect == 0) {
            return nullptr;
        }
        
        Offset plusIndirectOffset = RelativeOffsetPlusIndirect;
        uintptr_t address = applyRelativeOffset(this, plusIndirectOffset & ~1);
        if (plusIndirectOffset & 1) {
            return *reinterpret_cast<const ValueTy * const *>(address);
        } else {
            return reinterpret_cast<const ValueTy *>(address);
        }
    }
    
    /// A zero relative offset encodes a null reference.
    bool isNull() const & {
        return RelativeOffsetPlusIndirect == 0;
    }
    
    operator const ValueTy* () const & {
        return get();
    }
    
    const ValueTy *operator->() const & {
        return get();
    }
};

template <typename ValueTy, typename IntTy, bool Nullable = false, typename Offset = int32_t>
class RelativeIndirectablePointerIntPair {
private:
    static_assert(boost::is_integral<Offset>::value && boost::is_signed<Offset>::value, "offset type should be signed integer");
    Offset RelativeOffsetPlusIndirectAndInt;
    
    RelativeIndirectablePointerIntPair() = delete;
    RelativeIndirectablePointerIntPair(
                                       RelativeIndirectablePointerIntPair &&) = delete;
    RelativeIndirectablePointerIntPair(
                                       const RelativeIndirectablePointerIntPair &) = delete;
    RelativeIndirectablePointerIntPair& operator=(
                                                  RelativeIndirectablePointerIntPair &&) = delete;
    RelativeIndirectablePointerIntPair &operator=(
                                                  const RelativeIndirectablePointerIntPair &) = delete;
    
    static Offset getIntMask() {
        return (alignof(Offset) - 1) & ~((Offset)0x01);
    }
public:
    
    const ValueTy* getPointer() const & {
        static_assert(alignof(ValueTy) >= 2 && alignof(Offset) >= 2,
                      "alignment of value and offset must be at least 2 to "
                      "make room for indirectable flag");
        Offset offset = RelativeOffsetPlusIndirectAndInt & ~getIntMask();
        if (Nullable && offset == 0) return nullptr;
        
        Offset plusIndirectAndIntOffset = offset;
        uintptr_t address = applyRelativeOffset(this, plusIndirectAndIntOffset & ~1);
        if (plusIndirectAndIntOffset & 1) {
            return *reinterpret_cast<const ValueTy * const *>(address);
        } else {
            return reinterpret_cast<const ValueTy *>(address);
        }
    }
    
    bool isNull() const & {
        Offset offset = (RelativeOffsetPlusIndirectAndInt & ~getIntMask());
        return offset == 0;
    }
    
    IntTy getInt() const & {
        return IntTy((RelativeOffsetPlusIndirectAndInt & getIntMask()) >> 1);
    }
    
};

#endif /* RelativePointer_hpp */
