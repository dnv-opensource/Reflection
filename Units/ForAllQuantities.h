#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Acceleration.h"
#include "Angle.h"
#include "Area.h"
#include "AxialStiffness.h"
#include "BendingStiffness.h"
#include "CoupledDamping.h"
#include "CoupledMass.h"
#include "CoupledQuadraticDamping.h"
#include "CoupledQuadraticDampingForce.h"
#include "CoupledQuadraticDampingMoment.h"
#include "CoupledStiffness.h"
#include "Curvature.h"
#include "CurvatureResponseSpectrum.h"
#include "DampingPerUnitLength.h"
#include "DisplacementResponseSpectrum.h"
#include "Energy.h"
#include "EnergyDensity.h"
#include "FirstMomentOfArea.h"
#include "Force.h"
#include "ForceEnergyDensity.h"
#include "ForcePerUnitArea.h"
#include "ForcePerUnitAreaPerAngle.h"
#include "ForcePerUnitLength.h"
#include "ForcePerUnitVolume.h"
#include "ForceResponseSpectrum.h"
#include "Frequency.h"
#include "KinematicViscosity.h"
#include "Length.h"
#include "Mass.h"
#include "MassDensity.h"
#include "MassMomentOfInertia.h"
#include "MassPerUnitArea.h"
#include "MassPerUnitLength.h"
#include "MomentOfForce.h"
#include "MomentOfForceEnergyDensity.h"
#include "MomentResponseSpectrum.h"
#include "Pressure.h"
#include "QuadraticDampingPerUnitLength.h"
#include "RotationalAcceleration.h"
#include "RotationalDamping.h"
#include "RotationalFlexibility.h"
#include "RotationalQuadraticDamping.h"
#include "RotationalResponseSpectrum.h"
#include "RotationalStiffness.h"
#include "RotationalStiffnessPerUnitLength.h"
#include "RotationalVelocity.h"
#include "RotationPerLength.h"
#include "SecondMomentOfArea.h"
#include "SectionModulus.h"
#include "SpecificHeat.h"
#include "Stress.h"
#include "TempDiff.h"
#include "TempGradient.h"
#include "ThermalExpansionCoeff.h"
#include "Time.h"
#include "TorsionalStiffness.h"
#include "TranslationalDamping.h"
#include "TranslationalFlexibility.h"
#include "TranslationalQuadraticDamping.h"
#include "TranslationalStiffness.h"
#include "TranslationalStiffnessPerUnitLength.h"
#include "Velocity.h"
#include "VelocityResponseSpectrum.h"
#include "Volume.h"
#include "LambdaApplier.h"



namespace DNVS { namespace MoFa { namespace Units {

    template<typename Operation>
    void ForAllQuantities(Operation& op)
    {
        op.Apply<Acceleration>();
        op.Apply<Angle>();
        op.Apply<Area>();
        op.Apply<AxialStiffness>();
        op.Apply<BendingStiffness>();
        op.Apply<CoupledDamping>();
        op.Apply<CoupledMass>();
        op.Apply<CoupledQuadraticDamping>();
        op.Apply<CoupledQuadraticDampingForce>();
        op.Apply<CoupledQuadraticDampingMoment>();
        op.Apply<CoupledStiffness>();
        op.Apply<Curvature>();
        op.Apply<DampingPerUnitLength>();
        op.Apply<Energy>();
        op.Apply<EnergyDensity>();
        op.Apply<FirstMomentOfArea>();
        op.Apply<Force>();
        op.Apply<ForceEnergyDensity>();
        op.Apply<ForcePerUnitArea>();
        op.Apply<ForcePerUnitAreaPerAngle>();
        op.Apply<ForcePerUnitLength>();
        op.Apply<ForcePerUnitVolume>();
        op.Apply<Frequency>();
        op.Apply<KinematicViscosity>();
        op.Apply<Length>();
        op.Apply<Mass>();
        op.Apply<MassDensity>();
        op.Apply<MassMomentOfInertia>();
        op.Apply<MassPerUnitArea>();
        op.Apply<MassPerUnitLength>();
        op.Apply<MomentOfForce>();
        op.Apply<MomentOfForceEnergyDensity>();
        op.Apply<Pressure>();
        op.Apply<QuadraticDampingPerUnitLength>();
        op.Apply<RotationalDamping>();
        op.Apply<RotationalFlexibility>();
        op.Apply<RotationalQuadraticDamping>();
        op.Apply<RotationalStiffness>();
        op.Apply<SecondMomentOfArea>();
        op.Apply<SectionModulus>();
        op.Apply<SpecificHeat>();
        op.Apply<Stress>();
        op.Apply<TempDiff>();
        op.Apply<TempGradient>();
        op.Apply<ThermalExpansionCoeff>();
        op.Apply<Time>();
        op.Apply<TorsionalStiffness>();
        op.Apply<TranslationalDamping>();
        op.Apply<TranslationalFlexibility>();
        op.Apply<TranslationalQuadraticDamping>();
        op.Apply<TranslationalStiffness>();
        op.Apply<Velocity>();
        op.Apply<Volume>();

        op.Apply<CurvatureResponseSpectrum>();
        op.Apply<DisplacementResponseSpectrum>();
        op.Apply<ForceResponseSpectrum>();
        op.Apply<MomentResponseSpectrum>();
        op.Apply<RotationalAcceleration>();
        op.Apply<RotationalResponseSpectrum>();
        op.Apply<RotationalStiffnessPerUnitLength>();
        op.Apply<RotationalVelocity>();
        op.Apply<RotationPerLength>();
        op.Apply<TranslationalStiffnessPerUnitLength>();
        op.Apply<VelocityResponseSpectrum>();
    }
    template<typename LambdaT>
    void ForAllQuantitiesLambda(const LambdaT& op)
    {
        LambdaApplier<LambdaT> applier(op);
        ForAllQuantities(applier);
    }
}}}
