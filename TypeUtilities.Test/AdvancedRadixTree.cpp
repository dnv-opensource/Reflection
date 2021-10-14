//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <vector>
#include <utility>
#include "gtest\gtest.h"
#include <list>
#include <map>
#include "TypeUtilities\RadixTree.h"
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "boost\fusion\adapted\struct\adapt_struct.hpp"
#include <fstream>
#include "boost\spirit\home\support\iterators\istream_iterator.hpp"

namespace DNVS {namespace MoFa { namespace TypeUtilities {


void TestMap(const std::list<std::pair<std::string, bool> >& values, const std::set<std::string>& notInTree = std::set<std::string>())
    {
        std::set<std::string> sorted;
        RadixTree<std::string, bool> tree;
        for(auto it = values.begin(); it != values.end(); ++it)
        {
            tree.insert(*it);
            sorted.insert(it->first);
        }
        for(auto it = values.begin(); it != values.end(); ++it)
        {
            auto itree = tree.find(it->first.c_str());
            ASSERT_NE(tree.end(), itree);
            EXPECT_EQ(it->first, std::string(itree->first));
            EXPECT_EQ(it->second, itree->second);
        }
        auto iother = sorted.begin();
        size_t index = 0;
        for(auto it = tree.begin(); it != tree.end(); ++it, ++iother, ++index)
        {
            EXPECT_EQ(*iother, it->first);
        }
        EXPECT_EQ(tree.size(), index);
        EXPECT_EQ(tree.size(), values.size());
        for(auto it = notInTree.begin(); it != notInTree.end(); ++it)
        {
            EXPECT_EQ(tree.end(), tree.find(it->c_str()));
        }
    }
    TEST(AdvancedRadixTreeTests, InsertStringCompare)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        std::set<std::string> notInTree;
        notInTree.insert("C");
        notInTree.insert("Beam2");
        TestMap(values, notInTree);
    }

