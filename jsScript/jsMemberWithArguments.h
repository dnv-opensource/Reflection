#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsMember.h"
#include "TypeUtilities/IntrusiveClass.h"
#include "TypeUtilities/IntrusivePointer.h"
#include "Reflection/TypeConversions/IConversionGraph.h"

class jsMemberWithArguments;
using jsMemberWithArgumentsPointer = DNVS::MoFa::TypeUtilities::IntrusivePointer<jsMemberWithArguments>;

class jsMemberWithArguments : public DNVS::MoFa::TypeUtilities::IntrusiveClass<jsMemberWithArguments>
{
public:
    jsMemberWithArguments(DNVS::MoFa::Reflection::TypeConversions::ConversionGraphPointer conversionGraph, jsMember* member, const jsValue::Params& arguments);
    ///Compares two members for the best conversion quality.
    int CompareConversionQuality(const jsMemberWithArgumentsPointer& other) const;
    ///Invokes the method with the arguments passed in the constructor.
    jsValue* Invoke();
    ///Checks if the member can be called with the input arguments.
    bool IsOk() const { return m_isValid && !m_isAmbiguous; }
    jsMemberWithArgumentsPointer SelectBestOverload(const jsMemberWithArgumentsPointer& other);
    static jsMemberWithArgumentsPointer SelectBestOverload(const jsMemberWithArgumentsPointer& lhs, const jsMemberWithArgumentsPointer& rhs);
    DNVS::MoFa::Reflection::TypeConversions::ConversionSequencePointer GetConversionSequence(jsValue* input, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& resultType);
    jsMember* GetMember() const;
private:
    bool HasCorrectNumberOfArguments() const;
    bool IsMemberFunction() const;
    bool IsConstructor() const;
    DNVS::MoFa::Reflection::TypeConversions::ConversionGraphPointer m_conversionGraph;
    jsMember* m_member;
    const jsValue::Params& m_arguments;
    std::vector<DNVS::MoFa::Reflection::TypeConversions::ConversionSequencePointer> m_conversionArguments;
    bool m_isValid;
    bool m_isAmbiguous;
    bool m_isConverted;
    static jsMemberWithArgumentsPointer ReturnFirstIfArgumentConversionSucceeds(const jsMemberWithArgumentsPointer& first, const jsMemberWithArgumentsPointer& second);

};
