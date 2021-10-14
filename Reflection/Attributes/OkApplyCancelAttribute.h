#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    namespace Impl {
        struct Invisible {};
        struct Visible {
            Invisible operator!() const { return Invisible(); }
        };
        struct Disabled {};
        struct Enabled {
            Disabled operator!() const { return Disabled(); }
        };
    }
    constexpr Impl::Invisible Invisible;
    constexpr Impl::Disabled Disabled;
    constexpr Impl::Visible Visible;
    constexpr Impl::Enabled Enabled;
    class ButtonState {
    public:
        ButtonState()
            : m_isEnabled(true)
            , m_isVisible(true)
        {}
        template<typename... Args>
        ButtonState(Impl::Invisible, const Args&... args)
            : ButtonState(args...)
        {
            m_isVisible = false;
        }
        template<typename... Args>
        ButtonState(Impl::Visible, const Args&... args)
            : ButtonState(args...)
        {
            m_isVisible = true;
        }    
        template<typename... Args>
        ButtonState(Impl::Disabled, const Args&... args)
            : ButtonState(args...)
        {
            m_isEnabled = false;
        }
        template<typename... Args>
        ButtonState(Impl::Enabled, const Args&... args)
            : ButtonState(args...)
        {
            m_isEnabled = true;
        }
        template<typename... Args>
        ButtonState(const std::string& nameOverride, const Args&... args)
            : ButtonState(args...)
        {
            m_nameOverride = nameOverride;
        }
        bool IsEnabled() const { return m_isEnabled; }
        bool IsVisible() const { return m_isVisible; }
        const std::string& GetNameOverride() const { return m_nameOverride; }
    private:
        bool m_isEnabled;
        bool m_isVisible;
        std::string m_nameOverride;
    };
    class Ok : public ButtonState
    {
    public:
        template<typename... Args>
        Ok(const Args&... args)
            : ButtonState(args...)
        {}
    };
    class Apply : public ButtonState
    {
    public:
        template<typename... Args>
        Apply(const Args&... args)
            : ButtonState(args...)
        {}
    };   
    class Cancel : public ButtonState
    {
    public:
        template<typename... Args>
        Cancel(const Args&... args)
            : ButtonState(args...)
        {}
    };   
    class OkApplyCancelAttribute
    {
    public:
        OkApplyCancelAttribute() {}
        template<typename... Args>
        OkApplyCancelAttribute(const Ok& ok, const Args&... args)
            : OkApplyCancelAttribute(args...)
        {
            m_ok = ok;
        }
        template<typename... Args>
        OkApplyCancelAttribute(const Apply& apply, const Args&... args)
            : OkApplyCancelAttribute(args...)
        {
            m_apply = apply;
        }
        template<typename... Args>
        OkApplyCancelAttribute(const Cancel& cancel, const Args&... args)
            : OkApplyCancelAttribute(args...)
        {
            m_cancel = cancel;
        }
        const Ok& GetOk() const { return m_ok; }
        const Apply& GetApply() const { return m_apply; }
        const Cancel& GetCancel() const { return m_cancel; }
    private:
        Ok m_ok;
        Apply m_apply;
        Cancel m_cancel;
    };

}}}}
