//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <time.h>
template<typename BaseLine, typename Contender>
auto Profile(BaseLine&& baseline, Contender&& contender, double allowedDifference) -> decltype(baseline())
{
#ifndef _DEBUG
    clock_t start = clock();
    for(size_t i = 0; i<10; ++i)
    {
        baseline();
        contender();
    }
    clock_t end = clock();
    std::size_t repetitions = 10000000;
    if(end-start > 0)
        repetitions /= (end-start);

    clock_t baseline_start = clock();
    volatile decltype(baseline()) result1 = 0;
    for(size_t i = 0; i < repetitions; ++i)
    {
        result1 += baseline();
    }
    clock_t baseline_end = clock();
    double baselineTime = double(baseline_end-baseline_start) / CLOCKS_PER_SEC;
    std::cout << "Baseline: " << baselineTime << std::endl;
    clock_t contender_start = clock();
    volatile decltype(contender()) result2 = 0;
    for(size_t i = 0; i < repetitions; ++i)
    {
        result2 += contender();
    }
    clock_t contender_end = clock();
    double contenderTime = double(contender_end-contender_start) / CLOCKS_PER_SEC;
    std::cout << "Contender: " << contenderTime << std::endl;
    std::cout << "Relative difference (contender/baseline): " << contenderTime / baselineTime << ", allowed: " << allowedDifference << std::endl;
    EXPECT_GE(allowedDifference, contenderTime / baselineTime);
    EXPECT_EQ(result1, result2);
    return result1-result2;
#endif
    return 0;
}
