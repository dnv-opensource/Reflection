#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "ILayoutElement.h"
#include "OverloadSelector.h"
#include "VerticalElements.h"
#include "EmbeddedElement.h"
#include "Selector.h"
#include "DynamicArea.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    class IncludeInViewAttribute;
}}}}

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class IElementAdder;

    class REFLECTION_IMPORT_EXPORT LayoutBuilder {
    public:
        LayoutBuilder(const TypeLibraries::TypeLibraryPointer& typeLibrary, bool nested = false);
        LayoutElementPointer CreateLayoutFromProperty(Members::MemberPointer property, Members::MemberPointer function = nullptr);
        LayoutElementPointer CreateLayoutFromType(const std::string& name, const Types::DecoratedTypeInfo& typeInfo, Members::MemberPointer function = nullptr);
        LayoutElementPointer CreateLayoutFromType(const std::string& name, const Types::DecoratedTypeInfo& typeInfo, const Attributes::AttributeCollection& typeCollection, Members::MemberPointer function = nullptr);
        LayoutElementPointer CreateLayoutFromFunctionArgument(Members::MemberPointer function, size_t index);
        const Attributes::AttributeCollection& TryGetAttributeCollection(Members::MemberPointer function, size_t index);
        LayoutElementPointer CreateLayoutFromType(const Types::DecoratedTypeInfo& typeInfo);
        LayoutElementPointer CreateLayoutFromFunction(Members::MemberPointer function);
        LayoutElementPointer CreateLayoutFromMember(Members::MemberPointer member, bool allowProperty = true);
        LayoutElementPointer CreateLayoutFromFunctionArgumentOrProperty(Members::MemberPointer function, size_t index, const TypeLibraries::TypePointer& type, IElementAdder* adder = nullptr, const Attributes::IncludeInViewAttribute* includeInView = nullptr);

        template <typename T, typename Layout, typename... Methods>
        LayoutElementPointer CreateLayoutFromMembers(const std::string& title, const Layout& layout, Methods&&... methods)
        {
            std::list<Members::MemberPointer> members;
            CollectMembers<T>(members, std::forward<Methods>(methods)...);
            if (members.empty())
                throw std::runtime_error("No members found");
            OverloadSelector selector("Functions", (VerticalElements(Selector(layout), DynamicArea())));
            selector.SetMembers(members);
            selector.GetContext().parentType = Types::TypeId<T>();
            selector.GetContext().type = Types::TypeId<T>();
            EmbeddedElement embedded(title, VerticalElements(CreateLayoutFromType("__this__", Types::TypeId<T>()), selector));
            embedded.GetContext().type = Types::TypeId<T>();
            return MakeElement(std::move(embedded));
        }
    private:
        LayoutElementPointer CreateGridLayout(const Members::MemberPointer& function);
        bool HasAlias(const TypeLibraries::TypePointer& type, const std::string& name, const Attributes::IncludeInViewAttribute* includeInView = nullptr) const;
        template <typename T>
        void CollectMembers(std::list<Members::MemberPointer>& members)
        {}

        template <typename T, typename... Methods>
        void CollectMembers(std::list<Members::MemberPointer>& members, const Members::MemberPointer& member, Methods&&... methods)
        {
            if (!member)
                return;
            for (const auto& overload : member->GetOverloads())
            {
                members.emplace_back(overload);
            }
            CollectMembers<T>(members, std::forward<Methods>(methods)...);
        }

        template <typename T, typename... Methods>
        void CollectMembers(std::list<Members::MemberPointer>& members, const std::string& method, Methods&&... methods)
        {
            auto type = m_typeLibrary->LookupType(Types::TypeId<T>().GetTypeInfo());
            if (!type)
                return;
            auto member = type->Lookup(method, Variants::Variant());
            if (member)
                CollectMembers<T>(members, member);
            else
                throw std::runtime_error("Method '" + method + "' not found for type " + m_typeLibrary->GetTypeFormatter()->FormatType(Types::TypeId<T>()));
            CollectMembers<T>(members, std::forward<Methods>(methods)...);
        }
        LayoutElementPointer CreateLayoutFromTypeUsingLayoutAttribute(const Types::DecoratedTypeInfo& typeInfo, const TypeLibraries::TypePointer& type);
        LayoutElementPointer CreateLayoutFromTypeUsingElementAdder(const Types::DecoratedTypeInfo& typeInfo, const TypeLibraries::TypePointer& type, std::unique_ptr<IElementAdder> adder);
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        bool m_isNested;
    };
}}}}