    TEST(AdvancedRadixTreeTests, InsertString_ThenInsertOtherString)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("C", true));
        TestMap(values);
    }

    TEST(AdvancedRadixTreeTests, InsertString_ThenInsertSubset)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("B", true));
        TestMap(values);
    }
    
    TEST(AdvancedRadixTreeTests, InsertBeam1_B_Bell)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("B", true));
        values.push_back(std::make_pair("Bell", true));
        TestMap(values);
    }

    TEST(AdvancedRadixTreeTests, InsertTwoSimilarStrings)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("Beam2", true));
        std::set<std::string> notInTree;
        notInTree.insert("C");
        TestMap(values, notInTree);
    }

    TEST(AdvancedRadixTreeTests, InsertLongStrings)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("AVeryLongNamedBeam", true));
        values.push_back(std::make_pair("AVeryLongNamedBeam2", true));
        values.push_back(std::make_pair("BVeryLongNamedBeam2", true));
        std::set<std::string> notInTree;
        notInTree.insert("C");
        TestMap(values, notInTree);
    }

    TEST(AdvancedRadixTreeTests, InsertThreeSimilarStrings)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("Beam2", true));
        values.push_back(std::make_pair("Beam25", true));
        std::set<std::string> notInTree;
        notInTree.insert("C");
        notInTree.insert("Beam11111");
        TestMap(values, notInTree);
    }

    TEST(AdvancedRadixTreeTests, InsertSomeRandomStrings)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("B", true));
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("B2", true));
        values.push_back(std::make_pair("B22", true));
        values.push_back(std::make_pair("Bea11", true));
        values.push_back(std::make_pair("Bea12", true));
        values.push_back(std::make_pair("B2ea11", true));
        TestMap(values);
    }

    TEST(AdvancedRadixTreeTests, InsertBeam_Plate_PlateType)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Beam1", true));
        values.push_back(std::make_pair("Plate1", true));
        values.push_back(std::make_pair("PlateType1", true));
        TestMap(values);
    }

    TEST(AdvancedRadixTreeTests, TestSubstring)
    {
        std::list<std::pair<std::string, bool> > values;
        values.push_back(std::make_pair("Charleston", true));
        values.push_back(std::make_pair("Charlottetown", true));
        values.push_back(std::make_pair("Charlo", true));
        TestMap(values);
    }

    std::string ToString(size_t value)
    {
        std::string text;
        while(value!=0)
        {
            text = char('0' + (value%10)) + text;
            value/=10;
        }
        if(text.empty())
            text = '0';
        return text;
    }
    template<typename T>
    void TestPerformance(T& tree)
    {        
        for(size_t i = 1; i < 10000; ++i)
        {
            tree.insert(std::make_pair(("Beam" + ToString(i)).c_str(), true));
            tree.insert(std::make_pair(("Plate" + ToString(i)).c_str(), true));
            tree.insert(std::make_pair(("PlateType" + ToString(i)).c_str(), true));
        }
        tree.insert(std::make_pair("AsfjlksdfgklasdhdfkløhasdløghasdløasdkløfjkldøJDKLØDASJKLØADFJGKLØASJDFKLØAJSDFKLØASDJGKLØASDJKLØFJKASDLKØ", true));
        bool alwaysEnd = false;
        if(tree.find("AsfjlksdfgklasdhdfkløhasdløghasdløasdkløfjkldøJDKLØDASJKLØADFJGKLØASJDFKLØAJSDFKLØASDJGKLØASDJKLØFJKASDLKØ") == tree.end())
            alwaysEnd = true;
        size_t i;
        for(i = 1; i < 5000000; ++i)
        {
            std::string index = ToString(i);
            std::string beam = "Beam" + index;
            auto it1 = tree.find(beam);
            if(i < 10000 && !alwaysEnd)
            {
                if(it1 == tree.end())
                    break;
            }
            else
            {
                if(it1 != tree.end())
                    break;
            }
            std::string plate = "Plate" + index;
            auto it2 = tree.find(plate);
            if(i < 10000 && !alwaysEnd)
            {
                if(it2 == tree.end())
                    break;
            }
            else
            {
                if(it2 != tree.end())
                    break;
            }
            std::string plateType = "PlateType" + index;
            if(plateType != plateType)
                break;
            auto it3 = tree.find(plateType);
            if(i < 10000 && !alwaysEnd)
            {
                if(it3 == tree.end())
                    break;
            }
            else
            {
                if(it3 != tree.end())
                    break;
            }
            std::string longName = "AsfjlksdfgklasdhdfkløhasdløghasdløasdkløfjkldøJDKLØDASJKLØADFJGKLØASJDFKLØAJSDFKLØASDJGKLØASDJKLØFJKASDLKØ";
            if(longName != longName)
                break;
            auto it4 = tree.find(longName);
            if(it4 == tree.end() && !alwaysEnd)
                break;
        }
        ASSERT_EQ(i, 5000000);
    }
    TEST(AdvancedRadixTreeTests, DISABLED_RadixMapPerformance)
    {
        RadixTree<std::string, bool> tree;
        TestPerformance(tree);
    }
    TEST(AdvancedRadixTreeTests, DISABLED_STLMapPerformance)
    {
        std::map<std::string, bool> tree;
        TestPerformance(tree);
    }

    class FakeMap : public std::map<std::string, bool>
    {
    public:
        iterator find(const std::string& key)
        {
            return end();
        }
    };
    TEST(AdvancedRadixTreeTests, DISABLED_BaselineMapPerformance)
    {
        FakeMap tree;
        TestPerformance(tree);
    }

    TEST(AdvancedRadixTreeTests, ArrayNotation)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        EXPECT_EQ(42, tree["Hello world"]);
    }
    TEST(AdvancedRadixTreeTests, ArrayNotation2)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        EXPECT_EQ(42, tree["Hello world"]);
        EXPECT_EQ(43, tree["Hello world1"]);
    }

    TEST(AdvancedRadixTreeTests, Begin_Test)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        tree["Aloe Vera"] = 19;
        EXPECT_EQ(19, tree.begin()->second);
    }

    TEST(AdvancedRadixTreeTests, Begin_EmptyTree_EqualsEnd)
    {
        RadixTree<std::string, int> tree;
        EXPECT_EQ(tree.end(), tree.begin());
    }

    TEST(AdvancedRadixTreeTests, Size_OfTree)
    {
        RadixTree<std::string, int> tree;
        EXPECT_TRUE(tree.empty());
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        tree["Aloe Vera"] = 19;
        tree.insert(std::make_pair("Hello world", 18));
        tree["Aloe Vera"] = 16;
        EXPECT_EQ(3, tree.size());
        EXPECT_FALSE(tree.empty());
    }

    TEST(AdvancedRadixTreeTests, IterateFromBeginToEnd)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        tree["Aloe Vera"] = 19;
        std::vector<std::string> sorted(3);
        sorted[0] = "Aloe Vera";
        sorted[1] = "Hello world";
        sorted[2] = "Hello world1";
        size_t i=0;
        for(auto it = tree.begin(); it != tree.end(); ++it, ++i)
        {
            EXPECT_EQ(sorted[i], it->first);
        }
        EXPECT_EQ(tree.size(), i);
    }

    TEST(AdvancedRadixTreeTests, ClearTest)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        tree["Aloe Vera"] = 19;
        tree.clear();
        EXPECT_TRUE(tree.empty());
        tree["Hello w"] = 18;
        EXPECT_NE(tree.end(), tree.find("Hello w"));
        EXPECT_EQ(tree.end(), tree.find("Hello world"));
        EXPECT_EQ(1, tree.size());
    }

    TEST(AdvancedRadixTreeTests, MoveMap)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        tree["Aloe Vera"] = 19;
        RadixTree<std::string, int> newTree = std::move(tree);
        EXPECT_TRUE(tree.empty());
        EXPECT_EQ(3, newTree.size());
        EXPECT_EQ(tree.end(), tree.find("Hello world"));
        EXPECT_NE(tree.end(), newTree.find("Hello world"));
    }

    TEST(AdvancedRadixTreeTests, CopyMap)
    {
        RadixTree<std::string, int> tree;
        tree["Hello world"] = 42;
        tree["Hello world1"] = 43;
        tree["Aloe Vera"] = 19;
        RadixTree<std::string, int> newTree = tree;
        EXPECT_EQ(3, tree.size());
        EXPECT_EQ(3, newTree.size());
        EXPECT_NE(tree.end(), tree.find("Hello world"));
        EXPECT_NE(tree.end(), newTree.find("Hello world"));
    }

    TEST(AdvancedRadixTreeTests, InsertRange)
    {
        std::map<std::string,int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        RadixTree<std::string, int> tree;
        tree.insert(map.begin(), map.end());
        EXPECT_EQ(3, tree.size());
        EXPECT_NE(tree.end(), tree.find("Hello world"));
    }
    TEST(AdvancedRadixTreeTests, ConstructWithRange)
    {
        std::map<std::string,int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        RadixTree<std::string, int> tree(map.begin(), map.end());
        EXPECT_EQ(3, tree.size());
        EXPECT_NE(tree.end(), tree.find("Hello world"));
    }

    TEST(AdvancedRadixTreeTests, ConstructWithTypeInfo)
    {
        RadixTree<const std::type_info*,int> map;
        map[typeid(int)] = 13;
        map[typeid(RadixTree<const std::type_info*,int>)] = 18;
        map[typeid(std::string)] = 14;
        EXPECT_EQ(3, map.size());
        EXPECT_NE(map.end(), map.find(typeid(int)));
    }

    TEST(AdvancedRadixTreeTests, GetLowerBound)
    {
        std::map<std::string,int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        RadixTree<std::string, int> tree(map.begin(), map.end());
        EXPECT_EQ(tree.find("Hello world"), tree.lower_bound("Hello world"));
        EXPECT_EQ(tree.find("Hello world1"), tree.lower_bound("Hello world0"));
        EXPECT_EQ(tree.find("Hello world"), tree.lower_bound("Betakaroten"));
        EXPECT_EQ(tree.find("Hello world"), tree.lower_bound("Aloe Vero"));
        EXPECT_EQ(tree.find("Aloe Vera"), tree.lower_bound("Aloe Veq"));
        EXPECT_EQ(tree.end(), tree.lower_bound("Hello world2"));
    }

    TEST(AdvancedRadixTreeTests, GetUpperBound)
    {
        std::map<std::string,int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        RadixTree<std::string, int> tree(map.begin(), map.end());
        EXPECT_EQ(tree.find("Hello world1"), tree.upper_bound("Hello world"));
        EXPECT_EQ(tree.find("Hello world1"), tree.upper_bound("Hello world0"));
        EXPECT_EQ(tree.find("Hello world"), tree.upper_bound("Betakaroten"));
        EXPECT_EQ(tree.find("Hello world"), tree.upper_bound("Aloe Vero"));
        EXPECT_EQ(tree.find("Aloe Vera"), tree.upper_bound("Aloe Veq"));
        EXPECT_EQ(tree.end(), tree.upper_bound("Hello world2"));
    }

    TEST(AdvancedRadixTreeTests, DecrementIterator)
    {
        std::map<std::string,int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        RadixTree<std::string, int> tree(map.begin(), map.end());
        auto it = tree.end();
        EXPECT_EQ(--it, tree.find("Hello world1"));
        EXPECT_EQ(--it, tree.find("Hello world"));
    }

    TEST(AdvancedRadixTreeTests, EraseTests)
    {
        RadixTree<std::string, int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        EXPECT_EQ(1, map.erase("Hello world"));
        EXPECT_EQ(map.end(), map.find("Hello world"));
        EXPECT_NE(map.end(), map.find("Hello world1"));
        EXPECT_EQ(2, map.size());
        EXPECT_EQ(1, map.erase("Hello world1"));
        EXPECT_EQ(1, map.size());
    }

    TEST(AdvancedRadixTreeTests, EraseTests2)
    {
        RadixTree<std::string, int> map;
        map["Hello world"] = 42;
        map["Hello world1"] = 43;
        map["Aloe Vera"] = 19;
        EXPECT_EQ(1, map.erase("Hello world1"));
        EXPECT_EQ(0, map.erase("Hello world1"));
        EXPECT_EQ(map.end(), map.find("Hello world1"));
        EXPECT_NE(map.end(), map.find("Hello world"));
        EXPECT_EQ(2, map.size());
    }
