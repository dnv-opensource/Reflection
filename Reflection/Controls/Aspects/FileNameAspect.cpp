//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FileNameAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    FileNameAspect::FileNameAspect(const std::shared_ptr<ControlNode>& fileName)
        : m_fileName(fileName)
    {

    }

    const Attributes::AttributeCollection& FileNameAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string FileNameAspect::ToString() const
    {
        return "FileNameAspect()";
    }

    std::shared_ptr<ControlNode> FileNameAspect::GetFileNameNode() const
    {
        return m_fileName;
    }

    bool FileNameAspect::operator==(const FileNameAspect& other) const
    {
        return true;
    }

}}}}

