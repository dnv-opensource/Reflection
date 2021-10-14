#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <type_traits>
#include "Reflection/Config.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Types/DecoratedTypeInfo.h"
#include <string>
#include "TypeUtilities/Visitors/IVisitable.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutElement : public TypeUtilities::IVisitable {
    public:
        virtual ~ILayoutElement() {}
        virtual void Accept(TypeUtilities::IVisitor& generator) const = 0
        {
            AcceptImpl(*this, generator);
        }
        virtual std::shared_ptr<ILayoutElement> Clone() const = 0;
        virtual void SetParentType(TypeLibraries::TypeLibraryPointer typeLibrary, Types::DecoratedTypeInfo& typeInfo) = 0;
    };
    bool REFLECTION_IMPORT_EXPORT operator==(const ILayoutElement& lhs, const ILayoutElement& rhs);
    bool REFLECTION_IMPORT_EXPORT operator!=(const ILayoutElement& lhs, const ILayoutElement& rhs);
    std::string REFLECTION_IMPORT_EXPORT ToString(const ILayoutElement& element);
    template<class _E, class _Tr>
    inline std::basic_ostream<_E, _Tr>& __cdecl operator<<(std::basic_ostream<_E, _Tr>& _O, const ILayoutElement& element) {
        return _O << ToString(element);
    }

    typedef std::shared_ptr<ILayoutElement> LayoutElementPointer;
    template<typename ElementT>
    std::enable_if_t<std::is_base_of_v<ILayoutElement, ElementT>, LayoutElementPointer> MakeElement(ElementT&& element)
    {
        return std::make_shared<ElementT>(std::move(element));
    }

    template<typename ElementT>
    std::enable_if_t<std::is_base_of_v<ILayoutElement, ElementT>, LayoutElementPointer> MakeElement(const std::shared_ptr<ElementT>& element)
    {
        return element;
    }

    REFLECTION_IMPORT_EXPORT LayoutElementPointer MakeElement(const std::string& name);

}}}}
