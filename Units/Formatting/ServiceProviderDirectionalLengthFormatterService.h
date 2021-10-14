#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IDirectionalLengthFormatterService.h"
#include <memory>
#include "../Config.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Formatting {
#pragma warning(push)
#pragma warning(disable:4275)
    class UNITS_IMPORT_EXPORT ServiceProviderDirectionalLengthFormatterService : public IDirectionalLengthFormatterService
    {
    public:
        ServiceProviderDirectionalLengthFormatterService(std::shared_ptr<IDirectionalLengthFormatterService> fallback);
        std::string Format(const LengthX& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const override;
        std::string Format(const LengthY& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const override;
        std::string Format(const LengthZ& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const override;
    private:
        std::shared_ptr<IDirectionalLengthFormatterService> m_fallback;
    };
#pragma warning(pop)
}}}}