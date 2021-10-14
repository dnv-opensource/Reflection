#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"
#include "IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    
    /// <summary>
    /// This class has as its sole purpose to override the first argument to a function call.
    /// Example: We have a class cfMemberOptions that owns an object of type ConeLengthMemOpt.
    /// ConeLengthMemOpt has a member called coneLength.
    /// Due to compatibility, this has been scripted as "memberOption.coneLength";
    /// The class ConeLengthMemOpt is omitted altogether.
    /// Here, we override the lookup method of cfMemberOptions to allow it to search for members of ConeLengthMemOpt
    /// It finds "coneLength", but it requires the class to be set to ConeLengthMemOpt. However, what we supply is a pointer to the cfMemberOptions class:
    ///              variable owned by ConeLengthMemOpt
    /// cfMemberOptions   |
    ///      |            |
    /// memberOptions.coneLength
    /// The way we solve this, is that during lookup, we don't return the original member, instead we return MemberWithPredefinedFirstArgument with the appropriate ConeLengthMemOpt passed as the cls argument.
    /// This solves the problem.
    /// </summary>
    /// <seealso cref="IMember" />
    class REFLECTION_IMPORT_EXPORT MemberWithPredefinedFirstArgument : public IMember
    {
    public:
        MemberWithPredefinedFirstArgument(MemberPointer member, const Variants::Variant& cls);
        ~MemberWithPredefinedFirstArgument();

        virtual MemberWithArgumentsPointer PrepareInvoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll) override;
        virtual Variants::Variant FastInvoke(std::vector<Variants::Variant>& arguments) override;
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll) override;
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments) override;
        virtual void Validate(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll) override;
        virtual bool HasValidation() const override;
        virtual ArgumentInfoPointer GetArgumentInfo(size_t iarg) const override;
        virtual Types::DecoratedTypeInfo GetReturnType() const override;
        virtual bool IsConst() const override;
        virtual bool IsVarargMember() const override;
        virtual size_t GetArity() const override;
        virtual size_t GetMinimumArgumentCount() const override;
        virtual size_t GetMaximumArgumentCount() const override;
        virtual MemberPointer GetCompatibleOverload(MemberPointer method) const override;
        virtual MemberPointer AddOverload(MemberPointer method) override;
        virtual MemberType GetMemberType() const override;
        virtual std::string GetName() const override;
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const override;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const override;
        virtual const DNVS::MoFa::Formatting::IStringFormatter& GetFormatter() const override;
        virtual std::vector<MemberPointer> GetOverloads() const override;
        virtual void SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context) override;
        virtual std::shared_ptr<MemberLoggerContext> GetMemberLoggerContext() const override;
        virtual std::string ToString(const std::vector<std::string>& arguments) const;
        virtual void RegisterMemberDetails(const std::shared_ptr<MemberLoggerContext>& context);

        Attributes::AttributeCollection& GetAttributeCollectionCreateIfMissing() override;

    private:
        MemberPointer m_member;
        Variants::Variant m_cls;
    };
}}}}
