//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberWithArguments.h"
#include "MemberCompare.h"
#include "IMember.h"

#include "Reflection/Variants/ConversionError.h"
#include "Reflection/TypeConversions/ArgumentConversionList.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include <algorithm>
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {

    MemberWithArgumentsPointer MemberWithArguments::SelectBestOverload( MemberWithArgumentsPointer other )
    {
        if(!other) 
            return this;
        switch(CompareConversionQuality(other)) 
        {
        case -1:
            return ReturnFirstIfArgumentConversionSucceeds(this, other);
        case 1:
            return ReturnFirstIfArgumentConversionSucceeds(other, this);
        default:
            {
                if(!MemberCompare().HaveSameInputTypes(m_member, other->m_member))
                    m_isAmbiguous = true;
                return this;
            }
        }
    }

    Members::MemberWithArgumentsPointer MemberWithArguments::SelectBestOverload( MemberWithArgumentsPointer lhs, MemberWithArgumentsPointer rhs )
    {
        if(!lhs) 
            return rhs;
        else 
            return lhs->SelectBestOverload(rhs);
    }

    TypeConversions::ConversionSequencePointer MemberWithArguments::GetConversionSequence( Variants::Variant& input, const Types::DecoratedTypeInfo& resultType )
    {
        TypeConversions::ConversionSequencePointer conversionSequence = m_conversionGraph->GetConversionSequence(input, resultType);
        if(conversionSequence->IsValid()) 
            return conversionSequence;
        else 
        {
            input = m_conversionGraph->GetDynamicType(input);
            return m_conversionGraph->GetConversionSequence(input, resultType);
        }
    }

    void MemberWithArguments::ThrowIfConversionIsInvalid( const Variants::Variant& var, TypeConversions::ConversionSequencePointer conversion, const Types::DecoratedTypeInfo& toType ) const
    {
        if(conversion && !conversion->IsValid()) 
        {
            if(var.GetDecoratedTypeInfo().GetTypeInfoPointer() == 0)
                throw Variants::ConversionError(typeid(void), toType.GetTypeInfo());
            else
                throw Variants::ConversionError(var.GetDecoratedTypeInfo().GetTypeInfo(), toType.GetTypeInfo());
        }
    }

    bool IsNullPointerDereference(const Variants::Variant& variant, const Types::DecoratedTypeInfo& type)
    {
        return Types::IsPointer(variant.GetDecoratedTypeInfo()) && variant.GetData() == nullptr && !Types::IsPointer(type);
    }

    MemberWithArguments::MemberWithArguments( TypeConversions::ConversionGraphPointer conversionGraph, MemberPointer member, const std::vector<Variants::Variant>& arguments, MemberType type) :   m_member(member)
        ,   m_arguments(arguments)
        ,   m_conversionGraph(conversionGraph)
        ,   m_isValid(false)
        ,   m_isAmbiguous(false)
        ,   m_isConverted(false)
    {
        if(!m_conversionGraph) 
            return;
        if(!m_member) 
            return;
        if((m_member->GetMemberType()&type) == 0) 
            return;
        if(arguments.size() >= member->GetMinimumArgumentCount() && arguments.size() <= member->GetMaximumArgumentCount())
        {
            m_isValid = true;
            m_conversionArguments.resize(m_arguments.size());
            for(size_t i = 0; i<m_arguments.size(); ++i) 
            {
                m_conversionArguments[i] = GetConversionSequence(m_arguments[i], m_member->GetArgumentInfo(i)->GetTypeInfo());
                if(!m_conversionArguments[i]->IsValid()) 
                {
                    m_isValid = false;
                    break;
                }
                if (IsNullPointerDereference(m_arguments[i], m_member->GetArgumentInfo(i)->GetTypeInfo()))
                {
                    m_isValid = false;
                    break;
                }
            }
        }
    }

    int MemberWithArguments::CompareConversionQuality( MemberWithArgumentsPointer other ) const
    {
        if(!IsOk() && !other->IsOk()) 
            return 0;
        else if(!IsOk()) 
            return 1;
        else if(!other->IsOk()) 
            return -1;
        else 
        {
			///Vararg functions are always a worse choice.
            if(GetMember()->IsVarargMember() != other->GetMember()->IsVarargMember())
            {
                if(GetMember()->IsVarargMember()) 
                    return 1;
                else 
                    return -1;
            }
            return TypeConversions::ArgumentConversionList::Compare(m_conversionArguments, other->m_conversionArguments);
        }
        return 0;
    }

    void MemberWithArguments::PrepareArgumentsForInvoke()
    {
        if (m_isConverted)
            return;
        if(!m_isValid) 
        {
            for(size_t i = 0; i<m_arguments.size(); ++i) 
            {
                ThrowIfConversionIsInvalid(m_arguments[i], m_conversionArguments[i], m_member->GetArgumentInfo(i)->GetTypeInfo());
            }
            throw std::runtime_error("Invalid Overload");
        }
        else if(m_isAmbiguous) 
            throw std::runtime_error("Ambiguous call to function");
        else 
        {
            for(size_t i = 0; i<m_arguments.size(); ++i) 
            {
                m_conversionArguments[i]->IntrusiveConvert(m_arguments[i]);
                if(m_member->GetArgumentInfo(i)->GetTypeInfo().GetTypeInfo() != std::type_index(typeid(Variants::Variant)))
                    m_arguments[i].SetDecoratedTypeInfo(m_member->GetArgumentInfo(i)->GetTypeInfo());
            }
        }
        m_isConverted = true;
    }

    void MemberWithArguments::Validate()
    {
        PrepareArgumentsForInvoke();
        m_member->Validate(m_arguments);
    }
    
    Variants::Variant MemberWithArguments::Invoke()
    {
        PrepareArgumentsForInvoke();
        return m_member->Invoke(m_arguments);
    }

    Members::MemberPointer MemberWithArguments::GetMember() const
    {
        return m_member;
    }

    MemberWithArgumentsPointer MemberWithArguments::ReturnFirstIfArgumentConversionSucceeds(MemberWithArgumentsPointer first, MemberWithArgumentsPointer second)
    {
        try {
            first->PrepareArgumentsForInvoke();
            return first;
        }
        catch (std::exception)
        {
            return second;
        }
    }

}}}}