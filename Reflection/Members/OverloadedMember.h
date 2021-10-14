#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    
    /// <summary>
    /// This class keeps track of overloaded members.
    /// If you have a class:
    /// Class<Beam> cls(...);
    /// cls.Constructor<pos3d,pos3d>();
    /// cls.Constructor<GuideCurve>();
    /// The second constructor definition will cause an OverloadedMember to be created to contain the two constructor definitions.
    /// </summary>
    /// <seealso cref="IMember" />
    class OverloadedMember : public IMember
    {
    public:        
        /// <summary>
        /// Invokes the overloaded method with the best match to the supplied arguments.
        /// If more than one method found that has the same match to the supplied arguments, an exception is thrown.
        /// </summary>
        /// <param name="arguments">The arguments.</param>
        /// <param name="type">The type.</param>
        /// <returns></returns>
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll);
        virtual Variants::Variant InvokeNative( const std::vector<Variants::Variant>& arguments );
        virtual ArgumentInfoPointer GetArgumentInfo(size_t iarg) const;
        virtual Types::DecoratedTypeInfo GetReturnType() const;
        virtual bool IsConst() const;
        virtual bool IsVarargMember() const;
        virtual size_t GetArity() const;
        virtual ~OverloadedMember() {}

        virtual MemberWithArgumentsPointer PrepareInvoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll);
        virtual void Validate(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll);
        virtual bool HasValidation() const override;
        virtual size_t GetMinimumArgumentCount() const;
        virtual size_t GetMaximumArgumentCount() const;

        virtual MemberPointer AddOverload( MemberPointer method );
        ///Checks if this overload, or one with the exact same arguments is already registered with the reflection library.
        bool IsOverloadRegistered(MemberPointer method) const;
        virtual MemberType GetMemberType() const;
        virtual std::string GetName() const;
        
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const;
        virtual Attributes::AttributeCollection& GetAttributeCollectionCreateIfMissing();
        virtual const Formatting::IStringFormatter& GetFormatter() const;
        virtual MemberPointer GetCompatibleOverload(MemberPointer method) const override;
        virtual std::vector<MemberPointer> GetOverloads() const override;
        virtual void SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context) override;
        virtual std::shared_ptr<MemberLoggerContext> GetMemberLoggerContext() const override;
        virtual std::string ToString(const std::vector<std::string>& arguments) const;
        virtual void RegisterMemberDetails(const std::shared_ptr<MemberLoggerContext>& context);
    private:
        bool AreCompatibleTypes(MemberType lhs, MemberType rhs) const;

        virtual Variants::Variant FastInvoke( std::vector<Variants::Variant>& arguments );


        std::vector<MemberPointer> m_overloads;
    };
}}}}
