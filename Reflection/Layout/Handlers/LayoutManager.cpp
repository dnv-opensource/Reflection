//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LayoutManager.h"
#include "LayoutBoolHandler.h"
#include "LayoutFallbackHandler.h"
#include "LayoutEnumHandler.h"
#include "LayoutEnumerableAttributeHandler.h"
#include "LayoutTreatAsBoolAttributeHandler.h"
#include "LayoutGroupAttributeHandler.h"
#include "LayoutImageAttributeHandler.h"
#include "LayoutLayoutAttributeHandler.h"
#include "LayoutDefaultChildHandler.h"
#include "LayoutEliminateReuseHandler.h"
#include "LayoutMarkAddedElementsHandler.h"
#include "LayoutControlElementHandler.h"
#include "LayoutNestedLayoutAttributeHandler.h"
#include "LayoutGroupMergeHandler.h"
#include "LayoutVerticalElementsMergeHandler.h"
#include "LayoutEmbeddedElementHandler.h"
#include "LayoutDefaultOverloadSelectorHandler.h"
#include "LayoutOverloadSelectorHandler.h"
#include "LayoutEliminateStaticPropertyHandler.h"
#include "LayoutNameAttributeHandler.h"
#include "LayoutGridHandler.h"
#include "LayoutUtilityClassAttributeHandler.h"
#include "LayoutVerticalElementsGroupCreatorHandler.h"
#include "LayoutTabAttributeHandler.h"
#include "LayoutGroupEnablerAttributeHandler.h"
#include "LayoutFunctionSelectorAttributeHandler.h"
#include "LayoutEliminateAliasNodeHandler.h"
#include "LayoutRecordAttributeHandler.h"
#include "LayoutEliminateStaticEmbeddedElementsHandler.h"
#include "LayoutGridElementsMergerHandler.h"
#include "LayoutLambdaPropertyAspectHandler.h"
#include "LayoutRemoveVerticalElementFromGroupBoxHandler.h"
#include "LayoutSymmetryAttributeHandler.h"
#include "LayoutIgnoreSymmetryCheckBoxHandler.h"
#include "LayoutHeaderAspectHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    LayoutManager::LayoutManager(bool addDefaultHandlers)
    {
        if (!addDefaultHandlers)
            return;
        AddHandler(new LayoutEliminateReuseHandler);
        AddHandler(new LayoutIgnoreSymmetryCheckBoxHandler);
        AddHandler(new LayoutLayoutAttributeHandler);
        AddHandler(new LayoutEliminateStaticPropertyHandler);
        AddHandler(new LayoutEliminateAliasNodeHandler);
        //If we have a FunctionSelectorAttribute, it will govern the layout of the OverloadSelector.
        AddHandler(new LayoutFunctionSelectorAttributeHandler);
        //If LayoutAttribute did not add OverloadSelector, try to see if we need to add a default OverloadSelector instead.
        AddHandler(new LayoutDefaultOverloadSelectorHandler);
        AddHandler(new LayoutOverloadSelectorHandler);
        AddHandler(new LayoutNestedLayoutAttributeHandler);
        AddHandler(new LayoutNameAttributeHandler);
        AddHandler(new LayoutUtilityClassAttributeHandler);
        AddHandler(new LayoutDefaultChildHandler);
        //All bool values treated as check boxes by default
        AddHandler(new LayoutBoolHandler);
        //Treat enums as check box if they have the TreatAsBoolAttribute
        AddHandler(new LayoutTreatAsBoolAttributeHandler);
        //All enums treated as radio buttons by default
        AddHandler(new LayoutEnumHandler);
        //All nodes with EnumerableAttrte treated as ComboBox by default
        AddHandler(new LayoutEnumerableAttributeHandler);
        //Handle grids and set up columns etc.
        AddHandler(new LayoutGridHandler);
		AddHandler(new LayoutHeaderAspectHandler);
		//Uses the RecordAttribute to convert the layout into a fixed grid.
        AddHandler(new LayoutRecordAttributeHandler);
        //Ignore unnamed nodes with LayoutLambdaPropertyAspectHandler and add their children instead.
        AddHandler(new LayoutLambdaPropertyAspectHandler);
        //If nothing else fits, treat item as TextBox
        AddHandler(new LayoutFallbackHandler);
        //Handle the image attribute here
        AddHandler(new LayoutImageAttributeHandler);
        //If handler has a group attribute, add a group box.
        AddHandler(new LayoutGroupAttributeHandler);
        //Attaches predefined control elements to the correct control node.
        AddHandler(new LayoutControlElementHandler);
        AddHandler(new LayoutMarkAddedElementsHandler);
        AddHandler(new LayoutRemoveVerticalElementFromGroupBoxHandler);
        //Merge nested vertical elements
        AddHandler(new LayoutVerticalElementsMergeHandler);
        //Merge groups with the same name. This may move around the layout a bit.
        AddHandler(new LayoutGroupMergeHandler);
        //Handle Symmetry attribute - add Enabler check box to group box.
        AddHandler(new LayoutSymmetryAttributeHandler);
        AddHandler(new LayoutGroupEnablerAttributeHandler);
        AddHandler(new LayoutEmbeddedElementHandler);
        AddHandler(new LayoutEliminateStaticEmbeddedElementsHandler);
        AddHandler(new LayoutGridElementsMergerHandler);
        //Handle tab attribute. Allows embedding properties into tabs
        AddHandler(new LayoutTabAttributeHandler);
        AddHandler(new LayoutVerticalElementsGroupCreatorHandler);
    }

    std::shared_ptr<ILayoutElement> LayoutManager::ModifyLayout(std::shared_ptr<ILayoutElement> element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
    {
        if (!element)
            return nullptr;
        for (auto& handler : m_handlers)
            element = handler->ModifyLayout(element, controlNode, context);
        return element;
    }

    std::shared_ptr<ILayoutElementGroupCreator> LayoutManager::CreateGroupCreator(const std::shared_ptr<Controls::ControlNode>& controlNode)
    {
        for (auto& handler : m_handlers)
        {            
            if (auto creator = handler->CreateGroupCreator(controlNode))
                return creator;
        }
        return nullptr;
    }

    void LayoutManager::AddHandler(ILayoutHandler* handler)
    {
        m_handlers.emplace_back(std::move(handler));
    }

}}}}

