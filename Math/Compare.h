#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Math\NumericTraits.h"

namespace DNVS {namespace MoFa {namespace Math {
    template<typename Object,typename Comparator,typename A0>
    bool Compare(const Comparator& c,A0 a0)
    {
        if(c.Compare(Object(a0),Object(c.GetDifferentValue(a0))) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
            return false;
    }
    template<typename Object,typename Comparator,typename A0,typename A1>
    bool Compare(const Comparator& c,A0 a0, A1 a1)
    {
        if(c.Compare(Object(a0,a1),Object(c.GetDifferentValue(a0),a1)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1),Object(a0,c.GetDifferentValue(a1))) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2)
    {
        if(c.Compare(Object(a0,a1,a2),Object(c.GetDifferentValue(a0),a1,a2)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2),Object(a0,c.GetDifferentValue(a1),a2)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2),Object(a0,a1,c.GetDifferentValue(a2))) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3)
    {
        if(c.Compare(Object(a0,a1,a2,a3),Object(c.GetDifferentValue(a0),a1,a2,a3)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3),Object(a0,c.GetDifferentValue(a1),a2,a3)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3),Object(a0,a1,c.GetDifferentValue(a2),a3)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3),Object(a0,a1,a2,c.GetDifferentValue(a3))) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3,typename A4>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3,A4 a4)
    {
        if(c.Compare(Object(a0,a1,a2,a3,a4),Object(c.GetDifferentValue(a0),a1,a2,a3,a4)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4),Object(a0,c.GetDifferentValue(a1),a2,a3,a4)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4),Object(a0,a1,c.GetDifferentValue(a2),a3,a4)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4),Object(a0,a1,a2,c.GetDifferentValue(a3),a4)) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4),Object(a0,a1,a2,a3,c.GetDifferentValue(a4))) != c.Compare(a4,c.GetDifferentValue(a4))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3,typename A4,typename A5>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3,A4 a4,A5 a5)
    {
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5),Object(c.GetDifferentValue(a0),a1,a2,a3,a4,a5)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5),Object(a0,c.GetDifferentValue(a1),a2,a3,a4,a5)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5),Object(a0,a1,c.GetDifferentValue(a2),a3,a4,a5)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5),Object(a0,a1,a2,c.GetDifferentValue(a3),a4,a5)) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5),Object(a0,a1,a2,a3,c.GetDifferentValue(a4),a5)) != c.Compare(a4,c.GetDifferentValue(a4))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5),Object(a0,a1,a2,a3,a4,c.GetDifferentValue(a5))) != c.Compare(a5,c.GetDifferentValue(a5))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3,A4 a4,A5 a5,A6 a6)
    {
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(c.GetDifferentValue(a0),a1,a2,a3,a4,a5,a6)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(a0,c.GetDifferentValue(a1),a2,a3,a4,a5,a6)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(a0,a1,c.GetDifferentValue(a2),a3,a4,a5,a6)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(a0,a1,a2,c.GetDifferentValue(a3),a4,a5,a6)) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(a0,a1,a2,a3,c.GetDifferentValue(a4),a5,a6)) != c.Compare(a4,c.GetDifferentValue(a4))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(a0,a1,a2,a3,a4,c.GetDifferentValue(a5),a6)) != c.Compare(a5,c.GetDifferentValue(a5))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6),Object(a0,a1,a2,a3,a4,a5,c.GetDifferentValue(a6))) != c.Compare(a6,c.GetDifferentValue(a6))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7)
    {
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(c.GetDifferentValue(a0),a1,a2,a3,a4,a5,a6,a7)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,c.GetDifferentValue(a1),a2,a3,a4,a5,a6,a7)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,a1,c.GetDifferentValue(a2),a3,a4,a5,a6,a7)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,a1,a2,c.GetDifferentValue(a3),a4,a5,a6,a7)) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,a1,a2,a3,c.GetDifferentValue(a4),a5,a6,a7)) != c.Compare(a4,c.GetDifferentValue(a4))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,a1,a2,a3,a4,c.GetDifferentValue(a5),a6,a7)) != c.Compare(a5,c.GetDifferentValue(a5))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,a1,a2,a3,a4,a5,c.GetDifferentValue(a6),a7)) != c.Compare(a6,c.GetDifferentValue(a6))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7),Object(a0,a1,a2,a3,a4,a5,a6,c.GetDifferentValue(a7))) != c.Compare(a7,c.GetDifferentValue(a7))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8)
    {
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(c.GetDifferentValue(a0),a1,a2,a3,a4,a5,a6,a7,a8)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,c.GetDifferentValue(a1),a2,a3,a4,a5,a6,a7,a8)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,c.GetDifferentValue(a2),a3,a4,a5,a6,a7,a8)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,a2,c.GetDifferentValue(a3),a4,a5,a6,a7,a8)) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,a2,a3,c.GetDifferentValue(a4),a5,a6,a7,a8)) != c.Compare(a4,c.GetDifferentValue(a4))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,a2,a3,a4,c.GetDifferentValue(a5),a6,a7,a8)) != c.Compare(a5,c.GetDifferentValue(a5))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,a2,a3,a4,a5,c.GetDifferentValue(a6),a7,a8)) != c.Compare(a6,c.GetDifferentValue(a6))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,a2,a3,a4,a5,a6,c.GetDifferentValue(a7),a8)) != c.Compare(a7,c.GetDifferentValue(a7))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8),Object(a0,a1,a2,a3,a4,a5,a6,a7,c.GetDifferentValue(a8))) != c.Compare(a8,c.GetDifferentValue(a8))) return false;
        return true;
    }

    template<typename Object,typename Comparator,typename A0,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
    bool Compare(const Comparator& c,A0 a0, A1 a1, A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9)
    {
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(c.GetDifferentValue(a0),a1,a2,a3,a4,a5,a6,a7,a8,a9)) != c.Compare(a0,c.GetDifferentValue(a0))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,c.GetDifferentValue(a1),a2,a3,a4,a5,a6,a7,a8,a9)) != c.Compare(a1,c.GetDifferentValue(a1))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,c.GetDifferentValue(a2),a3,a4,a5,a6,a7,a8,a9)) != c.Compare(a2,c.GetDifferentValue(a2))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,c.GetDifferentValue(a3),a4,a5,a6,a7,a8,a9)) != c.Compare(a3,c.GetDifferentValue(a3))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,a3,c.GetDifferentValue(a4),a5,a6,a7,a8,a9)) != c.Compare(a4,c.GetDifferentValue(a4))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,a3,a4,c.GetDifferentValue(a5),a6,a7,a8,a9)) != c.Compare(a5,c.GetDifferentValue(a5))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,a3,a4,a5,c.GetDifferentValue(a6),a7,a8,a9)) != c.Compare(a6,c.GetDifferentValue(a6))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,a3,a4,a5,a6,c.GetDifferentValue(a7),a8,a9)) != c.Compare(a7,c.GetDifferentValue(a7))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,a3,a4,a5,a6,a7,c.GetDifferentValue(a8),a9)) != c.Compare(a8,c.GetDifferentValue(a8))) return false;
        if(c.Compare(Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9),Object(a0,a1,a2,a3,a4,a5,a6,a7,a8,c.GetDifferentValue(a9))) != c.Compare(a9,c.GetDifferentValue(a9))) return false;
        return true;
    }

    struct NotEqualComparator {
        template<typename T>
        T GetDifferentValue(T value) const
        {
            return Math::NumericTraits<T>::GetNextValue(value);
        }
        template<typename Object>
        bool Compare(const Object& lhs,const Object& rhs) const
        {
            return lhs!=rhs;
        }
    };

    struct LessThanComparator {
        template<typename T>
        T GetDifferentValue(T value) const
        {
            return Math::NumericTraits<T>::GetNextValue(value);
        }
        template<typename Object>
        bool Compare(const Object& lhs,const Object& rhs) const
        {
            return lhs<rhs;
        }
    };

    struct EqualComparator {
        template<typename T>
        T GetDifferentValue(T value) const
        {
            return Math::NumericTraits<T>::GetNextValue(value);
        }
        template<typename Object>
        bool Compare(const Object& lhs,const Object& rhs) const
        {
            return !(lhs==rhs);
        }
    };

    struct AlmostEqualComparator {
        template<typename T>
        T GetDifferentValue(T value) const
        {
            return Math::NumericTraits<T>::GetAlmostSameValue(value);
        }
        template<typename Object>
        bool Compare(const Object& lhs,const Object& rhs) const
        {
            return lhs==rhs;
        }
        bool Compare(double lhs,double rhs) {
            return CompareAlmostEqual(lhs,rhs);
        }
        bool Compare(float lhs,float rhs) {
            return CompareAlmostEqual(lhs,rhs);
        }
    };

}}}
