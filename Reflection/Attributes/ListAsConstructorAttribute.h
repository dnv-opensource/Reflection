#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The ListAsConstructorAttribute is used to control whether this method should be listed in the list of expandable items.
    For all constructors, the default is ListAlways (if it has named constructor arguments)
    For all other methods, the default is ListNever.
    */
    class ListAsConstructorAttribute {
    public:
        enum ListOption {
            ListAlways, //Always list this option
            ListTopLevel, //List this option only for top level model, such as NoCurveOffset() should not be listed as an option for a nested view model
            ListNever, //Ignore this option
        };
        ListOption GetOption() const { return m_option; }
        ListAsConstructorAttribute(ListOption option = ListAlways) : m_option(option) {}
    private:
        ListOption m_option;
    };
}}}}