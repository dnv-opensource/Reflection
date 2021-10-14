//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContentObjectAtIndex.h"
#include "Reflection/Utilities/DefaultArgumentExtracter.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    Objects::Object ContentObjectAtIndex::operator()(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, size_t index) const
    {
        std::shared_ptr<IContent> arg;
        if (m_content)
        {
            arg = m_content->TryGetArgument(member, index);
            if (arg)
            {
                auto object = arg->GetObject();
                if (object.IsValid())
                    return object;
            }
        }
        return Objects::Object(typeLibrary, Utilities::DefaultArgumentExtracter(typeLibrary, member, index).TryGetDefaultValue());
    }

}}}}