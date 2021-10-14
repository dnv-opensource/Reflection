#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>
#include <memory>
#include <typeindex>
#include "Config.h"
#include <type_traits>

namespace DNVS { namespace MoFa { namespace Formatting {
    class IFormatter;
    class FORMATTING_IMPORT_EXPORT FormattingService {
    public:
        template<typename FormatterInterface, typename ConcreteFormatter>
        void AddFormatter(const std::shared_ptr<ConcreteFormatter>& concreteFormatter)
        {
            static_assert(std::is_base_of_v<FormatterInterface, ConcreteFormatter>, "FormatterInterface must be a base class of ConcreteFormatter");
            static_assert(std::is_base_of_v<IFormatter, FormatterInterface>, "IFormatter must be a base class of FormatterInterface");
            m_formatters[typeid(FormatterInterface)] = concreteFormatter;
        }
        template<typename FormatterInterface, typename ConcreteFormatter>
        std::enable_if_t<std::is_base_of_v<FormatterInterface, ConcreteFormatter>> AddFormatter(const ConcreteFormatter& concreteFormatter)
        {
            static_assert(std::is_base_of_v<IFormatter, FormatterInterface>, "IFormatter must be a base class of FormatterInterface");
            m_formatters[typeid(FormatterInterface)] = std::make_shared<ConcreteFormatter>(concreteFormatter);
        }

        template<typename T>
        std::shared_ptr<T> GetFormatter() const {
            auto it = m_formatters.find(typeid(T));
            if (it != m_formatters.end())
                return std::static_pointer_cast<T>(it->second);
            else
                return nullptr;
        }
        template<typename T, typename DefaultT>
        std::shared_ptr<T> GetFormatterOrDefault() const {
            auto formatter = GetFormatter<T>();
            if (formatter)
                return formatter;
            return std::make_shared<DefaultT>();
        }
    private:
        std::map<std::type_index, std::shared_ptr<IFormatter>> m_formatters;
    };
}}}