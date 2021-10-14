#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <TypeUtilities/IntrusiveClass.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

#include "Reflection/Variants/Variant.h"
#include "Reflection/Config.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    class AttributeCollection;
}}}}

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    class REFLECTION_IMPORT_EXPORT ArgumentInfo : public TypeUtilities::IntrusiveClass<ArgumentInfo>
    {
    public:
        ArgumentInfo(const std::string& typeAlias, const std::string& name, const std::string& documentation);

        ArgumentInfo(const std::string& name,const std::string& documentation);
        ArgumentInfo(const std::string& name,const Types::DecoratedTypeInfo& type);
        ArgumentInfo(size_t index,const Types::DecoratedTypeInfo& type);
        ~ArgumentInfo();
        ArgumentInfo(const ArgumentInfo&) = delete;
        ArgumentInfo(ArgumentInfo&&) = delete;
        ArgumentInfo& operator=(const ArgumentInfo&) = delete;
        ArgumentInfo& operator=(ArgumentInfo&&) = delete;

        void SetName(const std::string& name);
        std::string GetName() const;

        Types::DecoratedTypeInfo GetTypeInfo() const;
        std::string TypeAlias() const;
        void SetTypeInfo(const Types::DecoratedTypeInfo& info);

        std::string GetDocumentation() const;

        bool HasDefault() const;
        Variants::Variant GetDefault() const;
        ///Sets the default value for this argument
        void SetDefault(const Variants::Variant& defaultValue);
        const Attributes::AttributeCollection& GetAttributeCollection() const;
        Attributes::AttributeCollection& GetAttributeCollection();
        bool HasAttributeCollection() const;
    private:
        Types::DecoratedTypeInfo m_type;
        std::string m_typeAlias;
        std::string m_name;
        std::unique_ptr<Attributes::AttributeCollection> m_attributeCollection;
    };
    typedef boost::intrusive_ptr<ArgumentInfo> ArgumentInfoPointer;
}}}}
