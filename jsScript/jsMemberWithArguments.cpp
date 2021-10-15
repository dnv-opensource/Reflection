//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsMemberWithArguments.h"
#include "Reflection/TypeConversions/ArgumentConversionList.h"
#include "Reflection/Variants/VariantService.h"

using namespace DNVS::MoFa::Reflection;

jsMemberWithArguments::jsMemberWithArguments(TypeConversions::ConversionGraphPointer conversionGraph, jsMember* member, const jsValue::Params& arguments)
    : m_conversionGraph(conversionGraph)
    , m_member(member)
    , m_arguments(arguments)
    , m_isValid(false)
    , m_isConverted(false)
    , m_isAmbiguous(false)
{
    if (!m_conversionGraph)
        return;
    if (!m_member)
        return;
    if (!HasCorrectNumberOfArguments())
        return;
    m_isValid = true;
    size_t argumentCount = (size_t)m_member->param_size();
    if (m_member->IsVariadic())
        --argumentCount;
    m_conversionArguments.resize(argumentCount);
    for (size_t i = 0; i < argumentCount; ++i)
    {
        m_conversionArguments[i] = GetConversionSequence(m_arguments[i+1], m_member->GetParamType((int)i));
        if (!m_conversionArguments[i]->IsValid())
        {
            m_isValid = false;
            break;
        }
    }
}

int jsMemberWithArguments::CompareConversionQuality(const jsMemberWithArgumentsPointer& other) const
{
    if (!IsOk() && !other->IsOk())
        return 0;
    else if (!IsOk())
        return 1;
    else if (!other->IsOk())
        return -1;
    else
    {
        ///Vararg functions are always a worse choice.
        if (GetMember()->IsVariadic() != other->GetMember()->IsVariadic())
        {
            if (GetMember()->IsVariadic())
                return 1;
            else
                return -1;
        }
        return TypeConversions::ArgumentConversionList::Compare(m_conversionArguments, other->m_conversionArguments);
    }
    return 0;
}

jsValue* jsMemberWithArguments::Invoke()
{
    return m_member->call(m_arguments);
}

jsMemberWithArgumentsPointer jsMemberWithArguments::SelectBestOverload(const jsMemberWithArgumentsPointer& other)
{
    if (!other)
        return this;
    switch (CompareConversionQuality(other))
    {
        case -1:
            return ReturnFirstIfArgumentConversionSucceeds(this, other);
        case 1:
            return ReturnFirstIfArgumentConversionSucceeds(other, this);
        default:
        {
            m_isAmbiguous = true;
            return this;
        }
    }
}

jsMemberWithArgumentsPointer jsMemberWithArguments::SelectBestOverload(const jsMemberWithArgumentsPointer& lhs, const jsMemberWithArgumentsPointer& rhs)
{
    if (!lhs)
        return rhs;
    else
        return lhs->SelectBestOverload(rhs);
}

TypeConversions::ConversionSequencePointer jsMemberWithArguments::GetConversionSequence(jsValue* input, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& resultType)
{
    Variants::Variant v = Variants::VariantService::ReflectDynamic(input);
    TypeConversions::ConversionSequencePointer conversionSequence = m_conversionGraph->GetConversionSequence(v, resultType);
    if (conversionSequence->IsValid())
        return conversionSequence;
    else
    {
        v = m_conversionGraph->GetDynamicType(v);
        return m_conversionGraph->GetConversionSequence(v, resultType);
    }
}

jsMember* jsMemberWithArguments::GetMember() const
{
    return m_member;
}

bool jsMemberWithArguments::HasCorrectNumberOfArguments() const
{
    size_t argumentCount = m_arguments.size();
    if (!IsMemberFunction())
        --argumentCount;
    if (m_member->IsVariadic())
        return argumentCount >= m_member->GetArity() - 1;
    else
        return argumentCount == m_member->GetArity();
}

bool jsMemberWithArguments::IsMemberFunction() const
{
    return m_member->param_size() < m_member->GetArity();
}

bool jsMemberWithArguments::IsConstructor() const
{
    return m_member->typeInfo()->type() == jsTypeInfo::jsConstructor;
}

jsMemberWithArgumentsPointer jsMemberWithArguments::ReturnFirstIfArgumentConversionSucceeds(const jsMemberWithArgumentsPointer& first, const jsMemberWithArgumentsPointer& second)
{
    return first;
}