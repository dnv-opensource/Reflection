#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Members/ArgumentInfo.h"
namespace DNVS {namespace MoFa {namespace Reflection { namespace Members {


    ///Defines a list of arguments of size N.
    template<size_t N,typename Derived>
    class ArgumentList : public std::vector<ArgumentInfoPointer>
    {
    public:
        static const size_t MinimumCount=N;
        static const size_t MaximumCount=N;
        ArgumentList() : std::vector<ArgumentInfoPointer>(N) {}
        ArgumentList(const ArgumentList& other) : std::vector<ArgumentInfoPointer>(other) {}
    };
    

    template<size_t N>
    class NamedArgumentList : public ArgumentList<N,NamedArgumentList<N> > {
    public:
        NamedArgumentList(const NamedArgumentList<N-1>& lhs,const NamedArgumentList<1>& rhs) 
        {
            for(size_t i=0;i<lhs.size();++i) {
                (*this)[i]=lhs[i];
            }
            for(size_t i=0;i<rhs.size();++i) {
                (*this)[i+lhs.size()]=rhs[i];
            }
        }
    };
    template<>
    class NamedArgumentList<0> : public ArgumentList<0, NamedArgumentList<0> > {
    public:
    };

    template<>
    class NamedArgumentList<1> : public ArgumentList<1,NamedArgumentList<1> > {
    public:
        NamedArgumentList(ArgumentInfoPointer arg) {(*this)[0]=arg;}
        NamedArgumentList(const char* name) {(*this)[0]=new ArgumentInfo(name, "");}
        NamedArgumentList(const NamedArgumentList<0>& lhs, const NamedArgumentList<1>& rhs)
            : ArgumentList<1, NamedArgumentList<1> >(rhs) {}
    };

    template<size_t N,size_t Defaults=N>
    class NamedArgumentListWithDefaults : public ArgumentList<N,NamedArgumentListWithDefaults<N,Defaults> > {
    public:
        static const size_t MinimumCount=N-Defaults;
        NamedArgumentListWithDefaults(const NamedArgumentListWithDefaults<N-1,Defaults-1>& lhs,const NamedArgumentListWithDefaults<1>& rhs) {
            for(size_t i=0;i<lhs.size();++i) {
                (*this)[i]=lhs[i];
            }
            for(size_t i=0;i<rhs.size();++i) {
                (*this)[i+lhs.size()]=rhs[i];
            }
        }
    };

    template<size_t N>
    class NamedArgumentListWithDefaults<N,1> : public ArgumentList<N,NamedArgumentListWithDefaults<N,1> > {
    public:
        static const size_t MinimumCount=N-1;
        NamedArgumentListWithDefaults(const NamedArgumentList<N-1>& lhs,const NamedArgumentListWithDefaults<1>& rhs) 
        {
            for(size_t i=0;i<lhs.size();++i) {
                (*this)[i]=lhs[i];
            }
            for(size_t i=0;i<rhs.size();++i) {
                (*this)[i+lhs.size()]=rhs[i];
            }
        }
    };

    template<>
    class NamedArgumentListWithDefaults<1,1> : public ArgumentList<1,NamedArgumentListWithDefaults<1,1> > {
    public:
        static const size_t MinimumCount=0;
        NamedArgumentListWithDefaults(const ArgumentInfoPointer arg) {(*this)[0]=arg;}
        NamedArgumentListWithDefaults(const NamedArgumentList<0>& lhs, const NamedArgumentListWithDefaults<1,1>& rhs)
            : ArgumentList<1, NamedArgumentListWithDefaults<1,1> >(rhs) {}
    };

    template<size_t N>
    NamedArgumentList<N + 1> operator,(const NamedArgumentList<N>& lhs, const NamedArgumentList<1>& rhs)
    {
        return NamedArgumentList<N + 1>(lhs, rhs);
    }

    template<size_t N>
    NamedArgumentListWithDefaults<N + 1, 1> operator,(const NamedArgumentList<N>& lhs, const NamedArgumentListWithDefaults<1>& rhs)
    {
        return NamedArgumentListWithDefaults<N + 1, 1>(lhs, rhs);
    }

    template<size_t N, size_t Defaults>
    NamedArgumentListWithDefaults<N + 1, Defaults + 1> operator,(const NamedArgumentListWithDefaults<N, Defaults>& lhs, const NamedArgumentListWithDefaults<1>& rhs)
    {
        return NamedArgumentListWithDefaults<N + 1, Defaults + 1>(lhs, rhs);
    }

}}}}
