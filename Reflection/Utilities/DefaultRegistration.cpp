//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultRegistration.h"
#include "Reflection/Classes/Class.h"
#include <sstream>
#include <float.h>
#include "Reflection/TypeLibraries/TypeNotCaseSensitive.h"
#include "Reflection/TypeLibraries/TypeWithGlobalMemberRegistration.h"
#include "Reflection/TypeLibraries/Type.h"
#include "Reflection/Attributes/ParserAttribute.h"
#include "Formatting/ToString.h"
#include "ConstructibleItem.h"
#include "MemberItem.h"
#include "boost\lexical_cast.hpp"
#include "boost\optional.hpp"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {

    template<typename T>
    struct FundamentalParser {
    public:
        boost::optional<T> operator()(const std::string& text, const Formatting::FormattingService& service) const
        {
            return boost::lexical_cast<T>(text);
        }
    };

    template<>
    struct FundamentalParser<double> {
    public:
        boost::optional<double> operator()(const std::string& text, const Formatting::FormattingService& service) const
        {
            if (text == "NaN")
            {
                return std::numeric_limits<double>::quiet_NaN();
            }
            else if (text == "+Infinity")
            {
                return std::numeric_limits<double>::infinity();
            }
            else if (text == "-Infinity")
            {
                return -std::numeric_limits<double>::infinity();
            }
            else {
                return boost::lexical_cast<double>(text);
            }
        }
    };

    template<>
    struct FundamentalParser<bool> {
    public:
        boost::optional<bool> operator()(const std::string& text, const Formatting::FormattingService& service) const
        {
            if (text == "true")
                return true;
            else
                return false;
        }
    };

    template<typename T>
    void ReflectFundamental(TypeLibraries::TypeLibraryPointer typeLibrary)
    {
        using namespace Classes;
        Class<T> cls(typeLibrary, "");
        cls.AddAttribute<ParserAttribute>(FundamentalParser<T>());
        cls.Operator(This.Const == This.Const);
        cls.Operator(This.Const != This.Const);
        cls.Operator(This.Const < This.Const);
        cls.Operator(This.Const > This.Const);
        cls.Operator(This.Const <= This.Const);
        cls.Operator(This.Const >= This.Const);
        RegisterToStringFunction(cls);
    }

    void DefaultRegistration::Reflect(TypeLibraries::TypeLibraryPointer typeLibrary)
    {
        ReflectFundamental<unsigned char>(typeLibrary);
        ReflectFundamental<signed char>(typeLibrary);
        ReflectFundamental<unsigned short>(typeLibrary);
        ReflectFundamental<signed short>(typeLibrary);
        ReflectFundamental<unsigned int>(typeLibrary);
        ReflectFundamental<signed int>(typeLibrary);
        ReflectFundamental<unsigned long>(typeLibrary);
        ReflectFundamental<signed long>(typeLibrary);
        ReflectFundamental<unsigned __int64>(typeLibrary);
        ReflectFundamental<signed __int64>(typeLibrary);

        ReflectFundamental<char>(typeLibrary);
        ReflectFundamental<bool>(typeLibrary);
        ReflectFundamental<float>(typeLibrary);
        ReflectFundamental<double>(typeLibrary);

        Reflection::Reflect<TypeLibraries::Type>(typeLibrary);
        Reflection::Reflect<TypeLibraries::TypeWithGlobalMemberRegistration>(typeLibrary);
        Reflection::Reflect<TypeLibraries::TypeNotCaseSensitive>(typeLibrary);
        Reflection::Reflect<ConstructibleItem>(typeLibrary);
        Reflection::Reflect<MemberItem>(typeLibrary);
    }
}}}}


