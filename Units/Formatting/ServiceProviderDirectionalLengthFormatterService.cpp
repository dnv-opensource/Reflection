//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ServiceProviderDirectionalLengthFormatterService.h"
#include "Services/ServiceProvider.h"
#include "DefaultDirectionalLengthFormatterService.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Formatting {

    ServiceProviderDirectionalLengthFormatterService::ServiceProviderDirectionalLengthFormatterService(std::shared_ptr<IDirectionalLengthFormatterService> fallback)
        : m_fallback(fallback)
    {
        if (!m_fallback)
            m_fallback = std::make_shared<DefaultDirectionalLengthFormatterService>();
    }

    std::string ServiceProviderDirectionalLengthFormatterService::Format(const LengthX& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        auto service = Services::ServiceProvider::Instance().TryGetService<IDirectionalLengthFormatterService>();
        if (service)
            return service->Format(length, formattingService);
        else
            return m_fallback->Format(length, formattingService);
    }

    std::string ServiceProviderDirectionalLengthFormatterService::Format(const LengthY& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        auto service = Services::ServiceProvider::Instance().TryGetService<IDirectionalLengthFormatterService>();
        if (service)
            return service->Format(length, formattingService);
        else
            return m_fallback->Format(length, formattingService);
    }

    std::string ServiceProviderDirectionalLengthFormatterService::Format(const LengthZ& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        auto service = Services::ServiceProvider::Instance().TryGetService<IDirectionalLengthFormatterService>();
        if (service)
            return service->Format(length, formattingService);
        else
            return m_fallback->Format(length, formattingService);
    }

}}}}


