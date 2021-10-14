#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "Reflection/Objects/Object.h"
#include <vector>
#include "TypeUtilities/Tribool.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    /**
    For any object, tries to find the constructor of that function that best matches the object.
    Example:
    We have the following constructors:
    Plane3d(Point, Vector3d)
    //These are static functions, but we can register them as NamedConstructor on the class.
    Plane3d ZPlane3d(double z);
    Plane3d YPlane3d(double y);
    Plane3d XPlane3d(double x);
    //operator- is not really a constructor, but we can add an attribute to this method to use it as if it was a constructor.
    //This is convenient in cases where we, in GUI want to script construction of an object as an operator. Such as
    //LinearVaryingCurveOffset(...) + AlignedCurveOffset(...)
    //instead of JoinedCurveOffset(LinearVaryingCurveOffset(...),AlignedCurveOffset(...))
    //We need to add the ListAsConstructorAttribute to enable this.
    Plane3d operator-() const;

    Imagine a plane: Plane3d(Point(4,1,2), Vector3d(0,-1,0));
    We want the selected constructor to be
    -YPlane3d(y);, where y = 1
    but for 
    Plane3d(Point(4,1,2), Vector3d(-1,-1,0));
    we need it to be 
    Plane3d(Point, Vector3d)
    This is done in the following way:
    1. Gather all constructors
    2. For each constructor, try to deduce its arguments.
    3. For the example of Plane3d, Plane3d operator-(const Plane3d& arg) is one of the "constructors".
       We want, for a general Plane3d 'a' to find out, what the argument to operator- needs to be to produce 'a'.
       So if 'a' is Plane3d(Point(0,0,0),Vector3d(0,-1,0)) then 'arg' is Plane3d(Point(0,0,0),Vector3d(0,1,0)) because -'arg' == 'a'.
       When detecting the best constructor for Plane3d(Point(0,0,0),Vector3d(0,1,0)) we find YPlane3d(0) to be the best suited, so 
       we end up with the expression -YPlane3d(0) representing 'a'. If, on the other hand, we had
       'a' = Plane3d(Point(0,0,0), Vector3d(1,1,0)), then the best matching constructor would be -'arg', based on the criteria listed for operator<, where 
       'arg' = Plane3d(Point(0,0,0), Vector3d(-1,-1,0)).
       Again, we are left with a plane, so we need to deduce the best constructor again, using 'arg' as input. The best constructor is -arg1,
       'arg1' = Plane3d(Point(0,0,0), Vector3d(1,1,0))
       We now have the expression --'a' which is the same as 'a'. Without a recursion stopper here, we will end up with infinitely many - signs and no plane.
       We therefore have a special recursion breaker if the argument type of any constructor is the same as the type being constructed:
          Returned type            Argument Type
                 |                   |
                 v                   v
       Such as Plane3d operator-(Plane3d)
    */
    class REFLECTION_IMPORT_EXPORT CallableConstructor
    {
    public:
        CallableConstructor();
        CallableConstructor(Members::MemberPointer member, const Objects::Object& object);
        /**
        The rules for validity is as follows:
        1. if constructor has the DefaultConstructorSelectorAttribute and this returns false for object, return false.
        2. Try to generate arguments based on GenerateConstructorArgumentsAttribute. If this succeeds return true
        3. Try to generate arguments based on properties with the same name as the constructor arguments. If this succeeds, return true
        4. Otherwise return false.
        */
        bool IsValid() const;        
        bool IsInfinitlyRecursive() const;
        /**
        The rules for sorting of constructor is as follows, in the sequence given below:
        1. If constructor has no arguments, it is selected last.
        2. If constructor has GenerateConstructorArgumentsAttribute, it is preferred
        3. If one of the constructor arguments has the same type as the input object, it is not preferred
        4. If constructor has ListAsConstructorAttribute, it is preferred
        5. After this, it is random which constructor is chosen.
        */
        bool operator<(const CallableConstructor& other) const;
        const Members::MemberPointer& GetMember() const;
        const std::vector<Variants::Variant>& GetArguments() const;
    private:
        bool IsDefaultConstructor() const;
        bool HasGenerateConstructorArgumentsAttribute() const;
        bool HasListAsConstructorAttribute() const;
        bool HasMemberArgumentOfSameTypeAsThis() const;
        TypeUtilities::Tribool TryGenerateConstructorArgumentsFromAttribute();
        TypeUtilities::Tribool TryGenerateConstructorArgumentsFromProperties();
        Members::MemberPointer m_member;
        Objects::Object m_object;
        std::vector<Variants::Variant> m_arguments;
        bool m_isValid;
        bool m_constructorArgumentsFromAttributes;
    };
}}}}