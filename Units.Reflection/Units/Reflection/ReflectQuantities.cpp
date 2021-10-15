//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units\Reflection\ReflectQuantities.h"

#include "Units\Quantity.h"
#include "Units\Length.h"
#include "Units\Time.h"
#include "Units\Force.h"
#include "Units\Angle.h"
#include "Units\Mass.h"
#include "Units\Area.h"
#include "Units\Volume.h"
#include "Units\Curvature.h"
#include "Units\Energy.h"
#include "Units\TempDiff.h"
#include "Units\Acceleration.h"
#include "Units\ForcePerUnitArea.h"
#include "Units\ForcePerUnitLength.h"
#include "Units\Velocity.h"
#include "Units\MassDensity.h"
#include "Units\Runtime\DynamicQuantity.h"
#include "Units\ForAllDimensions.h"

#include "Reflection\Classes\Class.h"
#include "Reflection\TypeConversions\UserConversion.h"

#include "Units\Reflection\Details\ReflectAcceleration.h"
#include "Units\Reflection\Details\ReflectAngle.h"
#include "Units\Reflection\Details\ReflectArea.h"
#include "Units\Reflection\Details\ReflectAxialStiffness.h"
#include "Units\Reflection\Details\ReflectBendingStiffness.h"
#include "Units\Reflection\Details\ReflectCoupledDamping.h"
#include "Units\Reflection\Details\ReflectCoupledMass.h"
#include "Units\Reflection\Details\ReflectCoupledQuadraticDamping.h"
#include "Units\Reflection\Details\ReflectCoupledQuadraticDampingForce.h"
#include "Units\Reflection\Details\ReflectCoupledQuadraticDampingMoment.h"
#include "Units\Reflection\Details\ReflectCoupledStiffness.h"
#include "Units\Reflection\Details\ReflectCurvature.h"
#include "Units\Reflection\Details\ReflectCurvatureResponseSpectrum.h"
#include "Units\Reflection\Details\ReflectDampingPerUnitLength.h"
#include "Units\Reflection\Details\ReflectDisplacementResponseSpectrum.h"
#include "Units\Reflection\Details\ReflectEnergy.h"
#include "Units\Reflection\Details\ReflectEnergyDensity.h"
#include "Units\Reflection\Details\ReflectFirstMomentOfArea.h"
#include "Units\Reflection\Details\ReflectForce.h"
#include "Units\Reflection\Details\ReflectForceEnergyDensity.h"
#include "Units\Reflection\Details\ReflectForcePerUnitArea.h"
#include "Units\Reflection\Details\ReflectForcePerUnitAreaPerAngle.h"
#include "Units\Reflection\Details\ReflectForcePerUnitLength.h"
#include "Units\Reflection\Details\ReflectForcePerUnitVolume.h"
#include "Units\Reflection\Details\ReflectForceResponseSpectrum.h"
#include "Units\Reflection\Details\ReflectFrequency.h"
#include "Units\Reflection\Details\ReflectKinematicViscosity.h"
#include "Units\Reflection\Details\ReflectLength.h"
#include "Units\Reflection\Details\ReflectMass.h"
#include "Units\Reflection\Details\ReflectMassDensity.h"
#include "Units\Reflection\Details\ReflectMassMomentOfInertia.h"
#include "Units\Reflection\Details\ReflectMassPerUnitArea.h"
#include "Units\Reflection\Details\ReflectMassPerUnitLength.h"
#include "Units\Reflection\Details\ReflectMomentOfForce.h"
#include "Units\Reflection\Details\ReflectMomentOfForceEnergyDensity.h"
#include "Units\Reflection\Details\ReflectMomentResponseSpectrum.h"
#include "Units\Reflection\Details\ReflectPressure.h"
#include "Units\Reflection\Details\ReflectQuadraticDampingPerUnitLength.h"
#include "Units\Reflection\Details\ReflectRotationalAcceleration.h"
#include "Units\Reflection\Details\ReflectRotationalDamping.h"
#include "Units\Reflection\Details\ReflectRotationalFlexibility.h"
#include "Units\Reflection\Details\ReflectRotationalQuadraticDamping.h"
#include "Units\Reflection\Details\ReflectRotationalResponseSpectrum.h"
#include "Units\Reflection\Details\ReflectRotationalStiffness.h"
#include "Units\Reflection\Details\ReflectRotationalStiffnessPerUnitLength.h"
#include "Units\Reflection\Details\ReflectRotationalVelocity.h"
#include "Units\Reflection\Details\ReflectRotationPerLength.h"
#include "Units\Reflection\Details\ReflectSecondMomentOfArea.h"
#include "Units\Reflection\Details\ReflectSectionModulus.h"
#include "Units\Reflection\Details\ReflectSpecificHeat.h"
#include "Units\Reflection\Details\ReflectStress.h"
#include "Units\Reflection\Details\ReflectTempDiff.h"
#include "Units\Reflection\Details\ReflectTempGradient.h"
#include "Units\Reflection\Details\ReflectThermalExpansionCoeff.h"
#include "Units\Reflection\Details\ReflectTime.h"
#include "Units\Reflection\Details\ReflectTorsionalStiffness.h"
#include "Units\Reflection\Details\ReflectTranslationalDamping.h"
#include "Units\Reflection\Details\ReflectTranslationalFlexibility.h"
#include "Units\Reflection\Details\ReflectTranslationalQuadraticDamping.h"
#include "Units\Reflection\Details\ReflectTranslationalStiffness.h"
#include "Units\Reflection\Details\ReflectTranslationalStiffnessPerUnitLength.h"
#include "Units\Reflection\Details\ReflectVelocity.h"
#include "Units\Reflection\Details\ReflectVelocityResponseSpectrum.h"
#include "Units\Reflection\Details\ReflectVolume.h"
#include "Units\Reflection\ReflectCommon.h"
#include "Units\Reflection\ReflectOperators.h"
#include "Units\Runtime\UnitParser.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "Reflection\Attributes\UndefinedAttribute.h"



