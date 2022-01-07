//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Scripting/SymbolReplacer.h"
#include "Services/ServiceProvider.h"
#include "Scripting/ISymbolReplacerService.h"
#include "Scripting/StreamSymbolReplacer.h"
#include "Services/ScopedServiceProvider.h"

namespace DNVS  {namespace MoFa {namespace Scripting {

    class MockSymbolReplacerService : public ISymbolReplacerService
    {
    public:
        MockSymbolReplacerService(SymbolReplacerPointer symbolReplacer) : m_symbolReplacer(symbolReplacer) 
        {
            m_symbolStrmReplacer = std::make_shared<StreamSymbolReplacer>(m_symbolReplacer);
        }
        virtual SymbolReplacerPointer GetSymbolReplacer() const override { return m_symbolReplacer; }
        virtual std::istream& TryReplaceStream(std::istream& stream) { return m_symbolStrmReplacer->TryReplace(stream); }

        static std::shared_ptr<MockSymbolReplacerService> CreateMockService()
        {
            auto replacer = std::make_shared<SymbolReplacer >("#");
            replacer->AddHandler("X", [](long n) {return "XArray[" + std::to_string(n) + "]"; });
            replacer->AddHandler("Y", [](long n) {return "YArray[" + std::to_string(n) + "]"; });
            auto myService = std::make_shared<MockSymbolReplacerService>(replacer);
            Services::ServiceProvider::Instance().RegisterService<ISymbolReplacerService>(myService);
            return myService;
        }
    private:
        SymbolReplacerPointer m_symbolReplacer;
        std::shared_ptr<StreamSymbolReplacer> m_symbolStrmReplacer;
    };

    TEST(SymbolReplacerTests, VerifySharpSymbol)
    {
        SymbolReplacer replacer{ "#" };
        replacer.AddHandler("", [](long n) {return "ValidJS_" + std::to_string(n) + "_";});

        EXPECT_EQ(replacer.Replace("#3"), "ValidJS_3_");
        EXPECT_EQ(replacer.Replace("#-86"), "ValidJS_-86_");
        EXPECT_EQ(replacer.Replace("#3 + 5 mm"), "ValidJS_3_ + 5 mm");
        EXPECT_EQ(replacer.Replace(R"(str = "#3 test";\npoint = #3 + 5 mm;)"), 
                                     "str = \"#3 test\";\\npoint = ValidJS_3_ + 5 mm;");
    }

    TEST(SymbolReplacerTests, VerifyQuotation)
    {
        SymbolReplacer replacer{ "#" };
        replacer.AddHandler("", [](long) {return ""; });

        EXPECT_EQ(replacer.Replace(""), "");
        EXPECT_EQ(replacer.Replace(R"("#3 test")"), "\"#3 test\"");
        EXPECT_EQ(replacer.Replace(R"("#3 test"  )"), "\"#3 test\"  ");
        EXPECT_EQ(replacer.Replace(R"('#3 test')"), "'#3 test'");
        EXPECT_EQ(replacer.Replace("\"'#3\""), "\"'#3\"");
        EXPECT_EQ(replacer.Replace("'text\\'#3'"), "'text\\'#3'");
        EXPECT_EQ(replacer.Replace(R"(' "#3" test')"), "' \"#3\" test'");
    }

    TEST(SymbolReplacerTests, VerifySymbolWithIds)
    {
        SymbolReplacer replacer{ "#" };
        replacer.AddHandler("X", [](long n) {return "XArray[" + std::to_string(n) + "]"; });
        replacer.AddHandler("Y", [](long n) {return "YArray[" + std::to_string(n) + "]"; });

        EXPECT_EQ(replacer.Replace("#X56"), "XArray[56]");
        EXPECT_EQ(replacer.Replace("#Y1421, ..."), "YArray[1421], ...");
        EXPECT_EQ(replacer.Replace("(#X56,#Y4)"), "(XArray[56],YArray[4])");
        EXPECT_EQ(replacer.Replace("(#X56,#Y4,#Z1)"), "(XArray[56],YArray[4],#Z1)");
    }

    TEST(SymbolReplacerTests, VerifyInvalidSymbolSyntax)
    {
        SymbolReplacer replacer{ "#" };
        replacer.AddHandler("", [](long n) {return "T:" + std::to_string(n); });

        EXPECT_EQ(replacer.Replace("#"), "#");
        EXPECT_EQ(replacer.Replace("#NAN"), "#NAN");
    }

    TEST(SymbolReplacerTests, VerifyStreamSymbolReplacer)
    {
        Services::ScopedServiceProvider provider;
        auto service = MockSymbolReplacerService::CreateMockService();

        std::string text = "(#X56, #Y4, #Z1)";
        std::stringstream strm(text);

        char result[80];
        service->TryReplaceStream(strm).getline(result,80);
        std::string resultStr(result);

        EXPECT_EQ(std::string(result), "(XArray[56], YArray[4], #Z1)");
    }

    TEST(SymbolReplacerTests, CheckSymbolReplacerQuotationMemory)
    {
        Services::ScopedServiceProvider provider;
        auto service = MockSymbolReplacerService::CreateMockService();
        StreamSymbolReplacer strmReplacer(service->GetSymbolReplacer(), 10);

        std::string text = "\"This is text\" #X101";
        std::stringstream strm(text);

        char resultStr[30];
        strmReplacer.TryReplace(strm).read(resultStr, 30);

        EXPECT_EQ(std::string(resultStr), "\"This is text\" XArray[101]");
    }

    TEST(SymbolReplacerTests, StreamSymbolReplacer_BufferSplitSymbolNumber)
    {
        Services::ScopedServiceProvider provider;
        auto service = MockSymbolReplacerService::CreateMockService();
        StreamSymbolReplacer strmReplacer(service->GetSymbolReplacer(), 10);

        std::string text = "                 #X1111111";
        std::stringstream strm(text);

        std::string resultStr;
        auto& result = strmReplacer.TryReplace(strm);
        std::getline(result, resultStr);

        EXPECT_EQ(resultStr, "                 XArray[1111111]");
    }

    TEST(SymbolReplacerTests, StreamSymbolReplacer_NoSymbolCheck)
    {
        Services::ScopedServiceProvider provider;
        auto service = MockSymbolReplacerService::CreateMockService();
        StreamSymbolReplacer strmReplacer(service->GetSymbolReplacer(), 10);

        std::string text = "                 1111111";
        std::stringstream strm(text);

        std::string resultStr;
        auto& result = strmReplacer.TryReplace(strm);
        std::getline(result, resultStr);

        EXPECT_EQ(resultStr, "                 1111111");
    }

}}}