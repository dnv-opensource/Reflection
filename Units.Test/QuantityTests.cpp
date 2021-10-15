//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units\Volume.h"
#include "Units\Mass.h"
#include "Units\Angle.h"
#include "Units\Frequency.h"
#include "Units\Time.h"
#include "Units\Energy.h"
#include "Units\Velocity.h"
#include "Units\Area.h"
#include "Units\ForcePerUnitArea.h"
#include "Units\Acceleration.h"
#include "Units\Length.h"
#include "Units\Force.h"
#include "Units\StreamSupport.h"

#include "Test\Asserter.h"

using namespace DNVS::MoFa::Units;
TEST(QuantityTests, TestVolumeLiterIsCorrect)
{
    Volume v = 5._l;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.005, v.GetValue(), 1e-6);
}

TEST(QuantityTests, TestMultiplyLengthToVolume)
{
    Volume v = 1._m * 4.5_cm * Length(3.2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1 * 0.045 * 3.2, v.GetValue(), 1e-6);
}

TEST(QuantityTests, TestMassKiloGramIsCorrect)
{
    Mass m = 5._kg;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5, m.GetValue(), 1e-6);
}

TEST(QuantityTests, TestAngleTrigonometricFunctions)
{
    Angle a = 4._deg;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cos(4 * M_PI / 180), cos(a), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sin(4 * M_PI / 180), sin(a), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tan(4 * M_PI / 180), tan(a), 1e-6);
}

TEST(QuantityTests, AssertEqualForQuantities)
{
    MOFA_ASSERT_EQUAL(5._kg, 5000_g);
}

TEST(QuantityTests, AssignToQuantity)
{
    Volume v;
    v = 5.4_dl;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(Volume(5.4e-4).GetValue(), v.GetValue(), 1e-6);
}

TEST(QuantityTests, ScalarDividedByQuantity_TimeToHertz)
{
    MOFA_ASSERT_EQUAL(1._Hz, 1 / _s);
}

TEST(QuantityTests, CompareQuantities)
{
    Length l1(5.4);
    Length l2(8.4);
    CPPUNIT_ASSERT(l1<l2);
    CPPUNIT_ASSERT(l1 <= l2);
    CPPUNIT_ASSERT(l1 <= l1);
    CPPUNIT_ASSERT(l2>l1);
    CPPUNIT_ASSERT(l2 >= l1);
    CPPUNIT_ASSERT(l2 >= l2);
    CPPUNIT_ASSERT(l2 != l1);
    CPPUNIT_ASSERT(l2 == l2);
}

TEST(QuantityTests, DivideUnitByScalar)
{
    Energy e = _J / 5.6;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1 / 5.6, e.GetValue(), 1e-6);
}

TEST(QuantityTests, PlusAssignTest)
{
    Energy e = 5.6_J;
    e += 8.4_J;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6 + 8.4, e.GetValue(), 1e-6);
}

TEST(QuantityTests, MinusAssignTest)
{
    Energy e = 5.6_J;
    e -= 8.4_J;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6-8.4, e.GetValue(), 1e-6);
}

TEST(QuantityTests, MultiplyAssignTest)
{
    Energy e = 5.6_J;
    e *= 8.4;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6 * 8.4, e.GetValue(), 1e-6);
}

TEST(QuantityTests, DivideAssignTest)
{
    Energy e = 5.6_J;
    e /= 8.4;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6 / 8.4, e.GetValue(), 1e-6);
}

TEST(QuantityTests, SubtractQuantities)
{
    Velocity v(5);
    v = v-4_m / _s;
    MOFA_ASSERT_EQUAL(Velocity(1), v);
}

TEST(QuantityTests, AddQuantities)
{
    Velocity v(5);
    v = v + 4_m / _s;
    MOFA_ASSERT_EQUAL(Velocity(9), v);
}

TEST(QuantityTests, NegateQuantity)
{
    Velocity v(5);
    MOFA_ASSERT_EQUAL(Velocity(-5), -v);
}

TEST(QuantityTests, UnitlessQuantityImplicitConvertToDouble)
{
    Velocity v(6);
    auto nonUnit = v / Velocity(2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3, nonUnit, 1e-6);
}

