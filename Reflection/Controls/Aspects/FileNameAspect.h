#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class ControlNode;
    //If a function represents an export or import command, then we need to know which function argument represents the
    //file name. This aspect is stored on the root node and points to the node representing the function name.
    //This simplifies name lookup.
    class REFLECTION_IMPORT_EXPORT FileNameAspect : public IControlAspectT<FileNameAspect>
    {
    public:
        FileNameAspect(const std::shared_ptr<ControlNode>& fileName);
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const FileNameAspect& other) const override;
        std::string ToString() const override;
        std::shared_ptr<ControlNode> GetFileNameNode() const;
    private:
        std::shared_ptr<ControlNode> m_fileName;
    };

}}}}