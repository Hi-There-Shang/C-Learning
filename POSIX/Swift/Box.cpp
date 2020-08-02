//
//  Box.cpp
//  POSIX
//
//  Created by Mr_Shang on 2020/7/27.
//  Copyright © 2020 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "Box.hpp"
#include <boost/type_traits.hpp>


constexpr size_t roundUpToAlignment(size_t offset, size_t alignment) {
  return ((offset + alignment - 1) & ~(alignment - 1));
}

template <class T,
          size_t Alignment = alignof(T),
          size_t Size = sizeof(T),
          size_t Stride = sizeof(T)>
struct NativeBox {
  using type = T;

  static constexpr size_t size = Size;
  static constexpr size_t alignment = Alignment;
  static constexpr size_t stride = Stride;
  static constexpr size_t isPOD = std::is_pod<T>::value;
  static constexpr bool isBitwiseTakable = isPOD;
  static constexpr unsigned numExtraInhabitants = 0;

  static void destroy(T *value) {
    value->T::~T();
  }
  
  static T *initializeWithCopy(T *dest, T *src) {
    return new (dest) T(*src);
  }

  static T *initializeWithTake(T *dest, T *src) {
    T *result = new (dest) T(std::move(*src));
    src->T::~T();
    return result;
  }
  
  static T *assignWithCopy(T *dest, T *src) {
    *dest = *src;
    return dest;
  }

  static T *assignWithTake(T *dest, T *src) {
    *dest = std::move(*src);
    src->T::~T();
    return dest;
  }

private:
  static T *next(T *ptr, size_t n = 1) {
    return (T*)((char*)ptr + stride * n);
  }
  static T *prev(T *ptr, size_t n = 1) {
    return (T*)((char*)ptr - stride * n);
  }
};


struct FunctionPointerBox : NativeBox<void*> {
    static constexpr unsigned numExtraInhabitants = 2;
    //swift_getFunctionPointerExtraInhabitantCount();

  static void storeExtraInhabitantTag(void **dest, unsigned tag) {
    //swift_storeFunctionPointerExtraInhabitant(dest, tag - 1);
      return;
  }

  static unsigned getExtraInhabitantTag(void * const *src) {
      return 1;
    //return swift_getFunctionPointerExtraInhabitantIndex(src) + 1;
  }
};


// A CRTP base class for defining boxes of retainable pointers.
template <class Impl, class T> struct RetainableBoxBase {
  using type = T;
  static constexpr size_t size = sizeof(T);
  static constexpr size_t alignment = alignof(T);
  static constexpr size_t stride = sizeof(T);
  static constexpr bool isPOD = false;
  static constexpr bool isBitwiseTakable = true;
#ifdef SWIFT_STDLIB_USE_NONATOMIC_RC
  static constexpr bool isAtomic = false;
#else
  static constexpr bool isAtomic = true;
#endif

  static void destroy(T *addr) {
    Impl::release(*addr);
  }

  static T *initializeWithCopy(T *dest, T *src) {
    *dest = Impl::retain(*src);
    return dest;
  }

  static T *initializeWithTake(T *dest, T *src) {
    *dest = *src;
    return dest;
  }
  
  static T *assignWithCopy(T *dest, T *src) {
    T oldValue = *dest;
    *dest = Impl::retain(*src);
    Impl::release(oldValue);
    return dest;
  }

  static T *assignWithTake(T *dest, T *src) {
    T oldValue = *dest;
    *dest = *src;
    Impl::release(oldValue);
    return dest;
  }

  // Right now, all object pointers are brought down to the least
  // common denominator for extra inhabitants, so that we don't have
  // to worry about e.g. type substitution on an enum type
  // fundamentally changing the layout.
    static constexpr unsigned numExtraInhabitants = 2;
    //swift_getHeapObjectExtraInhabitantCount();

  static void storeExtraInhabitantTag(T *dest, unsigned tag) {
    //swift_storeHeapObjectExtraInhabitant((HeapObject**) dest, tag - 1);
  }

  static unsigned getExtraInhabitantTag(const T *src) {
   // return swift_getHeapObjectExtraInhabitantIndex((HeapObject* const *) src) +1;
      return 1;
  }
};

struct HeapObject {
};

/// A box implementation class for Swift object pointers.
struct SwiftRetainableBox : RetainableBoxBase<SwiftRetainableBox, HeapObject*> {
  static HeapObject *retain(HeapObject *obj) {
    if (isAtomic) {
//      swift_retain(obj);
    } else {
//      swift_nonatomic_retain(obj);
    }
    return obj;
  }

  static void release(HeapObject *obj) {
    if (isAtomic) {
//      swift_release(obj);
    } else {
//      swift_nonatomic_release(obj);
    }
  }
};


struct RawPointerBox : NativeBox<void*> {
  static constexpr unsigned numExtraInhabitants = 1;

  static void storeExtraInhabitantTag(void **dest, unsigned tag) {
    assert(tag == 1);
    *dest = nullptr;
  }

  static unsigned getExtraInhabitantTag(void* const *src) {
    return *src == nullptr ? 1 : 0;
  }
};


template <size_t StartOffset, class... EltBoxes>
struct AggregateBoxHelper;