TEST(QuantityTests, UnitlessQuantityAssign)
{
    Velocity v(6);
    auto nonUnit = v / Velocity(2);
    nonUnit = v / Velocity(3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2, nonUnit, 1e-6);
}

TEST(QuantityTests, UnitlessQuantityCopyConstruct)
{
    Velocity v(6);
    auto nonUnit = v / Velocity(2);
    auto nonUnit2(nonUnit);
    MOFA_ASSERT_EQUAL(nonUnit, nonUnit2);
}

TEST(QuantityTests, UnitlessQuantityDefaultConstructor)
{
    Quantity<NoDimension> a;
    MOFA_ASSERT_EQUAL(0.0, a);
}

TEST(QuantityTests, DefaultConstructArea)
{
    Area a;
    MOFA_ASSERT_EQUAL(Area(0.0), a);
}

TEST(QuantityTests, CopyConstructArea)
{
    Area a(3.4);
    Area b(a);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, AssignToArea)
{
    Area a;
    a = 3.4_m * _cm;
    MOFA_ASSERT_EQUAL(Area(0.034), a);
}

TEST(QuantityTests, StreamLength)
{
    Length a = 3.4_dm;
    std::stringstream ss;
    ss << a;
    MOFA_ASSERT_EQUAL("0.34", ss.str());
}

TEST(QuantityTests, DefaultConstructForcePerArea)
{
    ForcePerUnitArea a;
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(0.0), a);
}

