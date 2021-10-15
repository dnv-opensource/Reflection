//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsScriptConversionGraph.h"
#include "Reflection/TypeConversions/IConversion.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/TypeConversions/ConversionSequence.h"
#include "Reflection/TypeConversions/ConversionType.h"

class jsValue;
using namespace DNVS::MoFa::Reflection;
class NullPtrConversion : public TypeConversions::IConversion
{
public:
    NullPtrConversion(const Types::DecoratedTypeInfo& type)
        : m_type(type) {}

    Variants::Variant Convert(const Variants::Variant& variable) override
    {
        return Variants::Variant(nullptr, m_type);
    }


    void IntrusiveConvert(Variants::Variant& variable) override
    {
        variable.SetDecoratedTypeInfo(m_type);
    }

private:
    Types::DecoratedTypeInfo m_type;
};

TypeConversions::ConversionSequencePointer jsScriptConversionGraph::GetConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const
{
    if (from.GetData() == nullptr &&
        from.GetDecoratedTypeInfo().GetTypeInfo() == typeid(jsValue) &&
        Types::IsPointer(from.GetDecoratedTypeInfo()) &&
        Types::IsPointer(to)
        )
    {
        auto conversion = std::make_shared<TypeConversions::ConversionSequence>(TypeConversions::ConversionType::TrivialConversion);
        conversion->Add(std::make_shared<NullPtrConversion>(to));
        return conversion;
    }
    return TypeConversions::ConversionGraphWithComplexDynamicConversion::GetConversionSequence(from, to);
}
