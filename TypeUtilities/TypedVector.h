#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename T, int Dim>
    class TypedVector;

    template<typename T>
    class TypedVector<T, 1>
    {
    public:
        TypedVector(const TypedVector& other) = default;
        TypedVector(TypedVector&& other) = default;
        TypedVector& operator=(const TypedVector& other) = default;
        TypedVector& operator=(TypedVector&& other) = default;

        template<typename U>
        TypedVector(const TypedVector<U, 1>& other, std::enable_if_t<std::is_convertible_v<U, T>, void*> = nullptr)
            : m_x(other.GetX())
        {}
        template<typename U>
        std::enable_if_t<std::is_convertible_v<U, T>, TypedVector&> operator=(const TypedVector<U, 1>& other)
        {
            SetX(other.GetX());
            return *this;
        }

        TypedVector(const T& x)
            : m_x(x)
        {}

        const T& GetX() const { return m_x; }
        void SetX(const T& x) { m_x = x; }
        template<typename U>
        auto operator*(const U& factor) const {
            using ResultType = decltype(std::declval<T>() * std::declval<U>());
            return TypedVector<ResultType, 1>(GetX() * factor);
        }
        template<typename U>
        auto operator+(const TypedVector<U, 1>& other) const {
            using ResultType = decltype(std::declval<T>() + std::declval<U>());
            return TypedVector<ResultType, 1>(GetX() + other.GetX());
        }
        template<typename U>
        auto Dot(const TypedVector<U, 1>& rhs) const
        {
            return GetX() * rhs.GetX();
        }
        auto Length() const {
            return sqrt(Dot(*this));
        }
        
        auto Normalize() const {
            return *this / Length();
        }
    private:
        T m_x;
    };

    template<typename T>
    class TypedVector<T, 2> : public TypedVector<T, 1>
    {
    public:
        TypedVector(const TypedVector& other) = default;
        TypedVector(TypedVector&& other) = default;
        TypedVector& operator=(const TypedVector& other) = default;
        TypedVector& operator=(TypedVector&& other) = default;

        template<typename U>
        TypedVector(const TypedVector<U, 2>& other, std::enable_if_t<std::is_convertible_v<U, T>, void*> = nullptr)
            : TypedVector<T, 1>(other)
            , m_y(other.GetY())
        {}
        template<typename U>
        std::enable_if_t<std::is_convertible_v<U, T>, TypedVector&> operator=(const TypedVector<U, 2>& other)
        {
            TypedVector<T, 1>::operator=(other);
            SetY(other.GetY());
            return *this;
        }

        TypedVector(const T& x, const T& y)
            : TypedVector<T, 1>(x)
            , m_y(y)
        {}
        const T& GetY() const { return m_y; }
        void SetY(const T& y) { m_y = y; }
        template<typename U>
        auto operator*(const U& factor) const {
            using ResultType = decltype(std::declval<T>() * std::declval<U>());
            return TypedVector<ResultType, 2>(GetX() * factor, GetY()*factor);
        }
        template<typename U>
        auto operator+(const TypedVector<U, 2>& other) const {
            using ResultType = decltype(std::declval<T>() + std::declval<U>());
            return TypedVector<ResultType, 2>(GetX() + other.GetX(), GetY() + other.GetY(), GetZ());
        }
        template<typename U>
        auto Dot(const TypedVector<U, 2>& rhs) const
        {
            return GetX() * rhs.GetX() + GetY() * rhs.GetY();
        }
        auto Length() const {
            return sqrt(Dot(*this));
        }
        auto Normalize() const {
            return *this / Length();
        }
    private:
        T m_y;
    };

    template<typename T>
    class TypedVector<T, 3> : public TypedVector<T, 2>
    {
    public:
        TypedVector(const TypedVector& other) = default;
        TypedVector(TypedVector&& other) = default;
        TypedVector& operator=(const TypedVector& other) = default;
        TypedVector& operator=(TypedVector&& other) = default;

        template<typename U>
        TypedVector(const TypedVector<U, 3>& other, std::enable_if_t<std::is_convertible_v<U, T>, void*> = nullptr)
            : TypedVector<T, 2>(other)
            , m_z(other.GetZ())
        {}
        template<typename U>
        std::enable_if_t<std::is_convertible_v<U, T>, TypedVector&> operator=(const TypedVector<U, 3>& other)
        {
            TypedVector<T, 2>::operator=(other);
            SetZ(other.GetZ());
            return *this;
        }

        TypedVector(const T& x, const T& y, const T& z)
            : TypedVector<T, 2>(x, y)
            , m_z(z)
        {}
        const T& GetZ() const { return m_z; }
        void SetZ(const T& y) { m_x = y; }

        template<typename U>
        auto operator*(const U& factor) const {
            using ResultType = decltype(std::declval<T>() * std::declval<U>());
            return TypedVector<ResultType, 3>(GetX() * factor, GetY()*factor, GetZ() * factor);
        }
        template<typename U>
        auto operator+(const TypedVector<U, 3>& other) const {
            using ResultType = decltype(std::declval<T>() + std::declval<U>());
            return TypedVector<ResultType, 3>(GetX() + other.GetX(), GetY() + other.GetY(), GetZ() + other.GetZ());
        }
        template<typename U>
        auto Cross(const TypedVector<U, 3>& rhs) const
        {
            using ResultType = decltype(std::declval<T>() * std::declval<U>());
            return TypedVector<ResultType, 3>(
                GetY() * rhs.GetZ() - GetZ() * rhs.GetY(),
                GetZ() * rhs.GetX() - GetX() * rhs.GetZ(),
                GetX() * rhs.GetY() - GetY() * rhs.GetX()
                );
        }
        template<typename U>
        auto Dot(const TypedVector<U, 3>& rhs) const
        {
            return GetX() * rhs.GetX() + GetY() * rhs.GetY() + GetZ() * rhs.GetZ();
        }
        auto Length() const {
            return sqrt(Dot(*this));
        }
        auto Normalize() const {
            return *this / Length();
        }
    private:
        T m_z;
    };
    template<typename T, typename U, int Dim>
    auto operator*(const TypedVector<T, Dim>& lhs, const TypedVector<U, Dim>& rhs) {
        return lhs.Cross(rhs);
    }
    template<typename T, typename U, int Dim>
    auto operator*(const T& factor, const TypedVector<U,Dim>& rhs)
    {
        return rhs * factor;
    }
    template<typename T, int Dim, typename U>
    auto operator/(const TypedVector<T, Dim>& rhs, const U& factor)
    {
        return rhs * (1./factor);
    }
    template<typename T, int Dim>
    const TypedVector<T,Dim>& operator+(const TypedVector<T, Dim>& vec)
    {
        return vec;
    }
    template<typename T, int Dim>
    auto operator-(const TypedVector<T, Dim>& vec)
    {
        return vec * -1;
    }

    template<typename T, typename U, int Dim>
    auto operator-(const TypedVector<T, Dim>& lhs, const TypedVector<U, Dim>& rhs)
    {
        return lhs + (-rhs);
    }

}}}