//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberWithPredefinedFirstArgument.h"
#include "MemberWithArguments.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {

    MemberWithPredefinedFirstArgument::MemberWithPredefinedFirstArgument(MemberPointer member, const Variants::Variant& cls)
        :   m_member(member)
        ,   m_cls(cls)
    {
    }

    MemberWithPredefinedFirstArgument::~MemberWithPredefinedFirstArgument()
    {

    }

    Members::MemberWithArgumentsPointer MemberWithPredefinedFirstArgument::PrepareInvoke(const std::vector<Variants::Variant>& arguments, MemberType type /*= MemberType::TypeAll*/)
    {
        std::vector<Variants::Variant> replacedArgs(arguments);
        replacedArgs[0] = m_cls;
        return m_member->PrepareInvoke(replacedArgs, type);
    }

    Variants::Variant MemberWithPredefinedFirstArgument::FastInvoke(std::vector<Variants::Variant>& arguments)
    {
        Variants::Variant old = arguments[0];
        arguments[0] = m_cls;
        Variants::Variant result = m_member->FastInvoke(arguments);
        arguments[0] = old;
        return result;
    }

    Variants::Variant MemberWithPredefinedFirstArgument::Invoke(const std::vector<Variants::Variant>& arguments, MemberType type /*= MemberType::TypeAll*/)
    {
        std::vector<Variants::Variant> replacedArgs(arguments);
        replacedArgs[0] = m_cls;
        return m_member->Invoke(replacedArgs, type);
    }

    Variants::Variant MemberWithPredefinedFirstArgument::InvokeNative(const std::vector<Variants::Variant>& arguments)
    {
        std::vector<Variants::Variant> replacedArgs(arguments);
        replacedArgs[0] = m_cls;
        return m_member->InvokeNative(replacedArgs);
    }

    void MemberWithPredefinedFirstArgument::Validate(const std::vector<Variants::Variant>& arguments, MemberType type /*= MemberType::TypeAll*/)
    {
        std::vector<Variants::Variant> replacedArgs(arguments);
        replacedArgs[0] = m_cls;
        m_member->Validate(replacedArgs);
    }

    bool MemberWithPredefinedFirstArgument::HasValidation() const
    {
        return m_member->HasValidation();
    }

    Members::ArgumentInfoPointer MemberWithPredefinedFirstArgument::GetArgumentInfo(size_t iarg) const
    {
        return m_member->GetArgumentInfo(iarg);
    }

    Types::DecoratedTypeInfo MemberWithPredefinedFirstArgument::GetReturnType() const
    {
        return m_member->GetReturnType();
    }

    bool MemberWithPredefinedFirstArgument::IsConst() const
    {
        return m_member->IsConst();
    }

    bool MemberWithPredefinedFirstArgument::IsVarargMember() const
    {
        return m_member->IsVarargMember();
    }

    size_t MemberWithPredefinedFirstArgument::GetArity() const
    {
        return m_member->GetArity();
    }

    size_t MemberWithPredefinedFirstArgument::GetMinimumArgumentCount() const
    {
        return m_member->GetMinimumArgumentCount();
    }

    size_t MemberWithPredefinedFirstArgument::GetMaximumArgumentCount() const
    {
        return m_member->GetMaximumArgumentCount();
    }

    Members::MemberPointer MemberWithPredefinedFirstArgument::GetCompatibleOverload(MemberPointer method) const
    {
        throw std::logic_error(__FUNCTION__ " is not implemented.");
    }

    Members::MemberPointer MemberWithPredefinedFirstArgument::AddOverload(MemberPointer method)
    {
        throw std::logic_error(__FUNCTION__ " is not implemented.");
    }

    Members::MemberType MemberWithPredefinedFirstArgument::GetMemberType() const
    {
        return m_member->GetMemberType();
    }

    std::string MemberWithPredefinedFirstArgument::GetName() const
    {
        return m_member->GetName();
    }

    TypeConversions::ConversionGraphPointer MemberWithPredefinedFirstArgument::GetConversionGraph() const
    {
        return m_member->GetConversionGraph();
    }

    const Attributes::AttributeCollection& MemberWithPredefinedFirstArgument::GetAttributeCollection() const
    {
        return m_member->GetAttributeCollection();
    }

    Attributes::AttributeCollection& MemberWithPredefinedFirstArgument::GetAttributeCollectionCreateIfMissing()
    {
        return m_member->GetAttributeCollectionCreateIfMissing();
    }

    const DNVS::MoFa::Formatting::IStringFormatter& MemberWithPredefinedFirstArgument::GetFormatter() const
    {
        return m_member->GetFormatter();
    }

    std::vector<MemberPointer> MemberWithPredefinedFirstArgument::GetOverloads() const
    {
        return{ const_cast<MemberWithPredefinedFirstArgument*>(this) };
    }

    void MemberWithPredefinedFirstArgument::SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context)
    {
        m_member->SetMemberLoggerContext(context);
    }

    std::shared_ptr<Members::MemberLoggerContext> MemberWithPredefinedFirstArgument::GetMemberLoggerContext() const
    {
        return m_member->GetMemberLoggerContext();
    }

    std::string MemberWithPredefinedFirstArgument::ToString(const std::vector<std::string>& arguments) const
    {
        return m_member->ToString(arguments);
    }

    void MemberWithPredefinedFirstArgument::RegisterMemberDetails(const std::shared_ptr<MemberLoggerContext>& context)
    {
        return;
    }

}}}}


