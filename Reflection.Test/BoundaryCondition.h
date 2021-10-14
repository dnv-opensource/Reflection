#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "BoundaryStiffnessMatrix.h"

namespace DNVS {namespace MoFa {namespace Reflection {
    class BoundaryCondition {
    public:

        BoundaryCondition(const TranslationalBoundary& dx, const TranslationalBoundary& dy, const TranslationalBoundary& dz,
            const RotationalBoundary& rx, const RotationalBoundary& ry, const RotationalBoundary& rz);

        TranslationalBoundary GetDx() const;
        void SetDx(const TranslationalBoundary& dx);
        TranslationalBoundary GetDy() const;
        void SetDy(const TranslationalBoundary& dy);
        TranslationalBoundary GetDz() const;
        void SetDz(const TranslationalBoundary& dz);
        RotationalBoundary GetRx() const;
        void SetRx(const RotationalBoundary& rx);
        RotationalBoundary GetRy() const;
        void SetRy(const RotationalBoundary& ry);
        RotationalBoundary GetRz() const;
        void SetRz(const RotationalBoundary& rz);
    protected:
        TranslationalBoundary m_dx, m_dy, m_dz;
        RotationalBoundary m_rx, m_ry, m_rz;
    };
    void DoReflect(const std::shared_ptr<DNVS::MoFa::Reflection::TypeLibraries::ITypeLibrary>& typeLibrary, BoundaryCondition**);
}}}