#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Units/TranslationalStiffness.h"
#include "Units/RotationalStiffness.h"
#include "Units/CoupledStiffness.h"
#include "Units/Runtime/DynamicQuantity.h"
#include "Matrix.h"
#include "EnumOrValue.h"

namespace DNVS {namespace MoFa {namespace Reflection {
    enum class BoundaryType
    {
        Undefined = -1,
        Free = 0,
        Fixed = 1,
        Prescribed = 2,
        Dependent = 3,
        Super = 4,
        Spring = 5
    };

    void DoReflect(const std::shared_ptr<TypeLibraries::ITypeLibrary>& typeLibrary, BoundaryType**);

    using namespace Units;
    using Units::TranslationalStiffness;
    using Units::RotationalStiffness;
    using Units::CoupledStiffness;
    using Units::Runtime::DynamicQuantity;
    using TranslationalBoundary = EnumOrValue<BoundaryType, BoundaryType::Spring, TranslationalStiffness>;
    using RotationalBoundary = EnumOrValue<BoundaryType, BoundaryType::Spring, RotationalStiffness>;

    class BoundaryStiffnessMatrix
    {
    public:
        BoundaryStiffnessMatrix(const TranslationalBoundary& dx, const TranslationalBoundary& dy, const TranslationalBoundary& dz,
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
        DynamicQuantity getStiffness(int freedomi, int freedomj);
        void setStiffness(int freedomi, int freedomj, const DynamicQuantity& spring);
    private:
        template<typename ResultType>
        ResultType GetDiagonal(size_t index, BoundaryType boundaryType) const;
        template<typename ResultType>
        void SetDiagonal(size_t index, BoundaryType& boundaryType, const ResultType& result);
        BoundaryType m_dx, m_dy, m_dz, m_rx, m_ry, m_rz;
        Space::Matrix m_matrix;
    };

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, BoundaryStiffnessMatrix**);
}}}
