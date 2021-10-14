//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "TypeUtilities\RadixTree.h"
#include <map>
#include <string>
#include <memory>

namespace DNVS {namespace MoFa {namespace TypeUtilities { namespace Tests {
    class CapitalLetterIndexer {
    public:
        static const size_t MaxArraySize = 'Z'-'0'+1;
        size_t operator()(char text) const {return text-'0';}
        bool IsTerminatingCharacter(char text) const {return text<'0' || text>'Z';}
    };
    template<typename MapT>
    void PopulateMap(MapT& myMap)
    {
        myMap.insert(std::make_pair("BGRAV",5));
        myMap.insert(std::make_pair("BEISTE",1));
        myMap.insert(std::make_pair("BELLO2",2));
        myMap.insert(std::make_pair("BELOAD1",3));
        myMap.insert(std::make_pair("BEUSLO",4));
        myMap.insert(std::make_pair("BNACCLO",6));
        myMap.insert(std::make_pair("BNDISPL",7));
        myMap.insert(std::make_pair("BNLOAD",8));
        myMap.insert(std::make_pair("BNMASS",9));
        myMap.insert(std::make_pair("BRIGAC",10));
        myMap.insert(std::make_pair("GBARM",11));
        myMap.insert(std::make_pair("GBEAMG",12));
        myMap.insert(std::make_pair("GBOX",13));
        myMap.insert(std::make_pair("GCHAN",14));
        myMap.insert(std::make_pair("GCOORD",15));
    }
    TEST(RadixTreeTests, InsertCards_Find)
    {
        RadixTree<std::string, int> myMap;
        PopulateMap(myMap);
        ASSERT_EQ(4, myMap.find("BEUSLO")->second);
        ASSERT_EQ(myMap.end(), myMap.find("DRESS"));
    }
    TEST(RadixTreeTests, InsertCards_Begin)
    {
        RadixTree<std::string, int> myMap;
        PopulateMap(myMap);
        ASSERT_EQ(1, myMap.begin()->second);
    }

    TEST(RadixTreeTests, ComparePerformance)
    {
        RadixTree<std::string, int> myMap;
        PopulateMap(myMap);
        clock_t start = clock();
        std::string beuslo("BEUSLO");
        std::string dress("DRESS");
        for(size_t i=0;i<1000000;++i)
        {
            volatile int a = myMap.find(beuslo)->second;
            volatile auto it = myMap.find(dress);
            
        }
        std::cout << std::endl << "New map: " << double(clock() - start)/CLOCKS_PER_SEC << std::endl;
        std::map<std::string, int> myMap2;
        PopulateMap(myMap2);
        start = clock();
        for(size_t i=0;i<1000000;++i)
        {
            volatile int a = myMap2.find(beuslo)->second;
            volatile auto it = myMap2.find(dress);
        }
        std::cout << "Old map: " << double(clock() - start)/CLOCKS_PER_SEC << std::endl;
    }

    TEST(RadixTreeTests, TestDeleteValueWhenClear_OneValueInMap)
    {
        typedef TypeUtilities::RadixTree<const std::type_info*, std::shared_ptr<int> > IntPointerMap;
        IntPointerMap a;
        std::shared_ptr<int> ptr = std::make_shared<int>(5);
        a[typeid(int)] = ptr;
        EXPECT_FALSE(ptr.unique());
        a.clear();
        EXPECT_TRUE(ptr.unique());
    }

    TEST(RadixTreeTests, TestDeleteValueWhenClear_TwoValuesInMap)
    {
        typedef TypeUtilities::RadixTree<const std::type_info*, std::shared_ptr<int> > IntPointerMap;
        IntPointerMap a;
        std::shared_ptr<int> ptr = std::make_shared<int>(5);
        a[typeid(unsigned int)] = ptr;
        a[typeid(unsigned long)] = ptr;
        EXPECT_FALSE(ptr.unique());
        a.clear();
        EXPECT_TRUE(ptr.unique());
    }

    TEST(RadixTreeTests, TestDeleteValueWhenErase_OneValueInMap)
    {
        typedef TypeUtilities::RadixTree<const std::type_info*, std::shared_ptr<int> > IntPointerMap;
        IntPointerMap a;
        std::shared_ptr<int> ptr = std::make_shared<int>(5);
        a[typeid(int)] = ptr;
        EXPECT_FALSE(ptr.unique());
        a.erase(typeid(int));
        EXPECT_TRUE(ptr.unique());
    }
}}}}