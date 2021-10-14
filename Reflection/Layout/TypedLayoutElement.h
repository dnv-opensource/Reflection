#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "TypeUtilities/Visitors/IVisitor.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutVisitor;
    class ILayoutElement;

    template<typename DerivedT, typename BaseT>
    class TypedLayoutElement : public BaseT
    {
    public:
        TypedLayoutElement(TypedLayoutElement&&) = default;
        TypedLayoutElement(const TypedLayoutElement&) = default;
        TypedLayoutElement& operator=(TypedLayoutElement&& other) = default;
        TypedLayoutElement& operator=(const TypedLayoutElement& other) = default;

        template<typename... Args>
        TypedLayoutElement(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {}
        virtual void Accept(TypeUtilities::IVisitor& generator) const
        {
            if (!AcceptImpl(GetDerived(), generator))
                BaseT::Accept(generator);
        }
        const DerivedT& GetDerived() const
        {
            return *static_cast<const DerivedT*>(this);
        }
        virtual std::shared_ptr<ILayoutElement> Clone() const override
        {
            return std::make_shared<DerivedT>(GetDerived());
        }
        virtual void SetParentType(TypeLibraries::TypeLibraryPointer typeLibrary, Types::DecoratedTypeInfo& typeInfo)
        {}
    };
}}}}
