#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Services/Module.h"
#include "TypedLayoutElement.h"
#include "ILayoutElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Image : public TypedLayoutElement<Image, ILayoutElement>
    {
    public:
        Image(const Image&) = default;
        Image(Image&&) = default;
        Image& operator=(const Image&) = default;
        Image& operator=(Image&&) = default;

        Image(int imageId, Services::Module module = Services::Module())
            : m_imageId(imageId)
            , m_module(module)
        {}
        int GetImageId() const { return m_imageId; }
        Services::Module GetModule() const { return m_module; }
    private:
        int m_imageId;
        Services::Module m_module;
    };
}}}}
