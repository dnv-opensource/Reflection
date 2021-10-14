#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Handlers/IControlNodeHandler.h"
#include <vector>
#include <memory>
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Stores all control node handlers and manages initialization of a node by calling all the handlers initialization code in sequence.
    class ControlNodeManager : public IControlNodeHandler
    {
    public:
        ControlNodeManager(const TypeLibraries::TypeLibraryPointer& typeLibrary);
        bool OnInitialize(ControlNode& node, bool priorState = false) override;
        bool OnInitializeMember(ControlNode& node, const Members::MemberPointer& member) override;
        void OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& decoratedTypeInfo) override;
        const TypeLibraries::TypeLibraryPointer& GetTypeLibrary() const;
		void AddHandler(IControlNodeHandler* handler);
    private:
        std::vector<std::unique_ptr<IControlNodeHandler>> m_handlers;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };
}}}}