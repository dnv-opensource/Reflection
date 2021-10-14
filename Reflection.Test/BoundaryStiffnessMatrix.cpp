//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "BoundaryStiffnessMatrix.h"
#include "Reflection/Enums/Enum.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/NameAttribute.h"
#include "Reflection/Attributes/RecordAttribute.h"
#include "Reflection/Attributes/IncludeInViewAttribute.h"
#include "Reflection/Attributes/DefaultAttribute.h"
#include "Reflection/Attributes/EnabledAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {

    Types::DecoratedTypeInfo GetMatrixUnit(size_t freedomi, size_t freedomj)
    {
        if (freedomj == 7)
            return Types::TypeId<BoundaryType>();
        if (freedomi < 1 || freedomi > 6)
            return Types::TypeId<DynamicQuantity>();
        if (freedomj < 1 || freedomj > 6)
            return Types::TypeId<DynamicQuantity>();

        if (freedomi <= 3 && freedomj <= 3)
            return Types::TypeId<TranslationalStiffness>();
        else if (freedomi >= 4 && freedomj >= 4)
            return Types::TypeId<RotationalStiffness>();
        else
            return Types::TypeId<CoupledStiffness>();
    }
    DynamicQuantity GetMatrixDefaultValue(size_t freedomi, size_t freedomj)
    {
        if (freedomi < 1 || freedomi > 6)
            throw std::runtime_error("Invalid index");
        if (freedomj < 1 || freedomj > 6)
            throw std::runtime_error("Invalid index");

        if (freedomi <= 3 && freedomj <= 3)
            return TranslationalStiffness(0);
        else if (freedomi >= 4 && freedomj >= 4)
            return RotationalStiffness(0);
        else
            return CoupledStiffness(0);
    }
    bool IsColumnOrRowEnabled(size_t freedom, BoundaryType dx, BoundaryType dy, BoundaryType dz, BoundaryType rx, BoundaryType ry, BoundaryType rz)
    {
        switch (freedom)
        {
        case 1:
            return dx == BoundaryType::Spring;
        case 2:
            return dy == BoundaryType::Spring;
        case 3:
            return dz == BoundaryType::Spring;
        case 4:
            return rx == BoundaryType::Spring;
        case 5:
            return ry == BoundaryType::Spring;
        case 6:
            return rz == BoundaryType::Spring;
        case 7:
            return true;
        default:
            return false;
        }
    }

    bool IsRecordEnabled(size_t freedomi, size_t freedomj, BoundaryType dx, BoundaryType dy, BoundaryType dz, BoundaryType rx, BoundaryType ry, BoundaryType rz)
    {
        if (freedomi > freedomj)
            return false;
        return IsColumnOrRowEnabled(freedomi, dx, dy, dz, rx, ry, rz) && IsColumnOrRowEnabled(freedomj, dx, dy, dz, rx, ry, rz);
    }


    void DoReflect(const std::shared_ptr<TypeLibraries::ITypeLibrary>& typeLibrary, BoundaryType**)
    {
        using namespace Enums;
        Enum<BoundaryType> cls(typeLibrary, "BoundaryType");
        cls.EnumValue("Free", BoundaryType::Free).AddDocumentation("Free boundary condition");
        cls.EnumValue("Fixed", BoundaryType::Fixed).AddDocumentation("Fixed boundary condition");
        cls.EnumValue("Prescribed", BoundaryType::Prescribed).AddDocumentation("Prescribed boundary condition");
        cls.EnumValue("Dependent", BoundaryType::Dependent).AddDocumentation("Dependent boundary condition");
        cls.EnumValue("Super", BoundaryType::Super).AddDocumentation("Super boundary condition");
        cls.InternalEnumValue("", BoundaryType::Spring).AddFormatter("Spring");
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, BoundaryStiffnessMatrix**)
    {
        using namespace Classes;
        Reflect<BoundaryType>(typeLibrary);
        Reflect<TranslationalBoundary>(typeLibrary, "TranslationalBoundary");
        Reflect<RotationalBoundary>(typeLibrary, "RotationalBoundary");

        Class<BoundaryStiffnessMatrix> cls(typeLibrary, "BoundaryStiffnessMatrix");
        cls.AddAttribute<NameAttribute>([](const BoundaryStiffnessMatrix*) {return "a"; }, [](BoundaryStiffnessMatrix*, const std::string&) {}, "Name");
        cls.Constructor<const TranslationalBoundary&, const TranslationalBoundary&, const TranslationalBoundary&, const RotationalBoundary&, const RotationalBoundary&, const RotationalBoundary&>()
            .AddSignature("dx", "dy", "dz", "rx", "ry", "rz")
            .AddAttribute<IncludeInViewAttribute>("SetStiffness")
            ;
        cls.SetGet("Dx", &BoundaryStiffnessMatrix::SetDx, &BoundaryStiffnessMatrix::GetDx).AddAttribute<DefaultAttribute>(0_N / _m);
        cls.SetGet("Dy", &BoundaryStiffnessMatrix::SetDy, &BoundaryStiffnessMatrix::GetDy).AddAttribute<DefaultAttribute>(0_N / _m);
        cls.SetGet("Dz", &BoundaryStiffnessMatrix::SetDz, &BoundaryStiffnessMatrix::GetDz).AddAttribute<DefaultAttribute>(0_N / _m);
        cls.SetGet("Rx", &BoundaryStiffnessMatrix::SetRx, &BoundaryStiffnessMatrix::GetRx).AddAttribute<DefaultAttribute>(0_N * _m);
        cls.SetGet("Ry", &BoundaryStiffnessMatrix::SetRy, &BoundaryStiffnessMatrix::GetRy).AddAttribute<DefaultAttribute>(0_N * _m);
        cls.SetGet("Rz", &BoundaryStiffnessMatrix::SetRz, &BoundaryStiffnessMatrix::GetRz).AddAttribute<DefaultAttribute>(0_N * _m);

        cls.Function("GetStiffness", &BoundaryStiffnessMatrix::getStiffness).AddSignature("freedomi", "freedomj");
        cls.Function("SetStiffness", &BoundaryStiffnessMatrix::setStiffness)
            .AddSignature("freedomi", "freedomj", "spring")
            .AddAttribute<RecordAttribute>(
                "getStiffness", //Define reflection method for extracting values from the matrix
                IndexRange(1, 6, "freedomI"), //Fixed size number of rows
                IndexRange(1, 7, "freedomJ", "1", "2", "3", "4", "5", "6", "boundary type"), //Fixed size number of columns 
                RecordTypeCallback(GetMatrixUnit, "freedomI", "freedomJ"), //Defines the types of each of the cells
                PropertyAlias("Dx.value", 1, 1),
                PropertyAlias("Dx.option", 1, 7),
                PropertyAlias("Dy.value", 2, 2),
                PropertyAlias("Dy.option", 2, 7),
                PropertyAlias("Dz.value", 3, 3),
                PropertyAlias("Dz.option", 3, 7),
                PropertyAlias("Rx.value", 4, 4),
                PropertyAlias("Rx.option", 4, 7),
                PropertyAlias("Ry.value", 5, 5),
                PropertyAlias("Ry.option", 5, 7),
                PropertyAlias("Rz.value", 6, 6),
                PropertyAlias("Rz.option", 6, 7),
                MatrixDiagonalSymmetric
                )
            .AddAttribute<DefaultAttribute>(GetMatrixDefaultValue, "freedomI", "freedomJ")
            .AddAttribute<EnabledAttribute>(IsRecordEnabled, "freedomI", "freedomJ", "Dx.option", "Dy.option", "Dz.option", "Rx.option", "Ry.option", "Rz.option");
        ;
    }

    BoundaryStiffnessMatrix::BoundaryStiffnessMatrix(const TranslationalBoundary& dx, const TranslationalBoundary& dy, const TranslationalBoundary& dz, const RotationalBoundary& rx, const RotationalBoundary& ry, const RotationalBoundary& rz) : m_matrix(6, 6)
    {
        SetDx(dx);
        SetDy(dy);
        SetDz(dz);
        SetRx(rx);
        SetRy(ry);
        SetRz(rz);
    }

    TranslationalBoundary BoundaryStiffnessMatrix::GetDx() const
    {
        return GetDiagonal<TranslationalBoundary>(0, m_dx);
    }

    void BoundaryStiffnessMatrix::SetDx(const TranslationalBoundary& dx)
    {
        SetDiagonal(0, m_dx, dx);
    }

    TranslationalBoundary BoundaryStiffnessMatrix::GetDy() const
    {
        return GetDiagonal<TranslationalBoundary>(1, m_dy);
    }

    void BoundaryStiffnessMatrix::SetDy(const TranslationalBoundary& dy)
    {
        SetDiagonal(1, m_dy, dy);
    }

    TranslationalBoundary BoundaryStiffnessMatrix::GetDz() const
    {
        return GetDiagonal<TranslationalBoundary>(2, m_dz);
    }

    void BoundaryStiffnessMatrix::SetDz(const TranslationalBoundary& dz)
    {
        SetDiagonal(2, m_dz, dz);
    }

    RotationalBoundary BoundaryStiffnessMatrix::GetRx() const
    {
        return GetDiagonal<RotationalBoundary>(3, m_rx);
    }

    void BoundaryStiffnessMatrix::SetRx(const RotationalBoundary& rx)
    {
        SetDiagonal(3, m_rx, rx);
    }

    RotationalBoundary BoundaryStiffnessMatrix::GetRy() const
    {
        return GetDiagonal<RotationalBoundary>(4, m_ry);
    }

    void BoundaryStiffnessMatrix::SetRy(const RotationalBoundary& ry)
    {
        SetDiagonal(4, m_ry, ry);
    }

    RotationalBoundary BoundaryStiffnessMatrix::GetRz() const
    {
        return GetDiagonal<RotationalBoundary>(5, m_rz);
    }

    void BoundaryStiffnessMatrix::SetRz(const RotationalBoundary& rz)
    {
        SetDiagonal(5, m_rz, rz);
    }

    DynamicQuantity BoundaryStiffnessMatrix::getStiffness(int freedomi, int freedomj)
    {
        if (freedomi < 1 || freedomi > 6) throw std::exception("freedomi must be between 1 and 6");
        if (freedomj < 1 || freedomj > 6) throw std::exception("freedomj must be between 1 and 6");

        if (freedomi <= 3 && freedomj <= 3)
            return TranslationalStiffness(m_matrix(freedomi - 1, freedomj - 1));
        else if (freedomi >= 4 && freedomj >= 4)
            return RotationalStiffness(m_matrix(freedomi - 1, freedomj - 1));
        else
            return CoupledStiffness(m_matrix(freedomi - 1, freedomj - 1));
    }

    void BoundaryStiffnessMatrix::setStiffness(int freedomi, int freedomj, const DynamicQuantity& spring)
    {
        if (freedomi < 1 || freedomi > 6) throw std::exception("freedomi must be between 1 and 6");
        if (freedomj < 1 || freedomj > 6) throw std::exception("freedomj must be between 1 and 6");

        double value;
        if (freedomi <= 3 && freedomj <= 3)
            value = TranslationalStiffness(spring).GetValue();
        else if (freedomi >= 4 && freedomj >= 4)
            value = RotationalStiffness(spring).GetValue();
        else
            value = CoupledStiffness(spring).GetValue();
        m_matrix(freedomi - 1, freedomj - 1) = value;
    }

    template<typename ResultType>
    ResultType BoundaryStiffnessMatrix::GetDiagonal(size_t index, BoundaryType boundaryType) const
    {
        if (boundaryType == BoundaryType::Spring)
            return ResultType::ValueType(m_matrix(index, index));
        else
            return boundaryType;
    }

    template<typename ResultType>
    void BoundaryStiffnessMatrix::SetDiagonal(size_t index, BoundaryType& boundaryType, const ResultType& result)
    {
        boundaryType = result.GetEnum();
        if (boundaryType == BoundaryType::Spring && result.IsValueValid())
            m_matrix(index, index) = result.GetValue()->GetValue();
    }

}}}


