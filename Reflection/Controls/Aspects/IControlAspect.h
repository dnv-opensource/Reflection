#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Contents/Content.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {    
    class REFLECTION_IMPORT_EXPORT IControlAspect {
    public:
        virtual ~IControlAspect() {}
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const = 0;
        virtual bool operator==(const IControlAspect& other) const = 0;
        bool operator!=(const IControlAspect& other) const
        {
            return !operator==(other);
        }
        virtual std::string ToString() const = 0;
        virtual Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) { return Contents::Content(); }
        virtual Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value) { return Contents::Content(); }
        virtual bool SupportUpdateParent() const { return false; }
        virtual bool SupportAssign() const { return false; }
        virtual bool HasMember() const { return false; }
    };

    template<typename DerivedT>
    class IControlAspectT : public IControlAspect
    {
    public:
        DerivedT& GetDerived() { return *static_cast<DerivedT*>(this); }
        const DerivedT& GetDerived() const { return *static_cast<DerivedT const*>(this); }
        bool operator==(const IControlAspect& other) const override
        {
            if (const DerivedT* derived = dynamic_cast<const DerivedT*>(&other))
                return operator==(*derived);
            else
                return false;
        }
        virtual bool operator==(const DerivedT& other) const = 0;
    };

    template<class _E, class _Tr>
    inline std::basic_ostream<_E, _Tr>& __cdecl operator<<(std::basic_ostream<_E, _Tr>& _O, const IControlAspect& attribute) {
        return _O << attribute.ToString();
    }
}}}}