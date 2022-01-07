#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"

#include "TypeUtilities/Tribool.h"
#include "Reflection/Members/IMember.h"
#include "HorizontalElements.h"
#include "VerticalElements.h"
#include "ClonableLayoutPointer.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    namespace Detail {
        struct CanAddAndRemoveRows {};
    }
    constexpr Detail::CanAddAndRemoveRows CanAddAndRemoveRows;
    
    class Grid : public TypedLayoutElement<Grid, ControlElement>
    {
    public:
        Grid(const Grid&) = default;
        Grid(Grid&&) = default;
        Grid& operator=(const Grid&) = default;
        Grid& operator=(Grid&&) = default;

        Grid(const AutoControl& other) 
            : TypedLayoutElement<Grid, ControlElement>(other) 
            , m_canAddRemoveRows(false) 
            , m_initialNumberOfRows(6) 
        {}
		
		Grid()
			: TypedLayoutElement<Grid, ControlElement>("")
			, m_canAddRemoveRows(false)
			, m_initialNumberOfRows(6)
		{}

		template<typename... Args>
		explicit Grid(int initialNumberOfRows, const Args&... args)
			: Grid(args...)
		{
			m_initialNumberOfRows = initialNumberOfRows;
		}

		template<typename... Args>
		explicit Grid(TypeUtilities::Tribool hasSorting, const Args&... args)
			: Grid(args...)
		{
			m_hasSorting = hasSorting;
		}

		template<typename... Args>
		explicit Grid(Detail::CanAddAndRemoveRows canAddRemoveRows, const Args&... args)
			: Grid(args...)
		{
			m_canAddRemoveRows = true;
		}

		template<typename... Args>
		Grid(const std::string& name, const std::string& caption, const Args&... args)
			: Grid(args...)
		{
			SetName(name);
			SetCaption(caption);
		}

		template<typename... Args>
		Grid(const char* name, const std::string& caption, const Args&... args)
			: Grid(args...)
		{
			SetName(name);
			SetCaption(caption);
		}

		template<typename... Args>
		Grid(const std::string& name, const char* caption, const Args&... args)
			: Grid(args...)
		{
			SetName(name);
			SetCaption(caption);
		}

		template<typename... Args>
		Grid(const char* name, const char* caption, const Args&... args)
			: Grid(args...)
		{
			SetName(name);
			SetCaption(caption);
		}

		template<typename... Args>
		explicit Grid(const std::string& name, const Args&... args)
			: Grid(args...)
		{
			SetName(name);
		}

		template<typename... Args>
		explicit Grid(const char* name, const Args&... args)
			: Grid(args...)
		{
			SetName(name);
		}

		template<typename... Args>
		explicit Grid(const HorizontalElements& header, const Args&... args)
			: Grid(args...)
		{
			m_header = header;
		}

		template<typename... Args>
		explicit Grid(const VerticalElements& fixedGrid, const Args&... args)
			: Grid(args...)
		{
			m_fixedGrid = std::make_shared<VerticalElements>(fixedGrid);
		}

		size_t GetInitialNumberOfRows() const { return m_initialNumberOfRows; }
        const HorizontalElements& GetHeader() const { return m_header; }
        TypeUtilities::Tribool HasSorting() const { return m_hasSorting; }

        void SetHeader(const HorizontalElements& header) { m_header = header; }
        void SetInitialNumberOfRows(size_t initialNumberOfRows) { m_initialNumberOfRows = initialNumberOfRows; }
        void SetHasSorting(TypeUtilities::Tribool hasSorting) { m_hasSorting = hasSorting; }

        const std::shared_ptr<VerticalElements>& GetFixedGrid() const { return m_fixedGrid; }
        void SetFixedGrid(const std::shared_ptr<VerticalElements>& fixedGrid) { m_fixedGrid = fixedGrid; }
        bool GetCanAddRemoveRows() const { return m_canAddRemoveRows; }
        void SetCanAddRemoveRows(bool value) { m_canAddRemoveRows = value; }
		template<typename AttributeT, typename... Args>
		Grid& AddAttribute(Args&&... args)
		{
			GetOrCreateAttributes()->AddAttribute(AttributeT(std::forward<Args>(args)...));
			return *this;
		}
	private:
        bool m_canAddRemoveRows;
        size_t m_initialNumberOfRows;
        HorizontalElements m_header;
        ClonableLayoutPointer<VerticalElements> m_fixedGrid;
        TypeUtilities::Tribool m_hasSorting;
    };

}}}}
