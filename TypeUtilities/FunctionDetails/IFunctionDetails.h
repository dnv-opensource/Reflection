#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class IFunctionDetails 
    {
    public:
        virtual ~IFunctionDetails() {}
        ///Add a single invocation of this call with the duration it took.
        virtual void AddCall(double duration) = 0;
        ///Gets the name of the DLL in which this class lies.
        virtual const std::string& GetDllName() const = 0;
        ///Gets the name of the class in which this function belongs, if any. Returns blank if this is a free function.
        virtual const std::string& GetClassName() const = 0;
        ///Gets the name of this function
        virtual const std::string& GetFunctionName() const = 0;
        ///Gets the function signature, such as '(int, double)' or 'getter' or 'setter'
        virtual const std::string& GetSignature() const = 0;
        ///Gets the number of times this function has been called.
        virtual size_t GetUseCount() const = 0;
        ///Gets the sum of all the durations this function has been running.
        virtual double GetTotalDuration() const = 0;
        ///Gets the longest duration this function has been running.
        virtual double GetMaxDuration() const = 0;
    };

    ///Streaming support for Point
    template<typename _Elem,typename _Traits>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr, const IFunctionDetails& details)
    {        
        _Ostr << "<function ";
        _Ostr << "dllName = \"" << details.GetDllName() << "\" ";
        _Ostr << "className = \"" << details.GetClassName() << "\" ";
        _Ostr << "functionName = \"" << details.GetFunctionName() << "\" ";
        _Ostr << "signature = \"" << details.GetSignature() << "\" ";
        _Ostr << "useCount = \"" << details.GetUseCount() << "\" ";
        _Ostr << "totalDuration = \"" << details.GetTotalDuration() << "\" ";
        _Ostr << "maxDuration = \"" << details.GetMaxDuration() << "\" ";
        _Ostr << "/>" << std::endl;
        return _Ostr;
    }
}}}