TEST(QuantityTests, CopyConstructVolume)
{
    Volume a = 3_dl;
    Volume b(a);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructVelocity)
{
    Velocity a;
    MOFA_ASSERT_EQUAL(Velocity(0), a);
    a = 3_cm / _s;
    Velocity b(a);
    MOFA_ASSERT_EQUAL(Velocity(0.03), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructForcePerUnitArea)
{
    ForcePerUnitArea a;
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(0), a);
    a = 3 * _MPa;
    ForcePerUnitArea b(a);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(3.e6), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructAngle)
{
    Angle a;
    MOFA_ASSERT_EQUAL(Angle(0), a);
    a = 3_rad;
    Angle b(a);
    MOFA_ASSERT_EQUAL(Angle(3), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructTime)
{
    Time a;
    MOFA_ASSERT_EQUAL(Time(0), a);
    a = 3 * _hr;
    Time b(a);
    MOFA_ASSERT_EQUAL(Time(3 * 3600), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructMass)
{
    Mass a;
    MOFA_ASSERT_EQUAL(Mass(0), a);
    a = 3_g;
    Mass b(a);
    MOFA_ASSERT_EQUAL(Mass(3.e-3), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructLength)
{
    Length a;
    MOFA_ASSERT_EQUAL(Length(0), a);
    a = 3_mm;
    Length b(a);
    MOFA_ASSERT_EQUAL(Length(3.e-3), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructFrequency)
{
    Frequency a;
    MOFA_ASSERT_EQUAL(Frequency(0), a);
    a = 3 * _MHz;
    Frequency b(a);
    MOFA_ASSERT_EQUAL(Frequency(3.e6), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructForce)
{
    Force a;
    MOFA_ASSERT_EQUAL(Force(0), a);
    a = 3_MN;
    Force b(a);
    MOFA_ASSERT_EQUAL(Force(3.e6), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructEnergy)
{
    Energy a;
    MOFA_ASSERT_EQUAL(Energy(0), a);
    a = 3_MJ;
    Energy b(a);
    MOFA_ASSERT_EQUAL(Energy(3.e6), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, DefaultAndCopyConstructAcceleration)
{
    Acceleration a;
    MOFA_ASSERT_EQUAL(Acceleration(0), a);
    a = 3_cm / _s2;
    Acceleration b(a);
    MOFA_ASSERT_EQUAL(Acceleration(3.e-2), b);
    MOFA_ASSERT_EQUAL(a, b);
}

TEST(QuantityTests, TestAllAngleUnits)
{
    MOFA_ASSERT_EQUAL(Angle(1), _rad);
    MOFA_ASSERT_EQUAL(Angle(M_PI / 180.), _deg);
    MOFA_ASSERT_EQUAL(Angle(2 * M_PI), _rev);
}

TEST(QuantityTests, TestAllEnergyUnits)
{
    MOFA_ASSERT_EQUAL(Energy(1), _J);
    MOFA_ASSERT_EQUAL(Energy(1e3), _kJ);
    MOFA_ASSERT_EQUAL(Energy(1e6), _MJ);
    MOFA_ASSERT_EQUAL(Energy(1055.05585262), _Btu);
}

TEST(QuantityTests, TestAllForceUnits)
{
    MOFA_ASSERT_EQUAL(Force(1), _N);
    MOFA_ASSERT_EQUAL(Force(1e3), _kN);
    MOFA_ASSERT_EQUAL(Force(1e6), _MN);
    MOFA_ASSERT_EQUAL(Force(1e-3), _mN);
    MOFA_ASSERT_EQUAL(Force(4.44822161526), _lbf);
    MOFA_ASSERT_EQUAL(Force(4.44822161526e3), _kipf);
    MOFA_ASSERT_EQUAL(Force(9.80665), _kgf);
}

TEST(QuantityTests, TestAllForcePerUnitAreaUnits)
{
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(1), _Pa);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(6894.75729316759), _psi);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(1e3), _kPa);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(1e6), _MPa);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(6894.75729316759e3), _ksi);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(1e5), _bar);
    MOFA_ASSERT_EQUAL(ForcePerUnitArea(101325.0), _atm);
}

TEST(QuantityTests, TestAllFrequencyUnits)
{
    MOFA_ASSERT_EQUAL(Frequency(1), _Hz);
    MOFA_ASSERT_EQUAL(Frequency(1e6), _MHz);
    MOFA_ASSERT_EQUAL(Frequency(2 * M_PI), _radps);
}

TEST(QuantityTests, TestAllLengthUnits)
{
    MOFA_ASSERT_EQUAL(Length(1e3), _km);
    MOFA_ASSERT_EQUAL(Length(1), _m);
    MOFA_ASSERT_EQUAL(Length(1e-1), _dm);
    MOFA_ASSERT_EQUAL(Length(1e-2), _cm);
    MOFA_ASSERT_EQUAL(Length(1e-3), _mm);
    MOFA_ASSERT_EQUAL(Length(0.0254), _in);
    MOFA_ASSERT_EQUAL(Length(0.3048), _ft);
}

TEST(QuantityTests, TestAllMassUnits)
{
    MOFA_ASSERT_EQUAL(Mass(1), _kg);
    MOFA_ASSERT_EQUAL(Mass(1e-3), _g);
    MOFA_ASSERT_EQUAL(Mass(1e-1), _hg);
    MOFA_ASSERT_EQUAL(Mass(0.45359237), _lb);
    MOFA_ASSERT_EQUAL(Mass(1e3), _tonne);
    MOFA_ASSERT_EQUAL(Mass(1016.0469088), _ton);
    MOFA_ASSERT_EQUAL(Mass(453.59237), _kips);
    MOFA_ASSERT_EQUAL(Mass(14.5939035919985), _slug);
}

TEST(QuantityTests, TestAllTimeUnits)
{
    MOFA_ASSERT_EQUAL(Time(1), _s);
    MOFA_ASSERT_EQUAL(Time(1e-3), _ms);
    MOFA_ASSERT_EQUAL(Time(60), _min);
    MOFA_ASSERT_EQUAL(Time(3600), _hr);
}

TEST(QuantityTests, TestAllVelocityUnits)
{
    MOFA_ASSERT_EQUAL(Velocity(1852.0 / 3600.0), _Knot);
}

TEST(QuantityTests, TestAllVolumeUnits)
{
    MOFA_ASSERT_EQUAL(Volume(1e-3), _l);
    MOFA_ASSERT_EQUAL(Volume(1e-4), _dl);
    MOFA_ASSERT_EQUAL(Volume(1e-5), _cl);
    MOFA_ASSERT_EQUAL(Volume(1e-6), _ml);
}

TEST(QuantityTests, TestAllAccelerationUnits)
{
    MOFA_ASSERT_EQUAL(Acceleration(9.80665), _G);
}

TEST(QuantityTests, EnergyPhenomenonTests)
{
    Energy e(5);
    ASSERT_EQ(e, Quantity<EnergyDimension>(5));
}
