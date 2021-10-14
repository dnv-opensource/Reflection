//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeSaveFileAttributeHandler.h"
#include "Reflection/Attributes/DefaultAttribute.h"
#include "Reflection/Controls/Aspects/IsAliasAspect.h"
#include "Reflection/Controls/Aspects/FunctionAspect.h"
#include "Reflection/Controls/Aspects/FileNameAspect.h"
#include "Reflection/Controls/Aspects/TypeAspect.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeSaveFileAttributeHandler::OnInitialize(ControlNode& node, const Attributes::SaveFileAttribute& saveFileAttribute, bool priorState)
    {
        if (!saveFileAttribute.GetJournallingRequired())
        {
            auto journalled = std::make_shared<ControlNode>(&node, Types::TypeId<bool>(), Attributes::DefaultAttribute(Attributes::StoreDefault, false));
            journalled->SetName("journalled");
            journalled->SetCaption("Journal export operation");
            node.AddChild(journalled);
        }
        auto extension = std::make_shared<ControlNode>(&node, Types::TypeId<std::string>(), "Extension");
        //This is an alias of the element found in the file dialog template
        extension->AddAspect<IsAliasAspect>();
        node.AddChild(extension);

        auto initialDir = std::make_shared<ControlNode>(&node, Types::TypeId<std::string>(), "InitialDir");
        initialDir->AddAspect<IsAliasAspect>();
        node.AddChild(initialDir);

        const FunctionAspect& functionAspect = node.GetAspect<FunctionAspect>();
        if (functionAspect.GetFunction()->GetArity() != 2)
            throw std::runtime_error("Method " + functionAspect.GetFunction()->GetActualName() + " must have one argument, it has " + std::to_string(functionAspect.GetFunction()->GetArity()-1));
        std::string name = functionAspect.GetFunction()->GetArgumentInfo(1)->GetName();
        if (name.empty())
            throw std::runtime_error("In the reflection code for " + node.GetAspect<TypeAspect>().GetType()->GetName() + ", you need to call .AddSignature for the method " + functionAspect.GetFunction()->GetActualName() + ".");
        auto fileName = node.GetChild(name);
        if (!fileName)
            throw std::runtime_error("Unable to detect fileName element '" + name + "' for method " + functionAspect.GetFunction()->GetActualName());
            
        fileName->AddAspect<IsAliasAspect>();

        node.AddAspect<FileNameAspect>(fileName);

        //Allow the object to be default constructed.
        auto constructors = Utilities::ConstructorsFromTypeService::GetService(node.GetTypeLibrary())->CollectAllConstructorMembersOfThisAndDerivedClasses(node.GetDecoratedTypeInfo(), true);
        if (constructors.size() == 1)
            node.GetManager()->OnInitializeMember(node, *constructors.begin());
        return priorState;
    }

}}}}

