//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <gtest/gtest.h>
#include "Units/Acceleration.h"
#include "Units/Angle.h"
#include "Units/Area.h"
#include "Units/AxialStiffness.h"
#include "Units/BendingStiffness.h"
#include "Units/CoupledDamping.h"
#include "Units/CoupledMass.h"
#include "Units/CoupledQuadraticDamping.h"
#include "Units/CoupledQuadraticDampingForce.h"
#include "Units/CoupledQuadraticDampingMoment.h"
#include "Units/CoupledStiffness.h"
#include "Units/Curvature.h"
#include "Units/CurvatureResponseSpectrum.h"
#include "Units/DampingPerUnitLength.h"
#include "Units/DisplacementResponseSpectrum.h"
#include "Units/Energy.h"
#include "Units/EnergyDensity.h"
#include "Units/FirstMomentOfArea.h"
#include "Units/Force.h"
#include "Units/ForceEnergyDensity.h"
#include "Units/ForcePerUnitArea.h"
#include "Units/ForcePerUnitAreaPerAngle.h"
#include "Units/ForcePerUnitLength.h"
#include "Units/ForcePerUnitVolume.h"
#include "Units/ForceResponseSpectrum.h"
#include "Units/Frequency.h"
#include "Units/KinematicViscosity.h"
#include "Units/Length.h"
#include "Units/Mass.h"
#include "Units/MassDensity.h"
#include "Units/MassMomentOfInertia.h"
#include "Units/MassPerUnitArea.h"
#include "Units/MassPerUnitLength.h"
#include "Units/MomentOfForce.h"
#include "Units/MomentOfForceEnergyDensity.h"
#include "Units/MomentResponseSpectrum.h"
#include "Units/Pressure.h"
#include "Units/QuadraticDampingPerUnitLength.h"
#include "Units/RotationalAcceleration.h"
#include "Units/RotationalDamping.h"
#include "Units/RotationalFlexibility.h"
#include "Units/RotationalQuadraticDamping.h"
#include "Units/RotationalResponseSpectrum.h"
#include "Units/RotationalStiffness.h"
#include "Units/RotationalStiffnessPerUnitLength.h"
#include "Units/RotationalVelocity.h"
#include "Units/RotationPerLength.h"
#include "Units/SecondMomentOfArea.h"
#include "Units/SectionModulus.h"
#include "Units/SpecificHeat.h"
#include "Units/Stress.h"
#include "Units/TempDiff.h"
#include "Units/ThermalExpansionCoeff.h"
#include "Units/Time.h"
#include "Units/TorsionalStiffness.h"
#include "Units/TranslationalDamping.h"
#include "Units/TranslationalFlexibility.h"
#include "Units/TranslationalQuadraticDamping.h"
#include "Units/TranslationalStiffness.h"
#include "Units/TranslationalStiffnessPerUnitLength.h"
#include "Units/Velocity.h"
#include "Units/VelocityResponseSpectrum.h"
#include "Units/Volume.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Tests {
    template<typename PhenomenonT>
    class PhenomenonTest : public ::testing::Test
    {
    public:
    };
    struct QuantityComposer 
    {
        template<typename DimensionT>
        Quantity<DimensionT> operator()(DimensionT& dim) const {return Quantity<DimensionT>();}
    };
    TYPED_TEST_SUITE_P(PhenomenonTest);
    TYPED_TEST_P(PhenomenonTest,ComposeUnitSignature)
    {
        ASSERT_EQ(typeid(Quantity<typename TypeParam::DimensionType>), typeid(TypeParam::ComposeUnit<Quantity<typename TypeParam::DimensionType>>(QuantityComposer())));
    }
    TYPED_TEST_P(PhenomenonTest,PhenomenonNameMatchesType)
    {
        std::string phenomenonTypeName = typeid(TypeParam).name();
        phenomenonTypeName.replace(0,25,"");
        ASSERT_EQ(phenomenonTypeName, TypeParam::GetName()+"Phenomenon");
    }
    REGISTER_TYPED_TEST_SUITE_P(PhenomenonTest, ComposeUnitSignature, PhenomenonNameMatchesType);
    INSTANTIATE_TYPED_TEST_SUITE_P(Acceleration,PhenomenonTest,AccelerationPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Angle,PhenomenonTest,AnglePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Area,PhenomenonTest,AreaPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(AxialStiffness,PhenomenonTest,AxialStiffnessPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(BendingStiffness,PhenomenonTest,BendingStiffnessPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CoupledDamping,PhenomenonTest,CoupledDampingPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CoupledMass,PhenomenonTest,CoupledMassPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CoupledQuadraticDamping,PhenomenonTest,CoupledQuadraticDampingPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CoupledQuadraticDampingForce,PhenomenonTest,CoupledQuadraticDampingForcePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CoupledQuadraticDampingMoment,PhenomenonTest,CoupledQuadraticDampingMomentPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CoupledStiffness,PhenomenonTest,CoupledStiffnessPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Curvature,PhenomenonTest,CurvaturePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(CurvatureResponseSpectrum,PhenomenonTest,CurvatureResponseSpectrumPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(DampingPerUnitLength,PhenomenonTest,DampingPerUnitLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(DisplacementResponseSpectrum,PhenomenonTest,DisplacementResponseSpectrumPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Energy,PhenomenonTest,EnergyPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(EnergyDensity,PhenomenonTest,EnergyDensityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(FirstMomentOfArea,PhenomenonTest,FirstMomentOfAreaPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Force,PhenomenonTest,ForcePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ForceEnergyDensity,PhenomenonTest,ForceEnergyDensityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ForcePerUnitArea,PhenomenonTest,ForcePerUnitAreaPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ForcePerUnitAreaPerAngle,PhenomenonTest,ForcePerUnitAreaPerAnglePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ForcePerUnitLength,PhenomenonTest,ForcePerUnitLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ForcePerUnitVolume,PhenomenonTest,ForcePerUnitVolumePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ForceResponseSpectrum,PhenomenonTest,ForceResponseSpectrumPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Frequency,PhenomenonTest,FrequencyPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(KinematicViscosity,PhenomenonTest,KinematicViscosityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Length,PhenomenonTest,LengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Mass,PhenomenonTest,MassPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MassDensity,PhenomenonTest,MassDensityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MassMomentOfInertia,PhenomenonTest,MassMomentOfInertiaPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MassPerUnitArea,PhenomenonTest,MassPerUnitAreaPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MassPerUnitLength,PhenomenonTest,MassPerUnitLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MomentOfForce,PhenomenonTest,MomentOfForcePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MomentOfForceEnergyDensity,PhenomenonTest,MomentOfForceEnergyDensityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(MomentResponseSpectrum,PhenomenonTest,MomentResponseSpectrumPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Pressure,PhenomenonTest,PressurePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(QuadraticDampingPerUnitLength,PhenomenonTest,QuadraticDampingPerUnitLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalAcceleration,PhenomenonTest,RotationalAccelerationPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalDamping,PhenomenonTest,RotationalDampingPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalFlexibility,PhenomenonTest,RotationalFlexibilityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalQuadraticDamping,PhenomenonTest,RotationalQuadraticDampingPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalResponseSpectrum,PhenomenonTest,RotationalResponseSpectrumPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalStiffness,PhenomenonTest,RotationalStiffnessPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalStiffnessPerUnitLength,PhenomenonTest,RotationalStiffnessPerUnitLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationalVelocity,PhenomenonTest,RotationalVelocityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(RotationPerLength,PhenomenonTest,RotationPerLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(SecondMomentOfArea,PhenomenonTest,SecondMomentOfAreaPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(SectionModulus,PhenomenonTest,SectionModulusPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(SpecificHeat,PhenomenonTest,SpecificHeatPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Stress,PhenomenonTest,StressPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TempDiff,PhenomenonTest,TempDiffPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(ThermalExpansionCoeff,PhenomenonTest,ThermalExpansionCoeffPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Time,PhenomenonTest,TimePhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TorsionalStiffness,PhenomenonTest,TorsionalStiffnessPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TranslationalDamping,PhenomenonTest,TranslationalDampingPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TranslationalFlexibility,PhenomenonTest,TranslationalFlexibilityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TranslationalQuadraticDamping,PhenomenonTest,TranslationalQuadraticDampingPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TranslationalStiffness,PhenomenonTest,TranslationalStiffnessPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(TranslationalStiffnessPerUnitLength,PhenomenonTest,TranslationalStiffnessPerUnitLengthPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Velocity,PhenomenonTest,VelocityPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(VelocityResponseSpectrum,PhenomenonTest,VelocityResponseSpectrumPhenomenon);
    INSTANTIATE_TYPED_TEST_SUITE_P(Volume,PhenomenonTest,VolumePhenomenon);
}}}}