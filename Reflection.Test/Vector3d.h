#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\TypeLibraryPointer.h"

class Vector3d
{
public:
    Vector3d(void);
    Vector3d(const double& x, const double& y, const double& z);
    Vector3d(const Vector3d& other);
    ~Vector3d(void);

    Vector3d Cross(const Vector3d& other) const;
    Vector3d operator*(const Vector3d& other) const;

    double Dot(const Vector3d& other) const;
    double operator%(const Vector3d& other) const;

    double Length() const;

    double X() const;
    void X(const double& x);

    double Y() const;
    void Y(const double& y);

    double Z() const;
    void Z(const double& z);

    bool operator==(const Vector3d& other) const;
    bool operator!=(const Vector3d& other) const;

    Vector3d operator-() const;
    const Vector3d& operator+() const;
    Vector3d operator+(const Vector3d& rhs) const;
    Vector3d operator-(const Vector3d& rhs) const;
    Vector3d operator*(const double& rhs) const;
    friend Vector3d operator*(const double& lhs, const Vector3d& rhs);
    Vector3d operator/(const double& rhs) const;
private:
    double m_x, m_y, m_z;
};

void DoReflect(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, Vector3d**);