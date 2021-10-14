#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "IMember.h"
#include "Reflection/TypeConversions/IConversionGraph.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    namespace Invoker { class FastInvoker; }
    
    /// <summary>
    /// This is the base class for all members of a class. See Wiki page for documentation on the individual members.    
    /// </summary>
    /// <seealso cref="IMember" />
    class REFLECTION_IMPORT_EXPORT Method : public IMember
    {
    public:
        Method(const std::string& name, TypeConversions::ConversionGraphPointer conversionGraph);
        ~Method();
        ///Prepares the argument conversions etc. for this method, but does not actually invoke the member. You can validate the result prior to invoking.
        virtual MemberWithArgumentsPointer PrepareInvoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll);
        virtual Variants::Variant FastInvoke( std::vector<Variants::Variant>& arguments );
        virtual MemberPointer AddOverload(MemberPointer method);
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const {return m_conversionGraph; }
        virtual ArgumentInfoPointer GetArgumentInfo( size_t iarg ) const;
        virtual size_t GetMinimumArgumentCount() const;
        virtual size_t GetMaximumArgumentCount() const;
        void AddDocumentation(const std::string& doc);
        virtual std::string GetName() const {return m_name; }
        virtual void SetName(const std::string& name) { m_name = name; }
        virtual bool IsVarargMember() const {return false; }
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const;
        virtual Attributes::AttributeCollection& GetAttributeCollectionCreateIfMissing();
        void SetFormatter(const DNVS::MoFa::Formatting::IStringFormatter& formatter);
        virtual const DNVS::MoFa::Formatting::IStringFormatter& GetFormatter() const;
        virtual MemberPointer GetCompatibleOverload(MemberPointer method) const override;
        virtual std::vector<MemberPointer> GetOverloads() const override;
        virtual void SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context) override;
        virtual std::shared_ptr<MemberLoggerContext> GetMemberLoggerContext() const override;

    protected:
        void SetArgumentList(const std::vector<ArgumentInfoPointer>& argumentList, size_t minimumArgumentCount, size_t maximumArgumentCount);
        void SetArgumentCount(size_t arity, size_t minimumArgumentCount, size_t maximumArgumentCount);
        void SetArgumentInfo(size_t index, ArgumentInfoPointer info);
    private:
        std::unique_ptr<Invoker::FastInvoker> m_invoker;
        TypeConversions::ConversionGraphPointer m_conversionGraph;
        std::vector<ArgumentInfoPointer> m_argumentList;
        size_t m_minimumArgumentCount;
        size_t m_maximumArgumentCount;
        std::string m_name;
        std::unique_ptr<Attributes::AttributeCollection> m_attributeCollection;
        std::unique_ptr<DNVS::MoFa::Formatting::IStringFormatter> m_formatter;
        std::shared_ptr<MemberLoggerContext> m_memberLoggerContext;
    };
}}}}
