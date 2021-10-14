#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {

    template<typename StoragePolicy,typename From,typename To>
    class ExtendConversion : public IConversion
    {
    public:
        //ConversionType::StandardConversion
        virtual Variants::Variant Convert(const Variants::Variant& var) {
            From* from=static_cast<From*>(var.Data());
            To* to=new(StoragePolicy::Allocate<To>()) To(from);
            return Variants::Variant(StoragePolicy::CreateStorage(to));
        }
    };

}}}}