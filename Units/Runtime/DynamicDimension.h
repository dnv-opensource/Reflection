#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <ostream>
#include "..\Config.h"
#include "..\Dimension.h"
#include <string>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class DynamicDimension 
    {
    public:
        DynamicDimension(const DynamicDimension& other)
            :   m_length(other.m_length)
            ,   m_time(other.m_time)
            ,   m_force(other.m_force)
            ,   m_temperatureDifference(other.m_temperatureDifference)
            ,   m_angle(other.m_angle)
        {}
        template<typename StaticDimensionT>
        DynamicDimension(StaticDimensionT dim, typename std::enable_if<IsDimension<StaticDimensionT>::value, void*>::type = nullptr)
            :   m_length(StaticDimensionT::L)
            ,   m_time(StaticDimensionT::T)
            ,   m_force(StaticDimensionT::F)
            ,   m_temperatureDifference(StaticDimensionT::O)
            ,   m_angle(StaticDimensionT::A)
        {}
        DynamicDimension(int length, int time, int force, int temperatureDifference, int angle)
            :   m_length(length)
            ,   m_time(time)
            ,   m_force(force)
            ,   m_temperatureDifference(temperatureDifference)
            ,   m_angle(angle)
        {}
        bool operator==(const DynamicDimension& other) const
        {
            if(m_length != other.m_length) 
                return false;
            if(m_time != other.m_time) 
                return false;
            if(m_force != other.m_force) 
                return false;
            if(m_temperatureDifference != other.m_temperatureDifference) 
                return false;
            if(m_angle != other.m_angle) 
                return false;
            return true;
        }
        bool operator!=(const DynamicDimension& other) const
        {
            return !operator==(other);
        }
        bool operator<(const DynamicDimension& other) const
        {
            if(m_length!=other.m_length)
                return m_length<other.m_length;
            if(m_time!=other.m_time)
                return m_time<other.m_time;
            if(m_force!=other.m_force)
                return m_force<other.m_force;
            if(m_temperatureDifference!=other.m_temperatureDifference)
                return m_temperatureDifference<other.m_temperatureDifference;
            if(m_angle!=other.m_angle)
                return m_angle<other.m_angle;
            return false;
        }
        DynamicDimension operator+(const DynamicDimension& other) const
        {
            return DynamicDimension(
                m_length + other.m_length,
                m_time + other.m_time,
                m_force + other.m_force,
                m_temperatureDifference + other.m_temperatureDifference,
                m_angle + other.m_angle
            );
        }
        DynamicDimension operator-(const DynamicDimension& other) const
        {
            return DynamicDimension(
                m_length - other.m_length,
                m_time - other.m_time,
                m_force - other.m_force,
                m_temperatureDifference - other.m_temperatureDifference,
                m_angle - other.m_angle
                );
        }
        DynamicDimension operator*(int scale) const
        {
            return DynamicDimension(
                m_length * scale,
                m_time * scale,
                m_force * scale,
                m_temperatureDifference * scale,
                m_angle * scale
                );
        }
        DynamicDimension operator/(int scale) const
        {
            if (m_length%scale != 0)
                throw std::runtime_error("Length dimension " + std::to_string(m_length) + " must be a multiple of " + std::to_string(scale));
            if (m_time%scale != 0)
                throw std::runtime_error("Time dimension " + std::to_string(m_time) + " must be a multiple of " + std::to_string(scale));
            if (m_time%scale != 0)
                throw std::runtime_error("Force dimension " + std::to_string(m_force) + " must be a multiple of " + std::to_string(scale));
            if (m_time%scale != 0)
                throw std::runtime_error("TempDiff dimension " + std::to_string(m_temperatureDifference) + " must be a multiple of " + std::to_string(scale));
            if (m_time%scale != 0)
                throw std::runtime_error("Angle dimension " + std::to_string(m_angle) + " must be a multiple of " + std::to_string(scale));
            return DynamicDimension(
                m_length / scale,
                m_time / scale,
                m_force / scale,
                m_temperatureDifference / scale,
                m_angle / scale
            );
        }
        int GetLength() const {return m_length; }
        int GetTime() const {return m_time; }
        int GetForce() const {return m_force; }
        int GetTemperatureDifference() const {return m_temperatureDifference; }
        int GetAngle() const {return m_angle; }
    private:
        signed char m_length;
        signed char m_time;
        signed char m_force;
        signed char m_temperatureDifference;
        signed char m_angle;
    };
    template<class _Elem, class _Traits> inline
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& stream, const DynamicDimension& dim)
    {	
        return stream << "DynamicDimension(" 
                      << dim.GetLength() << ", " 
                      << dim.GetTime() << ", " 
                      << dim.GetForce() << ", " 
                      << dim.GetTemperatureDifference() << ", " 
                      << dim.GetAngle() << ")";
    }
}}}}
