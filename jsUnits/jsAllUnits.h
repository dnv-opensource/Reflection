//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef AFX_JSALLUNITS_H
#define AFX_JSALLUNITS_H

#include <jsUnits/jsAcceleration.h>
#include <jsUnits/jsAxialStiffness.h>
#include <jsUnits/jsAngle.h>
#include <jsUnits/jsArea.h>
#include <jsUnits/jsBendingStiffness.h>
#include <jsUnits/jsCoupledDamping.h>
#include <jsUnits/jsCoupledMass.h>
#include <jsUnits/jsCoupledQuadraticDamping.h>
#include <jsUnits/jsCoupledQuadraticDampingForce.h>
#include <jsUnits/jsCoupledQuadraticDampingMoment.h>
#include <jsUnits/jsCoupledStiffness.h>
#include <jsUnits/jsCurvature.h>
#include <jsUnits/jsDampingPerUnitLength.h>
#include <jsUnits/jsEnergy.h>
#include <jsUnits/jsEnergyDensity.h>
#include <jsUnits/jsFirstMomentOfArea.h>
#include <jsUnits/jsForce.h>
#include <jsUnits/jsForceEnergyDensity.h>
#include <jsUnits/jsForceResponseSpectrum.h>
#include <jsUnits/jsFrequency.h>
#include <jsUnits/jsForcePerUnitArea.h>
#include <jsUnits/jsForcePerUnitLength.h>
#include <jsUnits/jsLength.h>
#include <jsUnits/jsMass.h>
#include <jsUnits/jsMassDensity.h>
#include <jsUnits/jsMassMomentOfInertia.h>
#include <jsUnits/jsMassPerUnitArea.h>
#include <jsUnits/jsMassPerUnitLength.h>
#include <jsUnits/jsMomentOfForce.h>
#include <jsUnits/jsMomentOfForceEnergyDensity.h>
#include <jsUnits/jsMomentResponseSpectrum.h>
#include <jsUnits/jsQuadraticDampingPerUnitLength.h>
#include <jsUnits/jsPressure.h>
#include <jsUnits/jsRotationalAcceleration.h>
#include <jsUnits/jsRotationalDamping.h>
#include <jsUnits/jsRotationalFlexibility.h>
#include <jsUnits/jsRotationalQuadraticDamping.h>
#include <jsUnits/jsRotationalStiffness.h>
#include <jsUnits/jsRotationalVelocity.h>
#include <jsUnits/jsSectionModulus.h>
#include <jsUnits/jsSecondMomentOfArea.h>
#include <jsUnits/jsStress.h>
#include <jsUnits/jsForcePerUnitVolume.h>
#include <jsUnits/jsForcePerUnitAreaPerAngle.h>
#include <jsUnits/jsTemperatureDifference.h>
#include <jsUnits/jsTemperatureGradient.h>
#include <jsUnits/jsThermalExpansionCoeff.h>
#include <jsUnits/jsTorsionalStiffness.h>
#include <jsUnits/jsTranslationalDamping.h>
#include <jsUnits/jsTranslationalFlexibility.h>
#include <jsUnits/jsTranslationalQuadraticDamping.h>
#include <jsUnits/jsTranslationalStiffness.h>
#include <jsUnits/jsTime.h>
#include <jsUnits/jsVelocity.h>
#include <jsUnits/jsVolume.h>
#include <jsUnits/jsKinematicViscosity.h>
#include <jsUnits/jsSpecificHeat.h>
#include <jsUnits/jsVelocityResponseSpectrum.h>
#include <jsUnits/jsDisplacementResponseSpectrum.h>
#include <jsUnits/jsRotationalResponseSpectrum.h>
#include <jsUnits/jsCurvatureResponseSpectrum.h>

