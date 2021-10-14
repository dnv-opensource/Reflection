#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/Config.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class AttributeCollection;
    /**
    The GroupAttribute is used in reflection in order to give the member a sensible name.
    Example:
    Class<BendingMomentMemOpt> cls(typeLibrary, "BendingMomentMemOpt");
    cls.SetGet("bendingMomentChoice", &BendingMomentMemOpt::SetBendingMomentChoice, &BendingMomentMemOpt::GetBendingMomentChoice)
    .AddAttribute<CaptionAttribute>("Bending Moment Option")

    When auto generating GUI for this, this is the text that will be displayed for the label in front of the control.
    If you do not specify this attribute, the name of the property will be used.
    If we hadn't specified the CaptionAttribute, the label would be "BendingMomentMemOpt".
    However, now the label will be "Bending Moment Option"
    */
    class CaptionAttribute : public IAttribute

    {
    public:
        CaptionAttribute(const std::string& caption) : m_caption(caption) {}
        const std::string& GetCaption() const { return m_caption; }
    private:
        std::string m_caption;
    };

    std::string REFLECTION_IMPORT_EXPORT GetCaptionOrDefault(const AttributeCollection& collection, const std::string& defaultCaption);
}}}}