using namespace DNVS::MoFa::Reflection::Variants;
using namespace DNVS::MoFa::Reflection::Operators;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Types;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Classes;

namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    void ReflectQuantities(TypeLibraryPointer typeLibrary)
    {           
        ReflectAcceleration(typeLibrary);
        ReflectAngle(typeLibrary);
        ReflectArea(typeLibrary);
        ReflectAxialStiffness(typeLibrary);
        ReflectBendingStiffness(typeLibrary);
        ReflectCoupledDamping(typeLibrary);
        ReflectCoupledMass(typeLibrary);
        ReflectCoupledQuadraticDamping(typeLibrary);
        ReflectCoupledQuadraticDampingForce(typeLibrary);
        ReflectCoupledQuadraticDampingMoment(typeLibrary);
        ReflectCoupledStiffness(typeLibrary);
        ReflectCurvature(typeLibrary);
        ReflectCurvatureResponseSpectrum(typeLibrary);
        ReflectDampingPerUnitLength(typeLibrary);
        ReflectDisplacementResponseSpectrum(typeLibrary);
        ReflectEnergy(typeLibrary);
        ReflectEnergyDensity(typeLibrary);
        ReflectFirstMomentOfArea(typeLibrary);
        ReflectForce(typeLibrary);
        ReflectForceEnergyDensity(typeLibrary);
        ReflectForcePerUnitArea(typeLibrary);
        ReflectForcePerUnitAreaPerAngle(typeLibrary);
        ReflectForcePerUnitLength(typeLibrary);
        ReflectForcePerUnitVolume(typeLibrary);
        ReflectForceResponseSpectrum(typeLibrary);
        ReflectFrequency(typeLibrary);
        ReflectKinematicViscosity(typeLibrary);
        ReflectLength(typeLibrary);
        ReflectMass(typeLibrary);
        ReflectMassDensity(typeLibrary);
        ReflectMassMomentOfInertia(typeLibrary);
        ReflectMassPerUnitArea(typeLibrary);
        ReflectMassPerUnitLength(typeLibrary);
        ReflectMomentOfForce(typeLibrary);
        ReflectMomentOfForceEnergyDensity(typeLibrary);
        ReflectMomentResponseSpectrum(typeLibrary);
        ReflectPressure(typeLibrary);
        ReflectQuadraticDampingPerUnitLength(typeLibrary);
        ReflectRotationalAcceleration(typeLibrary);
        ReflectRotationalDamping(typeLibrary);
        ReflectRotationalFlexibility(typeLibrary);
        ReflectRotationalQuadraticDamping(typeLibrary);
        ReflectRotationalResponseSpectrum(typeLibrary);
        ReflectRotationalStiffness(typeLibrary);
        ReflectRotationalStiffnessPerUnitLength(typeLibrary);
        ReflectRotationalVelocity(typeLibrary);
        ReflectRotationPerLength(typeLibrary);
        ReflectSecondMomentOfArea(typeLibrary);
        ReflectSectionModulus(typeLibrary);
        ReflectSpecificHeat(typeLibrary);
        ReflectStress(typeLibrary);
        ReflectTempDiff(typeLibrary);
        ReflectTempGradient(typeLibrary);
        ReflectThermalExpansionCoeff(typeLibrary);
        ReflectTime(typeLibrary);
        ReflectTorsionalStiffness(typeLibrary);
        ReflectTranslationalDamping(typeLibrary);
        ReflectTranslationalFlexibility(typeLibrary);
        ReflectTranslationalQuadraticDamping(typeLibrary);
        ReflectTranslationalStiffness(typeLibrary);
        ReflectTranslationalStiffnessPerUnitLength(typeLibrary);
        ReflectVelocity(typeLibrary);
        ReflectVelocityResponseSpectrum(typeLibrary);
        ReflectVolume(typeLibrary);

        ReflectCommon(typeLibrary);

        ReflectAllOperators<Quantity<Dimension<0, 0, 0, 0, 2> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<-2, 0, 0, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<2, -2, 0, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<2, 0, 1, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<0, 0, 2, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<2, 0, 2, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<0, 1, 1, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<-1, 0, 1, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<0, 2, 1, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<0, 0, 0, 0, 0> > >(typeLibrary);
        ReflectAllOperators<Quantity<Dimension<-3, 0, 1, 0, 0> > >(typeLibrary);

        AddUserConversion<Quantity<NoDimension>, double>(typeLibrary->GetConversionGraph(), ConversionType::StandardConversion, &Quantity<NoDimension>::operator double);
        AddConstructorConversion<double, Quantity<NoDimension>>(typeLibrary->GetConversionGraph(), ConversionType::StandardConversion);
    }

    struct ConverterFromDynamicQuantityToStaticQuantity
    {
    public:
        ConverterFromDynamicQuantityToStaticQuantity(const Variant& dynamicQuantity)
            :   m_dynamicQuantity(InternalVariantService::UnreflectUnchecked<Runtime::DynamicQuantity>(dynamicQuantity))
            ,   m_staticQuantity(dynamicQuantity)
        {
        }
        template<typename DimensionT>
        struct StaticQuantityComputer 
        {
            typedef Quantity<DimensionT> Type;
        };
        template<>
        struct StaticQuantityComputer<Dimension<0, 0, 0, 0, 0>>
        {
            typedef double Type;
        };
        template<typename DimensionT>
        void Apply()
        {
            typedef typename StaticQuantityComputer<DimensionT>::Type StaticQuantity;
            if(Runtime::DynamicDimension(DimensionT()) == m_dynamicQuantity.GetSimplifiedUnit().GetDimension())   
                m_staticQuantity = VariantService::ReflectType<StaticQuantity>(StaticQuantity(m_dynamicQuantity.GetNeutralValue()));
        }
        Variant GetStaticQuantity() const
        {
            return m_staticQuantity;
        }
    private:
        Variant m_staticQuantity;
        Runtime::DynamicQuantity m_dynamicQuantity;
    };

    struct FallbackUnitConverter : public IConversion
    {
        virtual Variant Convert(const Variant& other)
        {
            ConverterFromDynamicQuantityToStaticQuantity converter(other);
            ForAllUsedDimensions(converter);
            converter.Apply<Dimension<0, 0, 0, 0, 0>>();
            return converter.GetStaticQuantity();
        }
        virtual void IntrusiveConvert( Variant& variable ) 
        {
            ConverterFromDynamicQuantityToStaticQuantity converter(variable);
            ForAllUsedDimensions(converter);
            converter.Apply<Dimension<0, 0, 0, 0, 0>>();
            variable = converter.GetStaticQuantity();
        }
    };
    Runtime::DynamicQuantity DivideDynamicQuantities(const Runtime::DynamicQuantity& lhs, const Runtime::DynamicQuantity& rhs)
    {
        Runtime::DynamicQuantity result = lhs / rhs;
        result.TrySimplifyUnit();
        return result;
    }
    Runtime::DynamicQuantity MultiplyDynamicQuantities(const Runtime::DynamicQuantity& lhs, const Runtime::DynamicQuantity& rhs)
    {
        Runtime::DynamicQuantity result = lhs * rhs;
        result.TrySimplifyUnit();
        return result;
    }
    class StringToQuantityConversion : public IConversion
    {
    public:
        // creates copy of variable, converts it and returns converted copy
        virtual Variant Convert(const Variant& variable)
        {
            const std::string& quantity = InternalVariantService::UnreflectUnchecked<const std::string&>(variable);
            using boost::spirit::qi::double_;
            using boost::spirit::qi::char_;
            using boost::spirit::qi::phrase_parse;
            using boost::spirit::qi::_1;
            using boost::spirit::qi::space;

            double value;
            std::vector<char> unitVect;
            if (phrase_parse(quantity.begin(), quantity.end(), double_[boost::phoenix::ref(value) = _1] >> (*char_)[boost::phoenix::ref(unitVect) = _1], space))
            {
                if (unitVect.empty())
                    return VariantService::Reflect(std::move(value));
                else
                {
                    std::string unit(&unitVect.front(), unitVect.size());
                    return VariantService::Reflect(Runtime::DynamicQuantity(value, unit));
                }
            }
            else
                throw std::runtime_error("Unable to convert '" + quantity + "' to a quantity");
        }

        // converts variable itself (it is overwritten)
        virtual void IntrusiveConvert(Variant& variable) {
            variable = Convert(variable);
        }
    };

    void ReflectDynamicQuantities(TypeLibraryPointer typeLibrary)
    {
        Class<Runtime::DynamicQuantity> cls(typeLibrary, "DynamicQuantity");
        cls.AddAttribute<UndefinedAttribute>([](const Runtime::DynamicQuantity& q) {return !!_isnan(q.GetValue()); });
        cls.Constructor<double>();
        cls.Operator(This.Const + This.Const);
        cls.Operator(This.Const - This.Const);
        cls.Operator(This.Const * This.Const, &MultiplyDynamicQuantities);
        cls.Operator(This.Const / This.Const, &DivideDynamicQuantities);
        cls.Operator(This.Const / double());
        cls.Operator(double() / This.Const);
        cls.Operator(This.Const * double());
        cls.Operator(double() * This.Const);
        cls.Operator(-This.Const);
        cls.Operator( + This.Const);
        cls.Operator(This.Const < This.Const);
        cls.Operator(This.Const <= This.Const);
        cls.Operator(This.Const > This.Const);
        cls.Operator(This.Const >= This.Const);
        cls.Operator(This.Const == This.Const);
        cls.Operator(This.Const != This.Const);

        typeLibrary->GetConversionGraph()->AddConversion(
            TypeId<Runtime::DynamicQuantity>(), 
            TypeId<void>(), 
            ConversionType::FallbackConversion, 
            ConversionPointer(new FallbackUnitConverter)
            );
        typeLibrary->GetConversionGraph()->AddConversion(
            TypeId<const std::string>(),
            TypeId<void>(),
            ConversionType::FallbackConversion,
            ConversionPointer(new StringToQuantityConversion)
        );
    }
}}}}