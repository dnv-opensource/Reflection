#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IMember.h"
#include "MemberWithArgumentsFwd.h"
#include "MemberType.h"

#include "Reflection/TypeConversions/IConversionSequence.h"
#include "Reflection/TypeConversions/IConversionGraph.h"

#include "TypeUtilities/IntrusiveClass.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    ///MemberWithArguments tries to convert all the arguments to the correct type, and stores the conversion qualities.
    class REFLECTION_IMPORT_EXPORT MemberWithArguments : public TypeUtilities::IntrusiveClass<MemberWithArguments>
    {
    public:
        MemberWithArguments(TypeConversions::ConversionGraphPointer conversionGraph, MemberPointer member, const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll);
        ///Compares two members for the best conversion quality.
        int CompareConversionQuality(MemberWithArgumentsPointer other) const;
        ///Invokes the method with the arguments passed in the constructor.
        Variants::Variant Invoke();
        void Validate();
        ///Checks if the member can be called with the input arguments.
        bool IsOk() const {return m_isValid && !m_isAmbiguous; }
        MemberWithArgumentsPointer SelectBestOverload(MemberWithArgumentsPointer other);
        static MemberWithArgumentsPointer SelectBestOverload(MemberWithArgumentsPointer lhs, MemberWithArgumentsPointer rhs);
        TypeConversions::ConversionSequencePointer GetConversionSequence(Variants::Variant& input, const Types::DecoratedTypeInfo& resultType);
        void ThrowIfConversionIsInvalid( const Variants::Variant& var, TypeConversions::ConversionSequencePointer conversion, const Types::DecoratedTypeInfo& toType) const;
        MemberPointer GetMember() const;
    private:
        void PrepareArgumentsForInvoke();
        MemberType m_memberType;
        TypeConversions::ConversionGraphPointer m_conversionGraph;
        MemberPointer m_member;
        std::vector<Variants::Variant> m_arguments;
        std::vector<TypeConversions::ConversionSequencePointer> m_conversionArguments;
        bool m_isValid;
        bool m_isAmbiguous;
        bool m_isConverted;
        static MemberWithArgumentsPointer ReturnFirstIfArgumentConversionSucceeds(MemberWithArgumentsPointer first, MemberWithArgumentsPointer second);
    };

}}}}