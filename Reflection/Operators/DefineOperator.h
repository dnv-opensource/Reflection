#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Operators/BinaryOperator.h"
#include "Reflection/Operators/UnaryOperator.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Operators {

    ///Class to simplify registration of custom operators for a given type Reflected.
    template<typename Reflected, typename StorageType = Reflected>
    class DefineOperator
    {
    public:
        DefineOperator(TypeLibraries::TypePointer type) : m_type(type) {}
        /**
        Reflects a unary or binary operator expression.
        Usage:
        DefineOperator<int> cls(...);
        //Defines the operator int+double
        cls.Operator(This.Const+double());
        //Defines the operator ++int
        cls.Operator(++This);
        */
        template<typename OperatorGenerator>
        typename OperatorGenerator::template Rebind<Reflected, StorageType>::Type& Operator(const OperatorGenerator&) {
            //Replace all occurrences of 
            //- This with Reflected&
            //- This.Const with const Reflected&
            //- Other<T> with T
            typedef typename OperatorGenerator::template Rebind<Reflected, StorageType>::Type OperatorType;
            boost::intrusive_ptr<OperatorType> member(new OperatorType(OperatorGenerator::TypeName::Name(),m_type->GetConversionGraph()));
            return *boost::static_pointer_cast<OperatorType>(m_type->AddMember(OperatorGenerator::TypeName::Name(),member));
        }
        template<typename OperatorGenerator, typename Callback>
        typename OperatorGenerator::template Rebind<Reflected, StorageType, Callback>::Type& Operator(const OperatorGenerator&, Callback callback) {
            //Replace all occurrences of 
            //- This with Reflected&
            //- This.Const with const Reflected&
            //- Other<T> with T
            typedef typename OperatorGenerator::template Rebind<Reflected, StorageType, Callback>::Type OperatorType;
            boost::intrusive_ptr<OperatorType> member(new OperatorType(OperatorGenerator::TypeName::Name(), m_type->GetConversionGraph(), callback));
            return *boost::static_pointer_cast<OperatorType>(m_type->AddMember(OperatorGenerator::TypeName::Name(), member));
        }
        ///Specializations to support operator*, which can also be used to support registration of e.g. MyClass* < MyClass*
        typename ThisOperators::UnaryOperatorGenerator<ThisOperators::This<ThisOperators::ThisType>,DNVS::MoFa::Operators::Tags::Dereference>::Rebind<Reflected, StorageType>::Type&
            Operator(const ThisOperators::This<ThisOperators::ThisPointerType>&) 
        {
            return Operator(ThisOperators::UnaryOperatorGenerator<ThisOperators::This<ThisOperators::ThisType>,DNVS::MoFa::Operators::Tags::Dereference>());
        }

        typename ThisOperators::UnaryOperatorGenerator<ThisOperators::This<ThisOperators::ThisConstType>,DNVS::MoFa::Operators::Tags::Dereference>::Rebind<Reflected, StorageType>::Type&
            Operator(const ThisOperators::This<ThisOperators::ThisConstPointerType>&) 
        {
            return Operator(ThisOperators::UnaryOperatorGenerator<ThisOperators::This<ThisOperators::ThisConstType>,DNVS::MoFa::Operators::Tags::Dereference>());
        }

    private:
        TypeLibraries::TypePointer m_type;
    };

}}}}
