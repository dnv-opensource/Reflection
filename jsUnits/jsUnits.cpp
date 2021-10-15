//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <jsScript/jsValue.h>
#include <jsScript/jsUnit.h>
#include <jsScript/jsUnitValue.h>
#include <jsScript/jsClass.h>
#include <jsScript/jsQuantity.h>

#include <jsUnits/jsUnits.h>

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
#include <jsUnits/jsParameter.h>
#include <jsUnits/jsQuadraticDampingPerUnitLength.h>
#include <jsUnits/jsPressure.h>
#include <jsUnits/jsRotationalAcceleration.h>
#include <jsUnits/jsRotationalDamping.h>
#include <jsUnits/jsRotationalFlexibility.h>
#include <jsUnits/jsRotationalQuadraticDamping.h>
#include <jsUnits/jsRotationPerLength.h>
#include <jsUnits/jsRotationalStiffness.h>
#include <jsUnits/jsRotationalStiffnessPerUnitLength.h>
#include <jsUnits/jsRotationalVelocity.h>
#include <jsUnits/jsSectionModulus.h>
#include <jsUnits/jsSecondMomentOfArea.h>
#include <jsUnits/jsStress.h>
#include <jsUnits/jsForcePerUnitVolume.h>
#include <jsUnits/jsForcePerUnitAreaPerAngle.h>
//#include <jsUnits/jsTemperature.h>
#include <jsUnits/jsTemperatureDifference.h>
#include <jsUnits/jsTemperatureGradient.h>
#include <jsUnits/jsThermalExpansionCoeff.h>
#include <jsUnits/jsTorsionalStiffness.h>
#include <jsUnits/jsTranslationalDamping.h>
#include <jsUnits/jsTranslationalFlexibility.h>
#include <jsUnits/jsTranslationalQuadraticDamping.h>
#include <jsUnits/jsTranslationalStiffness.h>
#include <jsUnits/jsTranslationalStiffnessPerUnitLength.h>
#include <jsUnits/jsTime.h>
#include <jsUnits/jsVelocity.h>
#include <jsUnits/jsVolume.h>
#include <jsUnits/jsKinematicViscosity.h>
#include <jsUnits/jsVelocityResponseSpectrum.h>
#include <jsUnits/jsDisplacementResponseSpectrum.h>
#include <jsUnits/jsRotationalResponseSpectrum.h>
#include <jsUnits/jsCurvatureResponseSpectrum.h>

void jsUnits::init(jsTypeLibrary& typeLibrary)
{   
   jsNameSpace ns("Units");
   jsUnit::init(typeLibrary);
   jsUnitValue::init(typeLibrary);
   jsQuantity::init(typeLibrary);
   jsAcceleration::init(typeLibrary);
   jsAngle::init(typeLibrary);
   jsArea::init(typeLibrary);
   jsAxialStiffness::init(typeLibrary);
   jsBendingStiffness::init(typeLibrary);
   jsCoupledDamping::init(typeLibrary);
   jsCoupledMass::init(typeLibrary);
   jsCoupledQuadraticDamping::init(typeLibrary);
   jsCoupledQuadraticDampingForce::init(typeLibrary);
   jsCoupledQuadraticDampingMoment::init(typeLibrary);
   jsCoupledStiffness::init(typeLibrary);
   jsCurvature::init(typeLibrary);
   jsDampingPerUnitLength::init(typeLibrary);
   jsEnergy::init(typeLibrary);
   jsEnergyDensity::init(typeLibrary);
   jsFirstMomentOfArea::init(typeLibrary);
   jsForce::init(typeLibrary);
   jsForceEnergyDensity::init(typeLibrary);
   jsForceResponseSpectrum::init(typeLibrary);
   jsForcePerUnitArea::init(typeLibrary);
   jsForcePerUnitLength::init(typeLibrary);
   jsFrequency::init(typeLibrary);
   jsKinematicViscosity::init(typeLibrary);
   jsLength::init(typeLibrary);
   jsMass::init(typeLibrary);
   jsMassMomentOfInertia::init(typeLibrary);
   jsMassPerUnitArea::init(typeLibrary);
   jsMassPerUnitLength::init(typeLibrary);
   jsMassDensity::init(typeLibrary);
   jsMomentOfForce::init(typeLibrary);
   jsMomentOfForceEnergyDensity::init(typeLibrary);
   jsMomentResponseSpectrum::init(typeLibrary);
   jsParameter::init(typeLibrary);
   jsQuadraticDampingPerUnitLength::init(typeLibrary);
   jsPressure::init(typeLibrary);
   jsRotationalAcceleration::init(typeLibrary);
   jsRotationalDamping::init(typeLibrary);
   jsRotationalFlexibility::init(typeLibrary);
   jsRotationalQuadraticDamping::init(typeLibrary);
   jsRotationalStiffness::init(typeLibrary);
   jsRotationalStiffnessPerUnitLength::init(typeLibrary);
   jsRotationalVelocity::init(typeLibrary);
   jsRotationPerLength::init(typeLibrary);
   jsSecondMomentOfArea::init(typeLibrary);
   jsSectionModulus::init(typeLibrary);
   jsStress::init(typeLibrary);
//   jsTemperature::init(typeLibrary);
   jsTemperatureDifference::init(typeLibrary);
   jsTemperatureGradient::init(typeLibrary);
   jsThermalExpansionCoeff::init(typeLibrary);
   jsTorsionalStiffness::init(typeLibrary);
   jsTranslationalDamping::init(typeLibrary);
   jsTranslationalFlexibility::init(typeLibrary);
   jsTranslationalQuadraticDamping::init(typeLibrary);
   jsTranslationalStiffness::init(typeLibrary);
	jsTranslationalStiffnessPerUnitLength::init(typeLibrary);
   jsTime::init(typeLibrary);
   jsVelocity::init(typeLibrary);
   jsVolume::init(typeLibrary);
   jsForcePerUnitVolume::init(typeLibrary);
   jsForcePerUnitAreaPerAngle::init(typeLibrary);
   jsVelocityResponseSpectrum::init(typeLibrary);
   jsDisplacementResponseSpectrum::init(typeLibrary);
   jsRotationalResponseSpectrum::init(typeLibrary);
   jsCurvatureResponseSpectrum::init(typeLibrary);
}
