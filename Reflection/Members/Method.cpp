//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Method.h"
#include "MemberWithArguments.h"
#include "OverloadedMember.h"
#include "Invoker/FastInvoker.h"
#include "MemberCompare.h"
#include "Reflection/Attributes/DocumentationAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Formatting/IdentityFormatter.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {


    Method::Method(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph ) 
        :   m_conversionGraph(conversionGraph)
        ,   m_minimumArgumentCount(0)
        ,   m_name(name)
        , m_memberLoggerContext(nullptr)
    {

    }

    Members::MemberWithArgumentsPointer Method::PrepareInvoke( const std::vector<Variants::Variant>& arguments, MemberType type)
    {
        return MemberWithArgumentsPointer(new MemberWithArguments(m_conversionGraph, this, arguments, type));
    }

    void Method::SetArgumentList(const std::vector<ArgumentInfoPointer>& argumentList, size_t minimumArgumentCount, size_t maximumArgumentCount)
    {
        m_argumentList = argumentList;
        m_minimumArgumentCount = minimumArgumentCount;
        m_maximumArgumentCount = maximumArgumentCount;
    }
    void Method::SetArgumentCount(size_t arity, size_t minimumArgumentCount, size_t maximumArgumentCount)
    {
        m_argumentList.resize(arity);
        m_minimumArgumentCount = minimumArgumentCount;
        m_maximumArgumentCount = maximumArgumentCount;
    }
    void Method::SetArgumentInfo(size_t index, ArgumentInfoPointer info)
    {
        m_argumentList[index] = info;
    }

    MemberPointer Method::AddOverload( MemberPointer method )
    {
        if(MemberCompare().HaveSameInputTypes(this, method))
            return this;

        MemberPointer member = new OverloadedMember;
        member->AddOverload(this);
        member->AddOverload(method);
        return member;
    }

    ArgumentInfoPointer Method::GetArgumentInfo( size_t iarg ) const
    {
        if (m_argumentList.size() <= iarg)
            return nullptr;
        return m_argumentList.at(iarg);
    }

    size_t Method::GetMinimumArgumentCount() const
    {
        return m_minimumArgumentCount;
    }


    size_t Method::GetMaximumArgumentCount() const
    {
        return m_maximumArgumentCount;
    }
    
    void Method::AddDocumentation(const std::string& doc)
    {
        GetAttributeCollectionCreateIfMissing().AddAttribute(Attributes::DocumentationAttribute(doc));
    }

    const Attributes::AttributeCollection& Method::GetAttributeCollection() const
    {
        if (m_attributeCollection)
            return *m_attributeCollection;
        else
            return Attributes::AttributeCollection::GetEmptyCollection();
    }

    Attributes::AttributeCollection& Method::GetAttributeCollectionCreateIfMissing()
    {
        if (!m_attributeCollection)
            m_attributeCollection.reset(new Attributes::AttributeCollection);
        return *m_attributeCollection;
    }

    void Method::SetFormatter(const Formatting::IStringFormatter& formatter)
    {
        m_formatter.reset(formatter.Clone());
    }

    const Formatting::IStringFormatter& Method::GetFormatter() const
    {
        if (!m_formatter)
            return Formatting::IdentityFormatter::GetIdentityFormatter();
        else
            return *m_formatter;
    }

    Members::MemberPointer Method::GetCompatibleOverload(MemberPointer method) const
    {
        if (typeid(*this) == typeid(*method))
            return Members::MemberPointer(const_cast<Method*>(this));
        else
            return nullptr;
    }

    std::vector<MemberPointer> Method::GetOverloads() const
    {
        return{ const_cast<Method*>(this) };
    }

    void Method::SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context)
    {
        m_memberLoggerContext = context;
    }

    std::shared_ptr<Members::MemberLoggerContext> Method::GetMemberLoggerContext() const
    {
        return m_memberLoggerContext;
    }

    Variants::Variant Method::FastInvoke(std::vector<Variants::Variant>& arguments)
    {
        if(!m_invoker) 
            m_invoker.reset(new Invoker::FastInvoker(this));
        return m_invoker->Invoke(arguments);
    }

    Method::~Method()
    {

    }

}}}}