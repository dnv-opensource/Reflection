#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <functional>
#include "Reflection/Types/DecoratedTypeInfo.h"
#include <vector>
#include "Reflection/Variants/Variant.h"
#include "FunctionCallbackAttribute.h"
#include "Reflection/Config.h"
#include "IAttribute.h"
namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class REFLECTION_IMPORT_EXPORT IRecordRange 
    {
    public:
        virtual ~IRecordRange() {}
        //The name of the argument of the function we refer to
        virtual std::string GetName() const = 0;
        //Returns all indices in the range given an object of the right type
        virtual std::vector<Variants::Variant> GetIndicesInRange(const Variants::Variant& object) const = 0;
        virtual std::vector<std::string> GetHeadersInRange(const Variants::Variant& object) const = 0;
    };

    class REFLECTION_IMPORT_EXPORT IndexRange : public IRecordRange
    {
    public:
        IndexRange(size_t lowerIndex, size_t upperIndex, const std::string& indexName);
        template<typename... HeaderNames>
        IndexRange(size_t lowerIndex, size_t upperIndex, const std::string& indexName, HeaderNames&&... names)
            : IndexRange(lowerIndex, upperIndex, indexName)
        {
            if (sizeof...(names) != m_upperIndex - m_lowerIndex + 1)
                throw std::runtime_error("number of header names must match the number of indices given.");
            InitializeHeaderNames(std::forward<HeaderNames>(names)...);
        }
        virtual std::string GetName() const;
        //Returns all indices in the range given an object of the right type
        virtual std::vector<Variants::Variant> GetIndicesInRange(const Variants::Variant& object) const;
        virtual std::vector<std::string> GetHeadersInRange(const Variants::Variant& object) const;
    private:
        void InitializeHeaderNames();
        template<typename... HeaderNames>
        void InitializeHeaderNames(const std::string& headerName, HeaderNames&&... names)
        {
            m_headerNames.push_back(headerName);
            InitializeHeaderNames(std::forward<HeaderNames>(names)...);
        }
        size_t m_lowerIndex;
        size_t m_upperIndex;
        std::string m_indexName;
        std::vector<std::string> m_headerNames;
    };
    
    class REFLECTION_IMPORT_EXPORT PropertyAlias
    {
    public:
        template<typename... Indices>
        PropertyAlias(const std::string& propertyName,Indices&&... indices)
            : m_propertyName(propertyName)
        {
            InitializeIndices(std::forward<Indices>(indices)...);
        }
        const std::string& GetPropertyName() const;
        const std::vector<Variants::Variant>& GetIndices() const;
        bool IsAliasOf(const std::string& name) const;
    private:
        void InitializeIndices();
        template<typename Index,typename... Indices>
        void InitializeIndices(Index&& index, Indices&&... indices)
        {
            m_indices.push_back(Variants::VariantService::Reflect(std::forward<Index>(index)));
            InitializeIndices(std::forward<Indices>(indices)...);
        }
        std::string m_propertyName;
        std::vector<Variants::Variant> m_indices;
    };

    class RecordTypeCallback : public FunctionCallbackAttribute<Types::DecoratedTypeInfo>
    {
    public:
        template<typename Callback, typename... StringArguments>
        RecordTypeCallback(Callback callback, StringArguments&&... stringArguments)
            : FunctionCallbackAttribute<Types::DecoratedTypeInfo>(callback, std::forward<StringArguments>(stringArguments)...)
        {
        }
    };

    namespace Impl {
        class MatrixDiagonalSymmetric {};
    }
    constexpr Impl::MatrixDiagonalSymmetric MatrixDiagonalSymmetric;
    class RecordAttribute : public IAttribute {
    public:
        template<typename... Arguments>
        RecordAttribute(const std::string& getterFunction, Arguments&&... arguments)
            : m_getterFunction(getterFunction)
            , m_isSymmetric(false)
        {
            InitializeArguments(std::forward<Arguments>(arguments)...);
        }
        const std::string& GetGetterFunction() const { return m_getterFunction; }
        const std::vector<std::unique_ptr<IRecordRange>>& GetRanges() const { return m_ranges; }
        const std::unique_ptr<RecordTypeCallback>& GetRecordTypeCallback() const { return m_recordTypeCallback; }
        const std::vector<PropertyAlias>& GetAliases() const { return m_aliases; }
        bool IsSymmetric() const { return m_isSymmetric; }
    private:
        void InitializeArguments() {}

        template<typename RangeType, typename... Arguments>
        std::enable_if_t<std::is_base_of_v<IRecordRange, RangeType>> InitializeArguments(const RangeType& range, Arguments&&... arguments) 
        {
            m_ranges.emplace_back(std::make_unique<RangeType>(range));
            InitializeArguments(std::forward<Arguments>(arguments)...);
        }
        template<typename... Arguments>
        void InitializeArguments(const RecordTypeCallback& callback, Arguments&&... arguments)
        {
            m_recordTypeCallback = std::make_unique<RecordTypeCallback>(callback);
            InitializeArguments(std::forward<Arguments>(arguments)...);
        }        
        template<typename... Arguments>
        void InitializeArguments(const PropertyAlias& alias, Arguments&&... arguments)
        {
            m_aliases.push_back(alias);
            InitializeArguments(std::forward<Arguments>(arguments)...);
        }
        template<typename... Arguments>
        void InitializeArguments(const Impl::MatrixDiagonalSymmetric& alias, Arguments&&... arguments)
        {
            m_isSymmetric = true;
            InitializeArguments(std::forward<Arguments>(arguments)...);
        }
        std::string m_getterFunction;
        std::unique_ptr<RecordTypeCallback> m_recordTypeCallback;
        std::vector<std::unique_ptr<IRecordRange>> m_ranges;
        std::vector<PropertyAlias> m_aliases;
        bool m_isSymmetric;
    };

}}}}