// Base case: empty list.
template <size_t StartOffset>
struct AggregateBoxHelper<StartOffset> {
public:
  static constexpr size_t endOffset = StartOffset;
  static constexpr size_t alignment = 1;
  static constexpr bool isPOD = true;
  static constexpr bool isBitwiseTakable = true;

public:
#define COPY_OP(OP)                        \
  static char *OP(char *dest, char *src) { \
    return dest;                           \
  }
  COPY_OP(initializeWithCopy)
  COPY_OP(initializeWithTake)
  COPY_OP(assignWithCopy)
  COPY_OP(assignWithTake)
#undef COPY_OP

  static void destroy(char *addr) {}
};

// Recursive case: add an element to the start.
template <size_t StartOffset, class EltBox, class... NextBoxes>
struct AggregateBoxHelper<StartOffset, EltBox, NextBoxes...> {
private:
  static constexpr size_t eltOffset =
    roundUpToAlignment(StartOffset, EltBox::alignment);
  static constexpr size_t startToEltOffset = (eltOffset - StartOffset);
  static constexpr size_t nextOffset = eltOffset + EltBox::size;
  using NextHelper = AggregateBoxHelper<nextOffset, NextBoxes...>;

public:
  static constexpr size_t endOffset = NextHelper::endOffset;
  static constexpr size_t alignment =
    (NextHelper::alignment > EltBox::alignment
   ? NextHelper::alignment : EltBox::alignment);
  static constexpr bool isPOD = EltBox::isPOD && NextHelper::isPOD;
  static constexpr bool isBitwiseTakable =
    EltBox::isBitwiseTakable && NextHelper::isBitwiseTakable;

private:
  static constexpr size_t eltToNextOffset = (nextOffset - eltOffset);
  static constexpr size_t startToNextOffset = (nextOffset - StartOffset);

public:
#define COPY_OP(OP)                                                       \
  static char *OP(char *dest, char *src) {                                \
    dest += startToEltOffset;                                             \
    src += startToEltOffset;                                              \
    dest = (char*) EltBox::OP((typename EltBox::type*) dest,              \
                              (typename EltBox::type*) src);              \
    dest = NextHelper::OP(dest + eltToNextOffset, src + eltToNextOffset); \
    return dest - startToNextOffset;                                      \
  }
  COPY_OP(initializeWithCopy)
  COPY_OP(initializeWithTake)
  COPY_OP(assignWithCopy)
  COPY_OP(assignWithTake)
#undef COPY_OP

  static void destroy(char *addr) {
    // We have no particular reason to destroy in either order.
    addr += startToEltOffset;
    EltBox::destroy((typename EltBox::type*) addr);
    NextHelper::destroy(addr + eltToNextOffset);
  }
};

template <class... EltBoxes>
struct AggregateBox {
  using type = char;

  using Helper = AggregateBoxHelper<0, EltBoxes...>;
  static constexpr size_t size = Helper::endOffset;
  static constexpr size_t alignment = Helper::alignment;
  static constexpr size_t rawStride = roundUpToAlignment(size, alignment);
  static constexpr size_t stride = rawStride == 0 ? 1 : rawStride;

  static constexpr bool isPOD = Helper::isPOD;
  static constexpr bool isBitwiseTakable = Helper::isBitwiseTakable;

  /// Don't collect extra inhabitants from the members by default.
  static constexpr unsigned numExtraInhabitants = 0;

  static void destroy(char *value) {
    Helper::destroy(value);
  }

  static char *initializeWithCopy(char *dest, char *src) {
    return Helper::initializeWithCopy(dest, src);
  }

  static char *initializeWithTake(char *dest, char *src) {
    return Helper::initializeWithTake(dest, src);
  }
    
  static char *assignWithCopy(char *dest, char *src) {
    return Helper::assignWithCopy(dest, src);
  }

  static char *assignWithTake(char *dest, char *src) {
    return Helper::assignWithTake(dest, src);
  }
};



namespace {
  // @escaping function types.
  struct ThickFunctionBox
    : AggregateBox<FunctionPointerBox, SwiftRetainableBox> {

    static constexpr unsigned numExtraInhabitants =
      FunctionPointerBox::numExtraInhabitants;

    static void storeExtraInhabitantTag(char *dest, unsigned tag) {
      FunctionPointerBox::storeExtraInhabitantTag((void**) dest, tag);
    }

    static unsigned getExtraInhabitantTag(const char *src) {
      return FunctionPointerBox::getExtraInhabitantTag((void * const *) src);
    }
  };
  /// @noescape function types.
  struct TrivialThickFunctionBox
      : AggregateBox<FunctionPointerBox, RawPointerBox> {

    static constexpr unsigned numExtraInhabitants =
        FunctionPointerBox::numExtraInhabitants;

    static void storeExtraInhabitantTag(char *dest, unsigned tag) {
      FunctionPointerBox::storeExtraInhabitantTag((void **)dest, tag);
    }

    static unsigned getExtraInhabitantTag(const char *src) {
      return FunctionPointerBox::getExtraInhabitantTag((void *const *)src);
    }
  };
} // end anonymous namespace