//    "A0A0A0","47.055640","-53.201979","Gander","NL"
    
    struct Plant {
        std::string Symbol;
        std::string SynonymSymbol;
        std::string ScientificNameWithAuthor;
        std::string CommonName;
        std::string Family;
    };

    struct Canada {
        std::string postalCode;
        double x;
        double y;
        std::string city;
        std::string code;
    };
}}}
BOOST_FUSION_ADAPT_STRUCT(
    DNVS::MoFa::TypeUtilities::Canada,
    (std::string, postalCode)
    (double, x)
    (double, y)
    (std::string, city)
    (std::string, code)
);

BOOST_FUSION_ADAPT_STRUCT(
    DNVS::MoFa::TypeUtilities::Plant,
    (std::string, Symbol)
    (std::string, SynonymSymbol)
    (std::string, ScientificNameWithAuthor)
    (std::string, CommonName)
    (std::string, Family)
    );

namespace DNVS {namespace MoFa { namespace TypeUtilities {

    using namespace boost::spirit;

    template <typename Iterator>
    struct CanadaParser : qi::grammar<Iterator, Canada(), ascii::space_type>
    {
        CanadaParser() : CanadaParser::base_type(start)
        {
            using qi::double_;
            using qi::lit;
            using qi::double_;
            using qi::lexeme;
            using standard::char_;

            quoted_string %= lexeme['"' >> *(char_ - '"') >> '"'];
            quoted_double %= '"' >> double_ >> '"';
            start %=                               
                quoted_string >> ','
                >>  quoted_double >> ','
                >>  quoted_double >> ','
                >>  quoted_string >> ','
                >>  quoted_string;
                ;
        }

        qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
        qi::rule<Iterator, double(), ascii::space_type> quoted_double;
        qi::rule<Iterator, Canada(), ascii::space_type> start;
    };

    template <typename Iterator>
    struct PlantParser : qi::grammar<Iterator, Plant(), ascii::space_type>
    {
        PlantParser() : PlantParser::base_type(start)
        {
            using qi::lexeme;
            using standard::char_;

            quoted_string %= lexeme['"' >> *(char_ - '"') >> '"'];
            start %=                               
                quoted_string >> ','
                >>  quoted_string >> ','
                >>  quoted_string >> ','
                >>  quoted_string >> ','
                >>  quoted_string;
            ;
        }

        qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
        qi::rule<Iterator, Plant(), ascii::space_type> start;
    };
    static size_t memoryUsed = 0;
    template<typename T>
    class CustomAllocator : public std::allocator<T>
    {        
    public:
        CustomAllocator() {}
        CustomAllocator(const CustomAllocator& other) : std::allocator<T>(other) {}
        template<typename U>
        CustomAllocator(const CustomAllocator<U>& other) : std::allocator<T>(other) {}
        template<typename U>
        struct rebind {
            typedef CustomAllocator<U> other;
        };
        T* allocate(size_t count)
        {
            memoryUsed += count * sizeof(T);
            return std::allocator<T>::allocate(count);
        }
        void deallocate(T* ptr, size_t count)
        {
            memoryUsed -= count * sizeof(T);
            std::allocator<T>::deallocate(ptr, count);
        }
    };

