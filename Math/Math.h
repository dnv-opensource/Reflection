#pragma  once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
namespace DNVS { namespace MoFa { namespace Math {

    inline double PI(){return 3.1415926535897932384626433832795028841972;}
    namespace Statistics{

        template <class ContainerT>
        typename ContainerT::value_type CalculateMedian(const ContainerT &data )
        {
            typedef typename ContainerT::value_type value_type;
            std::vector<value_type> dataSet = std::vector<value_type>(data.begin(),data.end());
            std::sort(dataSet.begin(),dataSet.end());
            size_t middleIndex = (dataSet.size()-1)/2;
            value_type median = dataSet.at(middleIndex);
            if(dataSet.size()%2==0)
                median = value_type((median + dataSet.at(middleIndex+1))/2);
            return median;
        }
    }
}}}