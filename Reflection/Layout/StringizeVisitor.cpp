//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "StringizeVisitor.h"
#include "ControlElement.h"
#include "GroupBox.h"
#include "CollectionOfElements.h"
#include "boost\algorithm\string\replace.hpp"
#include "Image.h"
#include "sstream"
#include "EmbeddedElement.h"
#include "Table.h"
#include "Symmetry.h"
#include "OverloadSelector.h"
#include "DynamicArea.h"
#include "Caption.h"
#include "Grid.h"
#include "GridElements.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    StringizeVisitor::StringizeVisitor(const ILayoutElement& element)
        : BaseType(element)
    {

    }

    std::string StringizeVisitor::VisitAndReturn(const ControlElement& element)
    {
        std::string result = StripType(typeid(element).name()) + "(\"" + element.GetName()+"\"";
        if (element.HasCaption())
            result += ", \"" + element.GetCaption() + "\"";
        result += ")";
        return result;
    }

    std::string StringizeVisitor::VisitAndReturn(const CollectionOfElements& element)
    {
        std::string result = StripType(typeid(element).name()) + "(";
        for (auto it = element.begin(); it != element.end(); ++it)
        {
            if(it!=element.begin())
                result += ", ";
            result += ToString(**it);
        }
        result += ")";
        return result;
    }

    std::string StringizeVisitor::VisitAndReturn(const GroupBox& element)
    {
        std::string groupName = "\"" + element.GetGroupName() + "\"";
        std::string groupOrEnabler;
        if (element.GetEnabler())
        { 
            groupOrEnabler = ToString(*element.GetEnabler());
            if (!element.GetGroupName().empty())
                groupOrEnabler = "std::make_pair(" + groupName + ", " + groupOrEnabler + ")";
        }
        else
            groupOrEnabler = groupName;
        std::string result = StripType(typeid(element).name()) + "(";
        result += groupOrEnabler;
        for (auto it = element.begin(); it != element.end(); ++it)
        {
            result += ", ";
            result += ToString(**it);
        }
        result += ")";
        return result;
    }

    std::string StringizeVisitor::VisitAndReturn(const Image& element)
    {
        std::stringstream stream;
        stream << StripType(typeid(element).name()) << "(" << element.GetImageId() << ")";
        return stream.str();
    }

    std::string StringizeVisitor::VisitAndReturn(const EmbeddedElement& element)
    {
        std::stringstream stream;
        stream << StripType(typeid(element).name()) << "(\"" << element.GetName() << "\",";
        if (element.GetChild())
            stream << ToString(*element.GetChild());
        else
            stream << "nullptr";
        stream << ")";
        return stream.str();
    }

    std::string StringizeVisitor::VisitAndReturn(const Table& element)
    {
        std::stringstream stream;
        stream << StripType(typeid(element).name()) << "(\"" << element.GetName() << "\", ";
        if (element.HasCaption())
            stream << "\"" << element.GetCaption() << "\", ";
        stream << "Header(";
        bool isFirst = true;
        for (const auto& column : element.GetHeader().GetColumns())
        {
            if (isFirst)
                isFirst = false;
            else
                stream << ", ";
            stream << "Column(\"" << column->GetHeader(nullptr, typeid(void)) << "\"";
            if (!column->GetDocumentation().empty())
                stream << ", \"" << column->GetDocumentation() << "\"";
        }
        stream << "), " << element.GetInitialNumberOfRows() << ")";
        return stream.str();
    }

    std::string StringizeVisitor::VisitAndReturn(const Symmetry& element)
    {
        return StripType(typeid(element).name()) + "(\"" + element.GetMasterPropertyName() + "\", \"" + element.GetSymmetryCaption() + "\", \"" + element.GetDocumentation() + "\", " + ToString(*element.GetChild()) + ")";
    }

    std::string StringizeVisitor::VisitAndReturn(const ElementWithOneChild& element)
    {
        std::stringstream stream;
        stream << StripType(typeid(element).name()) << "(";
        if (element.GetChild())
            stream << ToString(*element.GetChild());
        else
            stream << "nullptr";
        stream << ")";
        return stream.str();
    }

    std::string StringizeVisitor::VisitAndReturn(const OverloadSelector& element)
    {
        std::stringstream stream;
        stream << StripType(typeid(element).name()) << "(";
        stream << "\"" << element.GetName() << "\"";
        if (element.GetChild())
            stream << ", " << ToString(*element.GetChild());
        stream << ")";
        return stream.str();
    }

    std::string StringizeVisitor::VisitAndReturn(const DynamicArea& element)
    {
        return "DynamicArea()";
    }

    std::string StringizeVisitor::VisitAndReturn(const Caption& element)
    {
        return "Caption(\"" + element.GetCaption() + "\")";
    }

    std::string StringizeVisitor::VisitAndReturn(const Grid& element)
    {
        std::stringstream stream;
        stream << StripType(typeid(element).name()) << "(\"" << element.GetName() << "\", ";
        if (element.HasCaption())
            stream << "\"" << element.GetCaption() << "\", ";
        stream << ToString(element.GetHeader()) << ", ";
        if (element.GetFixedGrid())
            stream << ToString(*element.GetFixedGrid()) << ", ";
        stream << element.GetInitialNumberOfRows() << ")";
        return stream.str();
    }

    std::string StringizeVisitor::VisitAndReturn(const GridElements& elements)
    {
        std::string result = StripType(typeid(elements).name()) + "(" + std::to_string(elements.GetRows()) + ", " + std::to_string(elements.GetCols());
        for (auto it = elements.begin(); it != elements.end(); ++it)
        {
            result += ", " + ToString(**it);
        }
        result += ")";
        return result;
    }

    std::string StringizeVisitor::StripType(std::string type) const
    {
        boost::algorithm::replace_all(type, "class ", "");
        size_t pos = type.find_last_of("::");
        if (pos != std::string::npos)
            return type.substr(pos+1);
        return type;
    }

}}}}
