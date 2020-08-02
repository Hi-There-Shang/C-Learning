//
//  Decl.cpp
//  POSIX
//
//  Created by Mr_Shang on 2020/7/29.
//  Copyright © 2020 S.S.S.S.S.S.S.S. All rights reserved.
//

#include "Decl.hpp"
#include <boost/type_traits.hpp>


template<typename T, bool Nullable, typename Offset>
class RelativeDirectPointerImpl {
private:
    
public:
    
    using ValueTy = T;
    using PointerTy = T*;
    
    PointerTy get() const & {
      return nullptr;
    }
};


template<typename T, bool Nullable = true, typename Offset = int32_t>
class RelativeDirectPointer :
  private RelativeDirectPointerImpl<T, Nullable, Offset>
{
  using super = RelativeDirectPointerImpl<T, Nullable, Offset>;
public:
  using super::get;
  using super::super;
};

class Value {};

template<typename T = Value, bool Nullable = true, typename Offset = int32_t>
using DirectPointer = RelativeDirectPointer<T>;

void testPointer() {
    DirectPointer<> pointer;
    
    pointer.get();
    
}

enum class MetadataSourceKind {
#define METADATA_SOURCE(Id, Parent) Id,
    METADATA_SOURCE(ClosureBinding, MetadataSource)
    METADATA_SOURCE(ReferenceCapture, MetadataSource)
    METADATA_SOURCE(MetadataCapture, MetadataSource)
    METADATA_SOURCE(GenericArgument, MetadataSource)
    METADATA_SOURCE(Self, MetadataSource)
    METADATA_SOURCE(SelfWitnessTable, MetadataSource)
#undef METADATA_SOURCE
};


class MetadataSource {
};

//class ClosureBindingMetadataSource: public MetadataSource {};
//class ReferenceCaptureMetadataSource: public MetadataSource {};
////class ClosureBindingMetadataSource: public MetadataSource {};
//class MetadataCaptureMetadataSource: public MetadataSource {};
//class GenericArgumentMetadataSource: public MetadataSource {};
//class SelfMetadataSource: public MetadataSource {};
//class SelfWitnessTableMetadataSource: public MetadataSource {};


void testDefine() {
    MetadataSourceKind kind = MetadataSourceKind::ClosureBinding;
    
//    strtol
}

class _Enum {
};

class NominalType {};

class _Protocol {};

class First_NominalTypeDecl {};
class Last_NominalTypeDecl {};

//typedef _Protocol*  Protocol;

enum class DeclKind : uint8_t {
#define DECL(Id, Parent) Id,
#define LAST_DECL(Id) Last_Decl = Id,
#define DECL_RANGE(Id, FirstId, LastId) \
  First_##Id##Decl = FirstId, Last_##Id##Decl = LastId,
#include "DeclNodes.def"
};


void test() {
    
    _Enum *Enum;
    _Protocol *Protocol;
//    DECL_RANGE(NominalType, Enum, Protocol)
}
