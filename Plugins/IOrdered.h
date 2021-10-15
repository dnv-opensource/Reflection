#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS { namespace MoFa { namespace Plugins {
    /**In some cases, the order of initialization of the subsystems matter. Examples of this include: Generating clean JS.
    If we want to distribute the work of generating clean JS, we need to make sure that 
    - properties are exported prior to structures
    - structures and guiding geometry are exported prior to sets
    - sets are exported prior to analyses.
    In these cases, let your subsystem base class inherit from IOrdered and specify the order by overriding GetPrerequisites().
    */
    class IOrdered
    {
    public:        
        virtual ~IOrdered() {}
        virtual std::string GetName() const = 0;
        virtual std::list<std::string> GetPrerequisites() const = 0;
    };
}}}