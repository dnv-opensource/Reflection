#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "MemberWithArgumentsFwd.h"
#include "IMemberFwd.h"
#include "MemberType.h"
#include "ArgumentInfo.h"

#include "Reflection/Variants/Variant.h"

#include "Reflection/TypeConversions/IConversionGraph.h"

#include "TypeUtilities/IntrusiveClass.h"
#include "Formatting\IStringFormatter.h"
#include "MemberLoggerContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    class AttributeCollection;
}}}}

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    static const std::string ConstructorString = "__Constructor__";
        
    class REFLECTION_IMPORT_EXPORT IMember : public TypeUtilities::IntrusiveClass<IMember>
    {
    public:
        ///Prepares the argument conversions etc. for this method, but does not actually invoke the member. You can validate the result prior to invoking.
        virtual MemberWithArgumentsPointer PrepareInvoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll) = 0;
        ///Use the FastInvoker to do fast type conversions, then call InvokeNative. Not validation. arguments are converted in-place.
        virtual Variants::Variant FastInvoke(std::vector<Variants::Variant>& arguments) = 0;
        ///Invokes the method. Do full type conversion. Use validation
        virtual Variants::Variant Invoke(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll) = 0;
        ///Invokes the method without doing any type conversions. Assumes variables are of correct type.
        virtual Variants::Variant InvokeNative(const std::vector<Variants::Variant>& arguments) = 0;
        ///Run argument validation
        virtual void Validate(const std::vector<Variants::Variant>& arguments, MemberType type = MemberType::TypeAll) = 0;
        virtual bool HasValidation() const = 0;
        ///Gets information about the i-th argument
        virtual ArgumentInfoPointer GetArgumentInfo(size_t iarg) const = 0;
        ///Gets information about the return type
        virtual Types::DecoratedTypeInfo GetReturnType() const = 0;
        ///Returns true if this method does not modify the data, but is only a lookup method.
        virtual bool IsConst() const = 0;

		///Returns true if this is a vararg member.
        virtual bool IsVarargMember() const = 0;

        ///Gets the arity of the member, i.e. how many arguments it takes
        virtual size_t GetArity() const = 0;

        ///Returns the minimum number of required arguments. Differs from arity if default arguments have been supplied.
        virtual size_t GetMinimumArgumentCount() const = 0;
        ///Returns the maximum number of arguments. For varargs this can be numeric_limits<size_t>::max()
        virtual size_t GetMaximumArgumentCount() const = 0;

        ///Adds an overload to this member. Returns a new member if needed to hold the new overload information.
        virtual MemberPointer GetCompatibleOverload(MemberPointer method) const = 0;
        virtual MemberPointer AddOverload(MemberPointer method) = 0;
        virtual std::vector<MemberPointer> GetOverloads() const = 0;
        virtual MemberType GetMemberType() const = 0;
        virtual std::string GetName() const = 0;
        virtual void SetName(const std::string& name) { throw std::runtime_error(__FUNCTION__ "not implemented"); }
        virtual std::string GetActualName() const { return GetName(); }
        virtual TypeConversions::ConversionGraphPointer GetConversionGraph() const = 0;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const = 0;
        virtual Attributes::AttributeCollection& GetAttributeCollectionCreateIfMissing() = 0;
        virtual const DNVS::MoFa::Formatting::IStringFormatter& GetFormatter() const = 0;
        virtual ~IMember() {}
        virtual std::string ToString(const std::vector<std::string>& arguments) const = 0;
        virtual void SetMemberLoggerContext(const std::shared_ptr<MemberLoggerContext>& context) = 0;
        virtual std::shared_ptr<MemberLoggerContext> GetMemberLoggerContext() const = 0;
        virtual void RegisterMemberDetails(const std::shared_ptr<MemberLoggerContext>& context) = 0;
        enum class FormatType
        {
            FunctionSignature = 1,
            FunctionBody = 2,
            Function = 3,
        };
        virtual std::string Format(FormatType formatType) { return ""; }
    };
}}}}
