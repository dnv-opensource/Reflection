//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeManager.h"
#include "Handlers/ControlNodeExpandMemberAttributeHandler.h"
#include "Handlers/ControlNodeRecordAttributeHandler.h"
#include "Handlers/ControlNodeIncludeInViewAttributeHandler.h"
#include "Handlers/ControlNodeCaptionAttributeHandler.h"
#include "Handlers/ControlNodePropertyHandler.h"
#include "Handlers/ControlNodeTypeHandler.h"
#include "Handlers/ControlNodeConstructorHandler.h"
#include "Handlers/ControlNodeRootHandler.h"
#include "Handlers/ControlNodeConstructorSelectorAttributeHandler.h"
#include "Handlers/ControlNodeFunctionSelectorAttributeHandler.h"
#include "Handlers/ControlNodeFunctionHandler.h"
#include "Handlers/ControlNodeUtilityClassAttributeHandler.h"
#include "Handlers/ControlNodeDeprecatedAttributeHandler.h"
#include "Handlers/ControlNodeContextAttributeHandler.h"
#include "Handlers/ControlNodeSymmetryAttributeHandler.h"
#include "Handlers/ControlNodeContainerAttributeHandler.h"
#include "Handlers/ControlNodeLayoutControlElementHandler.h"
#include "Handlers/ControlNodeAutoDetectConstructorHandler.h"
#include "Handlers/ControlNodeLayoutOverloadSelectorHandler.h"
#include "Handlers/ControlNodeLayoutEmbeddedElementHandler.h"
#include "Handlers/ControlNodeLayoutSelectorHandler.h"
#include "Handlers/ControlNodeNameAttributeHandler.h"
#include "Handlers/ControlNodeNoConstructorsHandler.h"
#include "Handlers/ControlNodeGroupEnablerAttributeHandler.h"
#include "Handlers/ControlNodeLayoutGroupBoxHandler.h"
#include "Handlers/ControlNodeGroupAttributeHandler.h"
#include "Handlers/ControlNodeLayoutDropdownButtonHandler.h"
#include "Handlers/ControlNodeFileAttributeHandler.h"
#include "Handlers/ControlNodeLambdaPropertyAspectHandler.h"
#include "Handlers/ControlNodeLayoutSelectorLookupHandler.h"
#include "Handlers/ControlNodeHeaderAspectHandler.h"
#include "Handlers/ControlNodeBindingAttributeHandler.h"
#include "Handlers/ControlNodeTupleAttributeHandler.h"
#include "Handlers/ControlNodeModifyExistingAttributeHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    ControlNodeManager::ControlNodeManager(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        : m_typeLibrary(typeLibrary)
    {
        m_handlers.emplace_back(new ControlNodeLambdaPropertyAspectHandler);
        //Eliminate deprecated nodes.
        m_handlers.emplace_back(new ControlNodeDeprecatedAttributeHandler);
        //Handles FunctionSelectorAttribute
        m_handlers.emplace_back(new ControlNodeFunctionSelectorAttributeHandler);
        //Handles property members and adds the PropertyAspect
        m_handlers.emplace_back(new ControlNodePropertyHandler);
        //Handles constructor members and adds the ConstructorAspect 
        m_handlers.emplace_back(new ControlNodeConstructorHandler);
        //Handles symmetry attributes by adding the SymmetryAspect to this node.
        m_handlers.emplace_back(new ControlNodeSymmetryAttributeHandler);
        //Handles the NameAttribute - Sets the name __this__ to the current node.
        m_handlers.emplace_back(new ControlNodeNameAttributeHandler);
        //Handles OverloadSelector in LayoutAttribute. Adds ConstructorSelectorAspect
        m_handlers.emplace_back(new ControlNodeLayoutOverloadSelectorHandler);
        //Handles EmbeddedElement in LayoutAttribute. Adds ConstructorAspect to the node
        m_handlers.emplace_back(new ControlNodeLayoutEmbeddedElementHandler);
        //Handles a Selector layout element
        m_handlers.emplace_back(new ControlNodeLayoutSelectorHandler);
        //Handles SelectorLookup
        m_handlers.emplace_back(new ControlNodeLayoutSelectorLookupHandler);
        //Handles ExpandMemberAttribute and adds ConstructorSelectorAspect.
        m_handlers.emplace_back(new ControlNodeExpandMemberAttributeHandler);
        //Adds default initialization of the type of the node
        m_handlers.emplace_back(new ControlNodeTypeHandler);
        //Handles ControlElements in LayoutAttribute.
        m_handlers.emplace_back(new ControlNodeLayoutControlElementHandler);
        //Handles a DropdownButton layout element - Adds the ExpandActionAspect.
        m_handlers.emplace_back(new ControlNodeLayoutDropdownButtonHandler);
        //Handles group boxes - Adds a GroupAspect if you have a named GroupBox in your layout.
        m_handlers.emplace_back(new ControlNodeLayoutGroupBoxHandler);
        //If no type is given from node, sets the type from the BindingAttribute
		m_handlers.emplace_back(new ControlNodeBindingAttributeHandler);
        //Handles GroupAttribute - Adds a GroupAspect if you have a named GroupBox in your layout.
        m_handlers.emplace_back(new ControlNodeGroupAttributeHandler);
        //Handles GroupEnablerAttribyte - Adds a GroupEnablerAspect. Only works if you have a GroupAspect on your node.
        m_handlers.emplace_back(new ControlNodeGroupEnablerAttributeHandler);
        //Handles the RecordAttribute on a given member - Adds new child nodes for each of the records referenced from this handler.
        m_handlers.emplace_back(new ControlNodeRecordAttributeHandler);
        //Fallback - for all other functions - add support for calling that function. Adds a FunctionAspect and adds child arguments.
        m_handlers.emplace_back(new ControlNodeFunctionHandler);
        //Handles the FileAttribute.
        m_handlers.emplace_back(new ControlNodeFileAttributeHandler);
        //Handles the CaptionAttribute - Sets a caption on the node
        m_handlers.emplace_back(new ControlNodeCaptionAttributeHandler);
        //Adds the RootAspect to the root onde (the one without parents)
        m_handlers.emplace_back(new ControlNodeRootHandler);
		//Adds support for tuples.
		m_handlers.emplace_back(new ControlNodeTupleAttributeHandler);
		m_handlers.emplace_back(new ControlNodeHeaderAspectHandler);
        //Handles the ConstructorSelectorAttribute - this will reduce the number of constructors that are shown in the dialog.
        m_handlers.emplace_back(new ControlNodeConstructorSelectorAttributeHandler);
        //Handles the IncludeInViewAttribute - adds extra members as children of this node in addition to constructor arguments.
        m_handlers.emplace_back(new ControlNodeIncludeInViewAttributeHandler);
        //Handles the ContextAttribute - adds all members with the same ContextAttribute as is stored on this node.
        m_handlers.emplace_back(new ControlNodeContextAttributeHandler);
        //Handles the UtilityClassAttribute - Add child nodes for all members on this class
        m_handlers.emplace_back(new ControlNodeUtilityClassAttributeHandler);
        //Handles the case when a class has no public constructors - Adds child nodes for all members on this class.
        m_handlers.emplace_back(new ControlNoConstructorsHandler);
        //Handles the ContainerAttribute - adds a ContainerAspect to this node
        m_handlers.emplace_back(new ControlNodeContainerAttributeHandler);
        //Fallback - Automatically detects a constructor for this type. If a single constructor is found - it will be added to this class.
        m_handlers.emplace_back(new ControlNodeAutoDetectConstructorHandler);
        m_handlers.emplace_back(new ControlNodeModifyExistingAttributeHandler);
    }
    
    bool ControlNodeManager::OnInitialize(ControlNode& node, bool priorState)
    {
        for (auto& handler : m_handlers)
            priorState = handler->OnInitialize(node, priorState);
        return priorState;
    }

    bool ControlNodeManager::OnInitializeMember(ControlNode& node, const Members::MemberPointer& member)
    {
        for (auto& handler : m_handlers)
            if (handler->OnInitializeMember(node, member))
                return true;
        return false;
    }

    void ControlNodeManager::OnInitializeType(ControlNode& node, const Types::DecoratedTypeInfo& decoratedTypeInfo)
    {
        for (auto& handler : m_handlers)
            handler->OnInitializeType(node, decoratedTypeInfo);
    }

    const TypeLibraries::TypeLibraryPointer& ControlNodeManager::GetTypeLibrary() const
    {
        return m_typeLibrary;
    }

	void ControlNodeManager::AddHandler(IControlNodeHandler* handler)
	{
		m_handlers.emplace_back(handler);
	}

}}}}