    template<typename MapType>
    void ParserCanada(MapType& map)
    {
        std::ifstream stream("zipcodeset.txt");
        stream.unsetf(std::ios::skipws);
        istream_iterator begin(stream);
        istream_iterator end;

        typedef typename CanadaParser<istream_iterator> CanadaParserType;
        std::vector<Canada> canada;
        canada.reserve(100000);
        CanadaParserType canadaParser;
        if(phrase_parse(begin, end, (*canadaParser)[boost::phoenix::ref(canada) = qi::_1], ascii::space))
        {
            clock_t start_insert = clock();
            for(size_t i = 0; i < canada.size(); ++i)
                map[canada[i].postalCode] = canada[i];
            clock_t end_insert = clock();
            std::cout << "Insert time for " << canada.size() << " items: " << double(end_insert - start_insert)/CLOCKS_PER_SEC << std::endl;
            clock_t start_find = clock();
            for(size_t j = 0; j < 10; ++j)
                for(size_t i = 0; i < canada.size(); ++i)
                    if(map.find(canada[i].postalCode) == map.end())
                        throw std::runtime_error("Error when trying to find " + canada[i].postalCode);
            clock_t end_find = clock();
            std::cout << "Find time for " << canada.size() << " items: " << double(end_find - start_find)/CLOCKS_PER_SEC/10 << std::endl;
            std::cout << "Memory used: " << memoryUsed << " bytes" << std::endl;
            clock_t start_erase = clock();
            for(size_t i = 0; i < canada.size(); ++i)
                map.erase(canada[i].postalCode);
            clock_t end_erase = clock();
            std::cout << "Erase time for " << canada.size() << " items: " << double(end_erase - start_erase)/CLOCKS_PER_SEC << std::endl;
            std::cout << "Memory used: " << memoryUsed << " bytes" << std::endl;
        }
        else
        {
            std::cout << "fail! " << canada.size();
        }
    }
    TEST(AdvancedRadixTreeTests, DISABLED_TestPerformanceCanada)
    {
        //std::map<std::string, Canada, std::less<std::string>, CustomAllocator<std::string> > map;
        RadixTree<std::string, Canada, DefaultTreeTraits<std::string>, CustomAllocator<std::string> > map;
        ParserCanada(map);
    }

    template<typename MapType>
    void ParserPlant(MapType& map)
    {
        std::ifstream stream("Plants.txt");
        stream.unsetf(std::ios::skipws);
        istream_iterator begin(stream);
        istream_iterator end;

        typedef typename PlantParser<istream_iterator> PlantParserType;
        std::vector<Plant> plants;
        plants.reserve(100000);
        PlantParserType plantParser;
        if(phrase_parse(begin, end, (*plantParser)[boost::phoenix::ref(plants) = qi::_1], ascii::space))
        {
            clock_t start_insert = clock();
            for(size_t i = 0; i < plants.size(); ++i)
                map[plants[i].ScientificNameWithAuthor] = plants[i];
            clock_t end_insert = clock();
            std::cout << "Insert time for " << plants.size() << " items: " << double(end_insert - start_insert)/CLOCKS_PER_SEC << std::endl;
            clock_t start_find = clock();
            for(size_t j = 0; j < 10; ++j)
                for(size_t i = 0; i < plants.size(); ++i)
                    if(map.find(plants[i].ScientificNameWithAuthor) == map.end())
                        throw std::runtime_error("Error when trying to find " + plants[i].ScientificNameWithAuthor);
            clock_t end_find = clock();
            std::cout << "Find time for " << plants.size() << " items: " << double(end_find - start_find)/CLOCKS_PER_SEC/10 << std::endl;
            std::cout << "Memory used: " << memoryUsed << " bytes" << std::endl;
            clock_t start_erase = clock();
            for(size_t i = 0; i < plants.size(); ++i)
                map.erase(plants[i].ScientificNameWithAuthor);
            clock_t end_erase = clock();
            std::cout << "Erase time for " << plants.size() << " items: " << double(end_erase - start_erase)/CLOCKS_PER_SEC << std::endl;
            std::cout << "Memory used: " << memoryUsed << " bytes" << std::endl;
        }
        else
        {
            std::cout << "fail! " << plants.size();
        }
    }
    TEST(AdvancedRadixTreeTests, DISABLED_TestPerformancePlant)
    {
        //std::map<std::string, Plant, std::less<std::string>, CustomAllocator<std::string> > map;
        RadixTree<std::string, Plant, DefaultTreeTraits<std::string>, CustomAllocator<std::string> > map;
        ParserPlant(map);
    }
}}}