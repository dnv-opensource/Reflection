#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Services/Module.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The ImageAttribute is used to add an image to a class, member or enum value.
    Example:
    Class<LinearVaryingCurveOffset,...> cls(typeLibrary, "LinearVaryingCurveOffset");
    cls.AddAttribute<ImageAttribute>(IDB_LinearVaryingCurveOffset, ImageAttribute::Top);

    The image will be displayed at the top of the dialog.
    */
    class ImageAttribute : public IAttribute {
    public:
        enum ImageSide {
            Left,
            Right,
            Top,
            Bottom,
            Documentation,
        };
        ImageAttribute(int imageId, ImageSide imageSide, const std::string& imageName = "", Services::Module module = Services::Module())
            : m_imageId(imageId)
            , m_imageSide(imageSide)
            , m_imageName(imageName)
            , m_module(module)
        {}
        int GetImageId() const { return m_imageId; }
        ImageSide GetImageSide() const { return m_imageSide; }
        const Services::Module& GetModule() const { return m_module; }
        const std::string& GetImageName() const { return m_imageName; }
    private:
        int m_imageId;
        ImageSide m_imageSide;
        Services::Module m_module;
        std::string m_imageName;
    };
}}}}