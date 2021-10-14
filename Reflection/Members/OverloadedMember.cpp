//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "OverloadedMember.h"
#include "MemberCompare.h"
#include "MemberWithArguments.h"
#include <algorithm>
#include "Reflection/Attributes/AttributeCollection.h"
#include "Formatting/IdentityFormatter.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {

    Variants::Variant OverloadedMember::Invoke( const std::vector<Variants::Variant>& arguments, MemberType type)
    {
        MemberWithArgumentsPointer bestOverload = PrepareInvoke(arguments, type);
        if(bestOverload) 
            return bestOverload->Invoke();
        else 
            throw std::runtime_error("No overloads found");
    }

    Types::DecoratedTypeInfo OverloadedMember::GetReturnType() const
    {
        Types::DecoratedTypeInfo commonReturnType;
        for (size_t i = 0; i < m_overloads.size(); ++i)
        {
            if (m_overloads[i]->GetMemberType() == MemberType::TypePropertyGet)
            {
                return m_overloads[i]->GetReturnType();
            }
            else if(commonReturnType == Types::DecoratedTypeInfo())
                commonReturnType = m_overloads[i]->GetReturnType();
            else if(commonReturnType != m_overloads[i]->GetReturnType())
                throw std::runtime_error(__FUNCTION__ " is not supported for the given member type");
        }
        return commonReturnType;        
    }

    bool OverloadedMember::IsConst() const
    {
        for (size_t i = 0; i < m_overloads.size(); ++i)
        {
            if (m_overloads[i]->GetMemberType() == MemberType::TypePropertyGet)
            {
                return m_overloads[i]->IsConst();
            }
        }
        throw std::runtime_error(__FUNCTION__ " is not supported for the given member type");
    }

    ArgumentInfoPointer OverloadedMember::GetArgumentInfo(size_t iarg) const
    {
        if (iarg == 0)
        {
            for (size_t i = 0; i < m_overloads.size(); ++i)
            {
                return m_overloads[i]->GetArgumentInfo(0);
            }
        }
        throw std::runtime_error(__FUNCTION__ " does not have any overloads");
    }

    size_t OverloadedMember::GetMinimumArgumentCount() const
    {
        size_t min = std::numeric_limits<size_t>::max();
        for(size_t i = 0; i<m_overloads.size(); ++i) 
        {
            min = std::min(min, m_overloads[i]->GetMinimumArgumentCount());
        }
        return min;
    }
    size_t OverloadedMember::GetMaximumArgumentCount() const
    {
        size_t max = 0;
        for(size_t i = 0; i<m_overloads.size(); ++i) 
        {
            max = std::max(max, m_overloads[i]->GetMaximumArgumentCount());
        }
        return max;
    }

    size_t OverloadedMember::GetArity() const
    {
        size_t max = 0;
        for(size_t i = 0; i<m_overloads.size(); ++i) 
        {
            max = std::max(max, m_overloads[i]->GetArity());
        }
        return max;
    }
    MemberType OverloadedMember::GetMemberType() const
    {
        MemberType memberType;
        for(size_t i = 0; i<m_overloads.size(); ++i) 
        {
            memberType |= m_overloads[i]->GetMemberType();
        }
        return memberType;
    }

    Members::MemberWithArgumentsPointer OverloadedMember::PrepareInvoke( const std::vector<Variants::Variant>& arguments, MemberType type)
    {
        MemberWithArgumentsPointer bestOverload;
        for(size_t i = 0; i<m_overloads.size(); ++i) 
        {
            MemberWithArgumentsPointer current = m_overloads[i]->PrepareInvoke(arguments, type);
            bestOverload = current->SelectBestOverload(bestOverload);
        }
        return bestOverload;
    }
    
    void OverloadedMember::Validate(const std::vector<Variants::Variant>& arguments, MemberType type)
    {    
        MemberWithArgumentsPointer bestOverload = PrepareInvoke(arguments, type);
        if(bestOverload) 
            bestOverload->Validate();
    }

    bool OverloadedMember::HasValidation() const
    {
        for (const MemberPointer& member : m_overloads)
            if (member->HasValidation())
                return true;
        return false;
    }

    Members::MemberPointer OverloadedMember::AddOverload(MemberPointer method)
    {
        if(IsOverloadRegistered(method)) 
            return this;
        if(!AreCompatibleTypes(GetMemberType(), method->GetMemberType())) 
        {
            throw std::runtime_error("Method already registered as " + GetMemberType().ToString() + " unable to add overload of type " + method->GetMemberType().ToString());
        }
        m_overloads.push_back(method);
        return this;
    }

    bool OverloadedMember::IsOverloadRegistered( MemberPointer method ) const
    {
        for(size_t i = 0; i<m_overloads.size(); ++i) 
        {
            if(MemberCompare().HaveSameInputTypes(method, m_overloads[i])) 
                return true;
        }
        return false;
    }

    TypeConversions::ConversionGraphPointer OverloadedMember::GetConversionGraph() const
    {
        return m_overloads[0]->GetConversionGraph();
    }

    const Attributes::AttributeCollection& OverloadedMember::GetAttributeCollection() const
    {
        for (auto it = m_overloads.begin(); it != m_overloads.end(); ++it)
        {
            MemberPointer member = *it;
            if (&member->GetAttributeCollection() != &Attributes::AttributeCollection::GetEmptyCollection())
                return member->GetAttributeCollection();
        }
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    Attributes::AttributeCollection& OverloadedMember::GetAttributeCollectionCreateIfMissing()
    {
        throw std::runtime_error(__FUNCTION__ " not implemented.");
    }

    const Formatting::IStringFormatter& OverloadedMember::GetFormatter() const
    {
        for (auto it = m_overloads.begin(); it != m_overloads.end(); ++it)
        {
            MemberPointer member = *it;
            if (&member->GetFormatter() != &Formatting::IdentityFormatter::GetIdentityFormatter())
                return member->GetFormatter();
        }
        return Formatting::IdentityFormatter::GetIdentityFormatter();
    }

    Members::MemberPointer OverloadedMember::GetCompatibleOverload(MemberPointer method) const
    {
        for (auto it = m_overloads.begin(); it != m_overloads.end(); ++it)
        {
            const MemberPointer& next = *it;
            auto compatible = next->GetCompatibleOverload(method);
            if (compatible)
                return compatible;
        }
        return nullptr;
    }

    std::vector<MemberPointer> OverloadedMember::GetOverloads() const
    {
        return m_overloads;
    }

    void OverloadedMember::SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context)
    {
        for (auto it = m_overloads.begin(); it != m_overloads.end(); ++it)
        {
            MemberPointer member = *it;
            member->SetMemberLoggerContext(context);
        }
    }

    std::shared_ptr<Members::MemberLoggerContext> OverloadedMember::GetMemberLoggerContext() const
    {
        for (auto it = m_overloads.begin(); it != m_overloads.end(); ++it)
        {
            MemberPointer member = *it;
            return member->GetMemberLoggerContext();
        }
        return nullptr;
    }

    std::string OverloadedMember::ToString(const std::vector<std::string>& arguments) const
    {
        return m_overloads.at(0)->ToString(arguments);
    }

    void OverloadedMember::RegisterMemberDetails(const std::shared_ptr<MemberLoggerContext>& context)
    {
        for (auto it = m_overloads.begin(); it != m_overloads.end(); ++it)
        {
            MemberPointer member = *it;
            member->RegisterMemberDetails(context);
        }
    }

    bool OverloadedMember::AreCompatibleTypes(MemberType lhs, MemberType rhs) const
    {
        switch(lhs) 
        {
            case MemberType::TypeNone:
                return true;
            case MemberType::TypeConstructor:
            case MemberType::TypeMemberFunction:
            case MemberType::TypeStaticFunction:
            case MemberType::TypeOperator:
                return lhs == rhs;
            case MemberType::TypePropertyGet:
                return rhs == MemberType::TypePropertySet;
            case MemberType::TypePropertySet:
                return rhs == MemberType::TypePropertyGet;
            default:
                return false;
        }
    }

    std::string OverloadedMember::GetName() const
    {
        return m_overloads.front()->GetName();
    }

    bool OverloadedMember::IsVarargMember() const
    {
        return false;
    }

    Variants::Variant OverloadedMember::InvokeNative( const std::vector<Variants::Variant>& arguments )
    {
        throw std::exception(__FUNCTION__ " is not implemented.");
    }

    Variants::Variant OverloadedMember::FastInvoke( std::vector<Variants::Variant>& arguments )
    {
        return Invoke(arguments);
    }

}}}}