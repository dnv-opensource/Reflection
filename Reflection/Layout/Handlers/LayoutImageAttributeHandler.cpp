//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutImageAttributeHandler.h"
#include "Reflection/Attributes/ImageAttribute.h"
#include "Reflection/Layout/VerticalElements.h"
#include "Reflection/Layout/HorizontalElements.h"
#include "Reflection/Layout/Image.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    std::shared_ptr<ILayoutElement> LayoutImageAttributeHandler::ModifyLayout(std::shared_ptr<ILayoutElement> layoutElement, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (!layoutElement)
            return nullptr;
        if (auto imageAttribute = controlNode->TryGetAttribute<Attributes::ImageAttribute>())
        {
            Image image(imageAttribute->GetImageId(), imageAttribute->GetModule());
            switch (imageAttribute->GetImageSide())
            {
            case Attributes::ImageAttribute::Top:
                return std::make_shared<VerticalElements>(image, layoutElement);
            case Attributes::ImageAttribute::Bottom:
                return std::make_shared<VerticalElements>(layoutElement, image);
            case Attributes::ImageAttribute::Left:
                return std::make_shared<HorizontalElements>(image, layoutElement);
            case Attributes::ImageAttribute::Right:
                return std::make_shared<HorizontalElements>(layoutElement, image);
            }
        }
        return layoutElement;
    }

}}}}

