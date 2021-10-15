//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsArray.Reflection.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Objects\Delegate.h"
#include "Reflection\Attributes\ContainerAttribute.h"
#include "jsValue.Reflection.h"
#include "jsScript\jsReference.h"

using namespace DNVS::MoFa::Reflection;

class ConvertjsReferenceTojsValue : public TypeConversions::IConversion
{
public:
    virtual Variants::Variant Convert(const Variants::Variant& other)
    {
        jsReference* ref = static_cast<jsReference*>(other.Data());
        return Variants::VariantService::Reflect(mofa::ref<jsValue>(ref->value()));
    }
    virtual void IntrusiveConvert(Variants::Variant& variable)
    {
        variable = Convert(variable);
    }
};

void DoReflect(const TypeLibraries::TypeLibraryPointer& typeLibrary, jsReference**)
{
    using namespace Classes;
    Class<jsReference, Public<jsValue>> cls(typeLibrary, "jsReference");
    typeLibrary->GetConversionGraph()->AddConversion(
        TypeId<jsReference>(), 
        TypeId<void>(), 
        TypeConversions::ConversionType::DynamicTypeConversion,
        std::make_shared<ConvertjsReferenceTojsValue>());
}
