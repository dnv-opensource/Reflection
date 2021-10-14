#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Objects/Object.h"
#include <map>
#include "TypeUtilities/CompareCaseLess.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //This aspect represents constant values stored on the node. This is used when parsing the RecordAttribute to store the row and column information for a given node.
    class REFLECTION_IMPORT_EXPORT IndexedPropertyAspect : public IControlAspectT<IndexedPropertyAspect>
    {
    public:
        using IndexMap = std::map<std::string, Objects::Object, TypeUtilities::CompareCaseLess>;
        IndexedPropertyAspect(const Members::MemberPointer& setFunction, const Members::MemberPointer& getFunction, const IndexMap& indices);
        ~IndexedPropertyAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const IndexedPropertyAspect& other) const override;
        std::string ToString() const override;
        const IndexMap& GetIndices() const;
        const Members::MemberPointer& GetGetFunction() const;
        const Members::MemberPointer& GetSetFunction() const;
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) override;
        Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value) override;
        bool SupportAssign() const override;
    private:
        Members::MemberPointer m_setFunction;
        Members::MemberPointer m_getFunction;
        IndexMap m_indices;
    };

}}}}