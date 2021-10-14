//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Vector3d.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Attributes\ShortSyntaxAttribute.h"
#include "Reflection\Attributes\ExpandMemberAttribute.h"

Vector3d::Vector3d(void)
:   m_x(0.0)
,   m_y(0.0)
,   m_z(0.0)
{
}

Vector3d::Vector3d(const double& x, const double& y, const double& z)
:   m_x(x)
,   m_y(y)
,   m_z(z)
{}

Vector3d::Vector3d(const Vector3d& other)
:   m_x(other.m_x)
,   m_y(other.m_y)
,   m_z(other.m_z)
{}


Vector3d::~Vector3d(void)
{}

Vector3d Vector3d::Cross(const Vector3d& other) const
{
    return Vector3d(Y()*other.Z()-other.Y()*Z(), Z()*other.X()-other.Z()*X(), X()*other.Y()-Y()-other.Z());
}

Vector3d Vector3d::operator*(const Vector3d& other) const
{
    return Cross(other);
}

double Vector3d::Dot(const Vector3d& other) const
{
    return X()*other.X() + Y()*other.Y() + Z()*other.Z();
}

double Vector3d::operator%(const Vector3d& other) const
{
    return Dot(other);
}

double Vector3d::Length() const
{
    return sqrt(Dot(*this));
}

double Vector3d::X() const
{
    return m_x;
}

void Vector3d::X(const double& x)
{
    m_x = x;
}

double Vector3d::Y() const
{
    return m_y;
}

void Vector3d::Y(const double& y)
{
    m_y = y;
}

double Vector3d::Z() const
{
    return m_z;
}

void Vector3d::Z(const double& z)
{
    m_z = z;
}

Vector3d Vector3d::operator-() const
{
    return Vector3d(-X(), -Y(), -Z());
}

const Vector3d& Vector3d::operator+() const
{
    return *this;
}

Vector3d Vector3d::operator+(const Vector3d& rhs) const
{
    return Vector3d(X() + rhs.X(), Y() + rhs.Y(), Z() + rhs.Z());
}

Vector3d Vector3d::operator-(const Vector3d& rhs) const
{
    return Vector3d(X()-rhs.X(), Y()-rhs.Y(), Z()-rhs.Z());
}

Vector3d Vector3d::operator*(const double& rhs) const
{
    return Vector3d(X()*rhs, Y()*rhs, Z()*rhs);
}

Vector3d operator*(const double& lhs, const Vector3d& rhs)
{
    return rhs*lhs;
}

Vector3d Vector3d::operator/(const double& rhs) const
{
    return Vector3d(X() / rhs, Y() / rhs, Z() / rhs);
}

bool Vector3d::operator==(const Vector3d& other) const
{
    if(m_x != other.m_x) 
        return false;
    if(m_y != other.m_y) 
        return false;
    if(m_z != other.m_z) 
        return false;
    return true;
}

bool Vector3d::operator!=(const Vector3d& other) const
{
    return !operator==(other);
}

void DoReflect(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer lib, Vector3d**)
{
    using namespace DNVS::MoFa::Reflection::Classes;
    Class<Vector3d> vector3d(lib, "Vector3d");
    vector3d.AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand);
    vector3d.Constructor();
    vector3d.Constructor<const double&, const double&, const double&>()
        .AddSignature("x", "y", "z")
        .AddAttribute<ShortSyntaxAttribute>(ShortSyntaxArgument::ScriptSyntax, ShortSyntaxArgument::ScriptSyntax, ShortSyntaxArgument::ScriptSyntax);
    vector3d.Constructor<const Vector3d&>();
    vector3d.Function("Cross", &Vector3d::Cross);
    vector3d.Operator(This.Const*This.Const);
    vector3d.Operator(This.Const % This.Const);
    vector3d.Function("Dot", &Vector3d::Dot)
            .AddSignature("other");
    vector3d.Get("Length", &Vector3d::Length);
    vector3d.Get("X", &Vector3d::X);
    vector3d.Set("X", &Vector3d::X);
    vector3d.Get("Y", &Vector3d::Y);
    vector3d.Set("Y", &Vector3d::Y);
    vector3d.Get("Z", &Vector3d::Z);
    vector3d.Set("Z", &Vector3d::Z);
    vector3d.Operator(-This.Const);
    vector3d.Operator( + This.Const);
    vector3d.Operator(This.Const + This.Const);
    vector3d.Operator(This.Const-This.Const);
    vector3d.Operator(This.Const*double());
    vector3d.Operator(double()*This.Const);
    vector3d.Operator(This.Const / double());
    vector3d.Operator(This.Const == This.Const);
    vector3d.Operator(This.Const != This.Const);
}
