#pragma warning(disable:4345)
#include "gtest\gtest.h"
#include "Services\ScopedServiceProvider.h"
namespace DNVS { namespace MoFa { namespace Services { namespace Tests {
    class IDummyService {};
    TEST(ServiceProviderTests, GetServicePriorToInitializationThrows)
    {
        ScopedServiceProvider provider;
        EXPECT_THROW(ServiceProvider::Instance().GetService<IDummyService>(),std::runtime_error);
    }

    TEST(ServiceProviderTests, RegisterSessionService_DiesWhenSessionDies)
    {
        ScopedServiceProvider permanent(IServiceContainer::Permanent);
        {
            ScopedServiceProvider session(IServiceContainer::Session);
            ServiceProvider::Instance().RegisterService(std::make_shared<IDummyService>());
            EXPECT_NO_THROW(ServiceProvider::Instance().GetService<IDummyService>());
        }
        EXPECT_THROW(ServiceProvider::Instance().GetService<IDummyService>(),std::runtime_error);
    }
    TEST(ServiceProviderTests, RegisterPermanentService_LivesWhenSessionDies)
    {
        ScopedServiceProvider permanent(IServiceContainer::Permanent);
        {
            ScopedServiceProvider session(IServiceContainer::Session);
            ServiceProvider::Instance().RegisterService(std::make_shared<IDummyService>(), IServiceContainer::Permanent);
            EXPECT_NO_THROW(ServiceProvider::Instance().GetService<IDummyService>());
        }
        EXPECT_NO_THROW(ServiceProvider::Instance().GetService<IDummyService>());
    }

}}}}
