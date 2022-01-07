#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"
#include "Reflection/Attributes/FunctionCallbackAttribute.h"
#include "TypeUtilities/IsCallable.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    Customize what is scripted in GUI when pressing OK or when changing the content of an element.
    */
    enum class ScriptExecutionMode {
        Defered = 0, //Script executed when the user presses Ok or apply in a dialog.
        Immediate = 1, //Scripted every time the user changes the value of this element.
    };

    class ScriptExpressionAttribute 
        : public IAttribute
        , public FunctionCallbackAttribute<std::list<std::string>>
    {
    private:
        ScriptExpressionAttribute() 
        : m_mode(ScriptExecutionMode::Defered)
        {}
    public:
        template<typename... Args>
        ScriptExpressionAttribute(ScriptExecutionMode mode, Args&&... args)
            : ScriptExpressionAttribute(std::forward<Args>(args)...)
        {
            m_mode = mode;
        }
        //Last argument is required to be convertible to void(const std::string&)
        template<typename Callback,typename... Args, typename = std::enable_if_t<TypeUtilities::IsCallable<Callback>>>
        ScriptExpressionAttribute(Callback callback, Args&&... args)      
            : ScriptExpressionAttribute()
        {
            InitializeFunction(this, callback, std::forward<Args>(args)...);
        }
        template<typename Callback, typename... Args, typename = std::enable_if_t<TypeUtilities::IsCallable<Callback>>>
        ScriptExpressionAttribute(const TypeLibraries::TypeLibraryPointer& typeLibrary, Callback callback, Args&&... args)
            : ScriptExpressionAttribute(std::forward<Args>(args)...)
        {
            InitializeFunction(this, typeLibrary, callback, std::forward<Args>(args)...);
        }
        void InitializeArguments() {}
        template<typename... Args>
        void InitializeArguments(ScriptExecutionMode mode, Args&&... args) {
            m_mode = mode;
            InitializeArguments(std::forward<Args>(args)...);
        }
        ScriptExecutionMode GetMode() const { return m_mode; }
    private:
        ScriptExecutionMode m_mode;
    };
}}}}
