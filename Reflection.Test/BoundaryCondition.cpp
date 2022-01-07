//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "BoundaryCondition.h"
#include "Reflection/Attributes/ExampleAttribute.h"
#include "Reflection/Attributes/CaptionAttribute.h"
#include "Reflection/Attributes/LayoutAttribute.h"
#include "Reflection/Attributes/DefaultAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {
    auto SliderAndTextBox(const std::string& caption)
    {
        using namespace DNVS::MoFa::Reflection::Attributes;
        return EmbeddedElement( //We want to represent an EnumOrValue. This class consists of an "option" enum plus a "value" representing the actual value, and we want to represent them as individual controls.
            HorizontalElements(//We organize the slider and the text box horizontally
                Slider("Option", Header(false), caption), //We create a slider to represent the option, and place the given caption before the actual slider. We explicitly prevent it having a header.
                TextBox("Value", "") //The text box represents the value part of the EnumOrValue. It has no caption.
            )
        );
    }

    auto SliderAndTextBoxWithHeader(const std::string& caption, const std::string& textBoxHeader)
    {
        using namespace DNVS::MoFa::Reflection::Attributes;
        return EmbeddedElement( //We want to represent an EnumOrValue. This class consists of an "option" enum plus a "value" representing the actual value, and we want to represent them as individual controls.
            HorizontalElements( //We organize the slider and the text box horizontally
                Slider("Option", caption), //We create a slider to represent the option, and place the given caption before the actual slider. By default, the slider will have a header representing all the options.
                VerticalElements(//We want a header for the text box also, and therefore we need a VerticalElements to represent this.
                    Caption(textBoxHeader), //This is the header for the text box
                    TextBox("Value", "") //The text box represents the value part of the EnumOrValue. It has no caption.
                )
            )
        );
    }

    void DoReflect(const std::shared_ptr<DNVS::MoFa::Reflection::TypeLibraries::ITypeLibrary>& typeLibrary, BoundaryCondition**)
    {
        using namespace DNVS::MoFa::Reflection::Classes;

        Reflect<BoundaryType>(typeLibrary);
        Reflect<TranslationalBoundary>(typeLibrary, "TranslationalBoundary");
        Reflect<RotationalBoundary>(typeLibrary, "RotationalBoundary");

        Class<BoundaryCondition> cls(typeLibrary, "BoundaryCondition");
        cls.AddDocumentation("Represent a set of Boundary Conditions");

        cls.Constructor<const TranslationalBoundary&, const TranslationalBoundary&, const TranslationalBoundary&,
            const RotationalBoundary&, const RotationalBoundary&, const RotationalBoundary&>()
            .AddSignature("Dx", "Dy", "Dz", "Rx", "Ry", "Rz")
            .AddDocumentation("Construct a new boundary condition")
            .AddAttribute<ExampleAttribute>("sp1.boundary = BoundaryCondition(Free,Fix,Dependent,Super,Super,Super);\n")
            .AddAttribute<CaptionAttribute>("Boundary condition");

        cls.SetGet("Dx", &BoundaryCondition::SetDx, &BoundaryCondition::GetDx)
            .AddAttribute<DefaultAttribute>(BoundaryType::Fixed)
            .AddAttribute<LayoutAttribute>(VerticalElements(Auto<bool>("TranslationalSymmetry","Let Dy and Dz equal Dx"),SliderAndTextBoxWithHeader("Dx", "Spring stiffness")));
        cls.SetGet("Dy", &BoundaryCondition::SetDy, &BoundaryCondition::GetDy)
            .AddAttribute<DefaultAttribute>(BoundaryType::Fixed)
            .AddAttribute<LayoutAttribute>(SliderAndTextBox("Dy"));
        cls.SetGet("Dz", &BoundaryCondition::SetDz, &BoundaryCondition::GetDz)
            .AddAttribute<DefaultAttribute>(BoundaryType::Fixed)
            .AddAttribute<LayoutAttribute>(SliderAndTextBox("Dz"));
        cls.SetGet("Rx", &BoundaryCondition::SetRx, &BoundaryCondition::GetRx)
            .AddAttribute<DefaultAttribute>(BoundaryType::Fixed)
            .AddAttribute<LayoutAttribute>(VerticalElements(Auto<bool>("RotationalSymmetry", "Let Ry and Rz equal Rx"), SliderAndTextBoxWithHeader("Rx", "Spring stiffness")));
        cls.SetGet("Ry", &BoundaryCondition::SetRy, &BoundaryCondition::GetRy)
            .AddAttribute<DefaultAttribute>(BoundaryType::Fixed)
            .AddAttribute<LayoutAttribute>(SliderAndTextBox("Ry"));
        cls.SetGet("Rz", &BoundaryCondition::SetRz, &BoundaryCondition::GetRz)
            .AddAttribute<DefaultAttribute>(BoundaryType::Fixed)
            .AddAttribute<LayoutAttribute>(SliderAndTextBox("Rz"));
    }

    BoundaryCondition::BoundaryCondition(const TranslationalBoundary& dx, const TranslationalBoundary& dy, const TranslationalBoundary& dz, const RotationalBoundary& rx, const RotationalBoundary& ry, const RotationalBoundary& rz)
    {
        SetDx(dx);
        SetDy(dy);
        SetDz(dz);
        SetRx(rx);
        SetRy(ry);
        SetRz(rz);
    }

    TranslationalBoundary BoundaryCondition::GetDx() const
    {
        return m_dx;
    }

    void BoundaryCondition::SetDx(const TranslationalBoundary& dx)
    {
        m_dx = dx;
    }

    TranslationalBoundary BoundaryCondition::GetDy() const
    {
        return m_dy;
    }

    void BoundaryCondition::SetDy(const TranslationalBoundary& dy)
    {
        m_dy = dy;
    }

    TranslationalBoundary BoundaryCondition::GetDz() const
    {
        return m_dz;
    }

    void BoundaryCondition::SetDz(const TranslationalBoundary& dz)
    {
        m_dz = dz;
    }

    RotationalBoundary BoundaryCondition::GetRx() const
    {
        return m_rx;
    }

    void BoundaryCondition::SetRx(const RotationalBoundary& rx)
    {
        m_rx = rx;
    }

    RotationalBoundary BoundaryCondition::GetRy() const
    {
        return m_ry;
    }

    void BoundaryCondition::SetRy(const RotationalBoundary& ry)
    {
        m_ry = ry;
    }

    RotationalBoundary BoundaryCondition::GetRz() const
    {
        return m_rz;
    }

    void BoundaryCondition::SetRz(const RotationalBoundary& rz)
    {
        m_rz = rz;
    }


}}}