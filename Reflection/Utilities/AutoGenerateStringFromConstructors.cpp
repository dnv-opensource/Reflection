//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ArgumentFromObjectExtracter.h"
#include "AutoGenerateStringFromConstructors.h"
#include "CallableConstructor.h"
#include "ConstructorSelector.h"
#include "Reflection/Attributes/AlternativeNullPointerNameAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/ContainerAttribute.h"
#include "Reflection/Attributes/DefaultConstructorSelectorAttribute.h"
#include "Reflection/Attributes/GenerateConstructorArgumentsAttribute.h"
#include "Reflection/Attributes/ObjectNameAttribute.h"
#include "Reflection/Attributes/ShortSyntaxAttribute.h"
#include "Reflection/Objects/Delegate.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Formatting/DefaultFormatterRules.h"
#include "Reflection/Attributes/NameAttribute.h"
#include "DefaultArgumentExtracter.h"
#include "Reflection/Attributes/MethodToStringAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {

    bool TryFormatNullPointer(const Objects::Object& object, std::string& text, const Formatting::FormattingService& formattingService)
    {
        if (!Types::IsPointer(object.GetDecoratedTypeInfo()))
            return false;
        if (object.GetVariant().GetData() != nullptr)
            return false;
        auto type = object.GetType();
        if (type && type->GetAttributeCollection().HasAttribute<Attributes::AlternativeNullPointerNameAttribute>())
        {
            text = type->GetAttributeCollection().GetAttribute<Attributes::AlternativeNullPointerNameAttribute>().GetNullPointerName();
        }
        else
            text = "null";
        return true;
    }

    bool TryFormatObjectName(const Objects::Object& object, std::string& text, const Formatting::FormattingService& formattingService)
    {
        auto type = object.GetType();
        if (type && type->GetAttributeCollection().HasAttribute<Attributes::NameAttribute>())
        {
            text = type->GetAttributeCollection().GetAttribute<Attributes::NameAttribute>().GetName(object);
            if (!text.empty())
                return true;
        }
        using namespace Formatting;
        if (formattingService.GetFormatterOrDefault<IFormatterRules, DefaultFormatterRules>()->ExpandExpression())
            return false;
        if (type && type->GetAttributeCollection().HasAttribute<Attributes::ObjectNameAttribute>())
        {
            text = type->GetAttributeCollection().GetAttribute<Attributes::ObjectNameAttribute>().GetName(object);
            return !text.empty();
        }
        return false;
    }

    bool TryFormatArray(Objects::Object object, std::string& text, const Formatting::FormattingService& formattingService)
    {
        auto type = object.GetType();
        if(type && type->GetAttributeCollection().HasAttribute<Attributes::ContainerAttribute>())
        { 
            text = "Array(";
            bool isFirst = true;
            Objects::Object it = object.Lookup("begin")().ToLValue();
            Objects::Object iend = object.Lookup("end")();
            for (; it != iend; ++it)
            {
                if (!isFirst)
                    text += ", ";
                isFirst = false;
                text += ToString(*it, formattingService);
            }
            text += ")";
            return true;
        }
        return false;
    }

    Formatting::FormattingService GetExactSyntaxFormatter(Formatting::FormattingService formattingService)
    {
        using namespace Formatting;
        auto defaultFormatter = formattingService.GetFormatterOrDefault<IFormatterRules, DefaultFormatterRules>();
        formattingService.AddFormatter<IFormatterRules>(std::make_shared<DefaultFormatterRules>(defaultFormatter->ShouldEncloseExpression(), IFormatterRules::Exact, defaultFormatter->RequireValidScript(), defaultFormatter->ExpandExpression()));
        return formattingService;
    }

    std::string FormatFromConstructorExactSyntax_UsingAttribute(const CallableConstructor& constructor, Objects::Object object, const Formatting::FormattingService& formattingService)
    {
        std::vector<std::pair<std::string,Objects::Object>> arguments;
        for (size_t i = 0; i < constructor.GetArguments().size(); ++i)
        {
            Objects::Object arg(object.GetTypeLibrary(), constructor.GetArguments()[i]);
            arguments.push_back(std::make_pair(ToString(arg, formattingService),arg));
        }
        return constructor.GetMember()->GetAttributeCollection().GetAttribute<Attributes::MethodToStringAttribute>().ToString(arguments);
    }

    std::string FormatFromConstructorExactSyntax(const CallableConstructor& constructor, Objects::Object object, const Formatting::FormattingService& formattingService)
    {
        bool allowSimpleSyntax = false;
        auto exactSyntaxFormatter = GetExactSyntaxFormatter(formattingService);
        if(constructor.GetMember()->GetAttributeCollection().HasAttribute<Attributes::MethodToStringAttribute>())
        {
            try {
                return FormatFromConstructorExactSyntax_UsingAttribute(constructor, object, exactSyntaxFormatter);
            }
            catch(...)
            {}
        }
        std::vector<std::string> arguments;
        for(size_t i = 0; i < constructor.GetArguments().size(); ++i)
        {
            Objects::Object arg(object.GetTypeLibrary(), constructor.GetArguments()[i]);
            arguments.push_back(ToString(arg, exactSyntaxFormatter));
        }
        return constructor.GetMember()->ToString(arguments);
    }

    std::string FormatFromConstructorShortSyntax(const CallableConstructor& constructor, Objects::Object object, const Formatting::FormattingService& formattingService)
    {
        const auto& shortSyntaxAttribute = constructor.GetMember()->GetAttributeCollection().GetAttribute<Attributes::ShortSyntaxAttribute>();
        if (shortSyntaxAttribute.GetArguments().size() != constructor.GetMember()->GetArity())
            return FormatFromConstructorExactSyntax(constructor, object, formattingService);
        std::vector<std::string> arguments;
        for (size_t i = 0; i < shortSyntaxAttribute.GetArguments().size(); ++i)
        {
            switch (shortSyntaxAttribute.GetArguments()[i])
            {
            case Attributes::ShortSyntaxArgument::ShortSyntax:
                arguments.push_back(ToString(Objects::Object(object.GetTypeLibrary(), constructor.GetArguments()[i]), formattingService));
                break;
            case Attributes::ShortSyntaxArgument::ScriptSyntax:
                arguments.push_back(ToString(Objects::Object(object.GetTypeLibrary(), constructor.GetArguments()[i]), GetExactSyntaxFormatter(formattingService)));
                break;
            default:
                break;
            }
        }
        std::string result;
        for (const std::string& arg: arguments)
        {
            if (!result.empty())
                result += " ";
            result += arg;
        }
        return result;
    }

    std::string FormatFromConstructor(const CallableConstructor& constructor, Objects::Object object, const Formatting::FormattingService& formattingService)
    {
        using namespace Formatting;
        if (formattingService.GetFormatterOrDefault<IFormatterRules, DefaultFormatterRules>()->GetExpressionRepresentation() == IFormatterRules::SimplifiedGui &&
            constructor.GetMember()->GetAttributeCollection().HasAttribute<Attributes::ShortSyntaxAttribute>())
            return FormatFromConstructorShortSyntax(constructor, object, formattingService);
        return FormatFromConstructorExactSyntax(constructor, object, formattingService);
    }

    std::string AutoGenerateStringFromConstructors(Objects::Object object, const Formatting::FormattingService& formattingService)
    {
        object = object.ConvertToDynamicType();
        if (!object.IsValid())
            return "Object is not valid";
        if (!object.GetTypeLibrary())
            return "Type library is not set for object";
        //Handle null pointers.
        std::string text;
        if (TryFormatNullPointer(object, text, formattingService))
            return text;
        if (TryFormatObjectName(object, text, formattingService))
            return text;
        if (TryFormatArray(object, text, formattingService))
            return text;
        ConstructorSelector selector(object);
        CallableConstructor constructor = selector.GetBestConstructor();
        if (!constructor.IsValid())
            return "No suitable constructors found to generate string from " + object.GetTypeLibrary()->GetTypeFormatter()->FormatType(object.GetDecoratedTypeInfo());
        return FormatFromConstructor(constructor, object, formattingService);
    }


}}}}


