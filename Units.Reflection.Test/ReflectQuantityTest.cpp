//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Reflection\Objects\Object.h"

#include "Reflection\TypeConversions\BuiltInConversions.h"

#include "Reflection\TypeLibraries\TypeLibraryFactory.h"

#include "Units\Reflection\ReflectQuantities.h"
#include "Units\Length.h"
#include "Units\Area.h"
#include "Units\Time.h"
#include "Units\Force.h"
#include "Units\Frequency.h"
#include "Units\Mass.h"
#include "Units\Angle.h"

using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Objects;
using namespace DNVS::MoFa::Units::Reflection;
using namespace DNVS::MoFa::Units;

class ReflectQuantityTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
        AddBuiltInConversions(m_typeLibrary->GetConversionGraph());
        ReflectQuantities(m_typeLibrary);
    }

    TypeLibraryPointer m_typeLibrary;
};

TEST_F(ReflectQuantityTest,ReflectAllOperators_MultiplyLengthLength)
{
    Object a=Object::Create<Length>(m_typeLibrary,Length(5.2));
    Object b=Object::Create<Length>(m_typeLibrary,Length(4.2));
    Object c=a*b;
    EXPECT_EQ(Area(5.2*4.2),c.As<Area>());
}

TEST_F(ReflectQuantityTest,SquareTime)
{
    Object a=Object::Create<Time>(m_typeLibrary,Time(5.2));
    Object b(m_typeLibrary,2);
    Object c=a^b;
    typedef Quantity<Dimension<0,2,0,0,0>> TimeSquared;
    EXPECT_EQ(Pow<2>(Time(5.2)),c.As<TimeSquared>());
}

TEST_F(ReflectQuantityTest,ScaleUnit)
{
    Object a(m_typeLibrary,2.4);
    Object b(m_typeLibrary,Length(5.2));
    Object c=a*b;
    EXPECT_EQ(Length(2.4*5.2),c.As<Length>());
}

TEST_F(ReflectQuantityTest,ComputeDensity)
{
    Object kipf=Object::Create<Force>(m_typeLibrary,_kipf);
    Object ft=Object::Create<Length>(m_typeLibrary,_ft);
    Object s=Object::Create<Time>(m_typeLibrary,_s);
    Object two(m_typeLibrary,2);
    Object three(m_typeLibrary,3);
    EXPECT_NO_THROW(Object result=kipf/ft*(s^two)/(ft^three));
}

TEST_F(ReflectQuantityTest,SubtractLengths)
{
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    EXPECT_NO_THROW(Object result=m-m);
}

TEST_F(ReflectQuantityTest,AreaDividedByLength)
{
    Object m2(m_typeLibrary,Pow<2>(_m));
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    EXPECT_NO_THROW(Object result=m2/m);
}

TEST_F(ReflectQuantityTest,VelocityTimesFrequency)
{
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    Object hz=Object::Create<Frequency>(m_typeLibrary,_Hz);
    EXPECT_NO_THROW(Object result=m*hz*hz);
}

TEST_F(ReflectQuantityTest,LengthTimesForcePerUnitArea)
{
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    Object fpa(m_typeLibrary,_N/_m2);
    EXPECT_NO_THROW(Object result=m*fpa);
}

TEST_F(ReflectQuantityTest,LengthTimesForcePerUnitLength)
{
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    Object fpl(m_typeLibrary,_N/_m);
    EXPECT_NO_THROW(Object result=m*fpl);
}

TEST_F(ReflectQuantityTest,LengthTimesForceTimeSquareByLengthSquare)
{
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    Object ftsbls(m_typeLibrary,_N/_m2*_s2);
    EXPECT_NO_THROW(Object result=m*ftsbls);
}

TEST_F(ReflectQuantityTest, CompareAngleWithLong)
{
    Object angleValue = Object::Create<Angle>(m_typeLibrary, 0_deg);
    Object longValue(m_typeLibrary, 0);
    EXPECT_TRUE(angleValue == longValue);
    EXPECT_TRUE(longValue == angleValue);
}

TEST_F(ReflectQuantityTest,CompareUnits)
{
    Object m=Object::Create<Length>(m_typeLibrary,_m);
    EXPECT_TRUE(m<=m);
    EXPECT_TRUE(m>=m);
    EXPECT_TRUE(m==m);
    EXPECT_FALSE((bool)(m>m));
    EXPECT_FALSE((bool)(m<m));
    EXPECT_FALSE((bool)(m!=m));
}