template<typename Functor>
bool match_all_units(Functor& f)
{
   if(f(jsType<jsLength>())) return true;
   if(f(jsType<jsAcceleration>())) return true;
   if(f(jsType<jsAngle>())) return true;
   if(f(jsType<jsArea>())) return true;
   if(f(jsType<jsAxialStiffness>())) return true;
   if(f(jsType<jsBendingStiffness>())) return true;
   if(f(jsType<jsCoupledDamping>())) return true;
   if(f(jsType<jsCoupledMass>())) return true;
   if(f(jsType<jsCoupledQuadraticDamping>())) return true;
   if(f(jsType<jsCoupledQuadraticDampingForce>())) return true;
   if(f(jsType<jsCoupledQuadraticDampingMoment>())) return true;
   if(f(jsType<jsCoupledStiffness>())) return true;
   if(f(jsType<jsCurvature>())) return true;
   if(f(jsType<jsDampingPerUnitLength>())) return true;
   if(f(jsType<jsEnergy>())) return true;
   if(f(jsType<jsEnergyDensity>())) return true;
   if(f(jsType<jsFirstMomentOfArea>())) return true;
   if(f(jsType<jsForce>())) return true;
   if(f(jsType<jsForceEnergyDensity>())) return true;
   if(f(jsType<jsForceResponseSpectrum>())) return true;
   if(f(jsType<jsForcePerUnitArea>())) return true;
   if(f(jsType<jsForcePerUnitLength>())) return true;
   if(f(jsType<jsFrequency>())) return true;
   if(f(jsType<jsKinematicViscosity>())) return true;
   if(f(jsType<jsLength>())) return true;
   if(f(jsType<jsMass>())) return true;
   if(f(jsType<jsMassMomentOfInertia>())) return true;
   if(f(jsType<jsMassPerUnitArea>())) return true;
   if(f(jsType<jsMassPerUnitLength>())) return true;
   if(f(jsType<jsMassDensity>())) return true;
   if(f(jsType<jsMomentOfForce>())) return true;
   if(f(jsType<jsMomentOfForceEnergyDensity>())) return true;
   if(f(jsType<jsMomentResponseSpectrum>())) return true;
   if(f(jsType<jsQuadraticDampingPerUnitLength>())) return true;
   if(f(jsType<jsPressure>())) return true;
   if(f(jsType<jsRotationalAcceleration>())) return true;
   if(f(jsType<jsRotationalDamping>())) return true;
   if(f(jsType<jsRotationalFlexibility>())) return true;
   if(f(jsType<jsRotationalQuadraticDamping>())) return true;
   if(f(jsType<jsRotationalStiffness>())) return true;
   if(f(jsType<jsRotationalVelocity>())) return true;
   if(f(jsType<jsSecondMomentOfArea>())) return true;
   if(f(jsType<jsSectionModulus>())) return true;
   if(f(jsType<jsStress>())) return true;
//   if(f(jsType<jsTemperature>())) return true;
   if(f(jsType<jsTemperatureDifference>())) return true;
   if(f(jsType<jsTemperatureGradient>())) return true;
   if(f(jsType<jsThermalExpansionCoeff>())) return true;
   if(f(jsType<jsTorsionalStiffness>())) return true;
   if(f(jsType<jsTranslationalDamping>())) return true;
   if(f(jsType<jsTranslationalFlexibility>())) return true;
   if(f(jsType<jsTranslationalQuadraticDamping>())) return true;
   if(f(jsType<jsTranslationalStiffness>())) return true;
   if(f(jsType<jsTime>())) return true;
   if(f(jsType<jsVelocity>())) return true;
   if(f(jsType<jsVolume>())) return true;
   if(f(jsType<jsForcePerUnitVolume>())) return true;
   if(f(jsType<jsForcePerUnitAreaPerAngle>())) return true;
   if(f(jsType<jsSpecificHeat>())) return true;
   if(f(jsType<jsVelocityResponseSpectrum>())) return true;
   if(f(jsType<jsDisplacementResponseSpectrum>())) return true;
   if(f(jsType<jsRotationalResponseSpectrum>())) return true;
   if(f(jsType<jsCurvatureResponseSpectrum>())) return true;
   return false;
}

#endif
