#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include "TypeUtilities/FunctionTraits.h"
#include "IAttribute.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Description:
    Some objects, like ggGuideCurve, has names. Use this attribute to get/set this name.
    */
    class NameAttribute : public IAttribute
    {
    public:
        template<typename Getter,typename Setter>
        NameAttribute(const Getter& getter, const Setter& setter, const std::string& caption = "Name")
            : m_caption(caption)
        {
            using GetterArgumentType = std::tuple_element_t<0, typename TypeUtilities::FunctionTraits<Getter>::Arguments>;
            m_getter = [getter](const Objects::Object& object) -> std::string
            {
                if (object.IsConvertibleTo<GetterArgumentType>())
                    return std::invoke(getter,object.As<GetterArgumentType>());
                else
                    return "";
            };
            using SetterArgumentType = std::tuple_element_t<0, typename TypeUtilities::FunctionTraits<Setter>::Arguments>;
            m_setter = [setter](const Objects::Object& object, const std::string& name)
            {
                if (object.IsConvertibleTo<SetterArgumentType>())
                    std::invoke(setter, object.As<SetterArgumentType>(), name);
            };
        }
        std::string GetName(const Objects::Object& object) const
        {
            return m_getter(object);
        }
        void SetName(const Objects::Object& object, const std::string& name) const
        {
            m_setter(object, name);
        }
        std::string GetCaption() const { return m_caption; }
    private:
        std::function<std::string(const Objects::Object&)> m_getter;
        std::function<void(const Objects::Object&, const std::string&)> m_setter;
        std::string m_caption;
    };

}}}}
