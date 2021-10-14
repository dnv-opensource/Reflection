#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
#include <functional>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class REFLECTION_IMPORT_EXPORT LambdaPropertyAspect : public IControlAspectT<LambdaPropertyAspect>
    {
    public:
        template<typename CallbackT>
        LambdaPropertyAspect(const std::string& name, TypeLibraries::TypeLibraryPointer typeLibrary, CallbackT callback)
            : m_name(name)
            , m_decoratedTypeInfo(Types::TypeId<typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType>())
        {
            m_callback = [=](const Contents::Content& content) {
                std::vector<Contents::Content> result;
                for (const auto& child : content.Split())
                    result.emplace_back(Contents::Content(typeLibrary, callback(child.As<TypeUtilities::FunctionTraits<CallbackT>::Argument0>())));
                return Contents::Content(result);
            };            
        }
        template<typename CallbackT>
        LambdaPropertyAspect(TypeLibraries::TypeLibraryPointer typeLibrary, CallbackT callback)
            : m_decoratedTypeInfo(Types::TypeId<typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType>())
        {
            m_callback = [=](const Contents::Content& content) {
                std::vector<Contents::Content> result;
                for (const auto& child : content.Split())
                    result.emplace_back(Contents::Content(typeLibrary, callback(child.As<TypeUtilities::FunctionTraits<CallbackT>::Argument0>())));
                return Contents::Content(result);
            };
        }
        template<typename CallbackT>
        LambdaPropertyAspect(TypeLibraries::TypeLibraryPointer typeLibrary, CallbackT callback, const std::type_info& info)
            : m_decoratedTypeInfo(info,Types::TypeId<typename TypeUtilities::FunctionTraits<CallbackT>::ReturnType>().GetDecoration())
        {            
            m_callback = [=](const Contents::Content& content) {
                std::vector<Contents::Content> result;
                for (const auto& child : content.Split())
                    result.emplace_back(Contents::Content(typeLibrary, callback(child.As<TypeUtilities::FunctionTraits<CallbackT>::Argument0>())));
                return Contents::Content(result);
            };
        }
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const LambdaPropertyAspect& other) const override;
        std::string ToString() const override;
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) override;
        Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value) override;
        bool SupportAssign() const override;
        const Types::DecoratedTypeInfo& GetDecoratedTypeInfo() const;
        std::string GetName() const;
        virtual bool HasMember() const { return true; }
    private:
        Contents::Content GetNestedProperty(const Contents::Content& parent) const;
        Types::DecoratedTypeInfo m_decoratedTypeInfo;
        std::function<Contents::Content(const Contents::Content&)> m_callback;
        std::string m_name;
    };

}}}}