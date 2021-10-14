#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/DecoratedTypeInfo.h"
#include "Aspects/IControlAspect.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include <type_traits>
#include <vector>
#include "Reflection/Members/IMember.h"
#include "Reflection/Contents/Content.h"
#include "Reflection/Attributes/AttributeCollectionService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class ControlNodeManager;
    class FunctionOverload;
    /*
    ControlNode represents a reflection control tree.
    Example:
    Given the following constructor:
    Vector3d(Length x, Length y, Length z)
    This is the tree that will be produced:
    ContolNode root -> {Type = Vector3d, Name = "Vector3d", Aspects = {ConstuctorAspect(Vector3d(Length,Length,Length)), TypeAspect(Vector3d))}
    Children:
        "x" -> {Type = Length, Name = "X", Aspects = {FunctionArgumentAspect(Vector3d(Length,Length,Length), 0), PropertyAspect(Vector3d::X), TypeAspect(Length))}
        "y" -> {Type = Length, Name = "Y", Aspects = {FunctionArgumentAspect(Vector3d(Length,Length,Length), 1), PropertyAspect(Vector3d::Y), TypeAspect(Length))}
        "z" -> {Type = Length, Name = "Z", Aspects = {FunctionArgumentAspect(Vector3d(Length,Length,Length), 2), PropertyAspect(Vector3d::Z), TypeAspect(Length))}
    
    You can pass representations of objects, such as Content("Vector3d(1,2,3)") through methods on the control node in order to extract corresponding values for child nodes.
    */    
    namespace Detail {
        struct NoInit {};
    }
    constexpr Detail::NoInit NoInit;
    class REFLECTION_IMPORT_EXPORT ControlNode
    {
    public:
        ControlNode(const ControlNode&) = delete;
        ControlNode(ControlNode&&) = delete;
        //Constructs an empty root node.
        ControlNode(const std::shared_ptr<ControlNodeManager>& manager);
        //Constructs a root node based on a type.
        ControlNode(const std::shared_ptr<ControlNodeManager>& manager, const Types::DecoratedTypeInfo& decoratedTypeInfo);
        //Constructs a root node based on a member pointer.
        ControlNode(const std::shared_ptr<ControlNodeManager>& manager, const Members::MemberPointer& member);
        //Constructs an empty root node.
        ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary);
        //Constructs a root node based on a type.
        ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& decoratedTypeInfo);
        //Constructs a root node based on a type - no initialization of child nodes.
        ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& decoratedTypeInfo, Detail::NoInit);

        //Constructs a root node based on a member - no initialization of child nodes.
        ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, Detail::NoInit);
        //Constructs a root node that supports operations given the functions in functionOverload or other attributes.
        template<typename... AttributesT>
        ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& decoratedTypeInfo, AttributesT&&... attributes)
            : ControlNode(typeLibrary)
        {
            Attributes::AttributeCollection collection;
            HandleAttributes(collection, std::forward<AttributesT>(attributes)...);
            if (!collection.IsEmpty())
                SetCustomAttributeCollection(std::move(collection));
            InitializeType(decoratedTypeInfo);
        }
        //Constructs a root node based on a member pointer.
        ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member);

        //Constructs a child node.
        ControlNode(ControlNode* parent);
        ControlNode(ControlNode* parent, const Members::MemberPointer& member);
        ControlNode(ControlNode* parent, const Types::DecoratedTypeInfo& decoratedTypeInfo);
        template<typename... AttributesT>
        ControlNode(ControlNode* parent, const Types::DecoratedTypeInfo& decoratedTypeInfo, AttributesT&&... attributes)
            : ControlNode(parent, decoratedTypeInfo)
        {
            Attributes::AttributeCollection collection;
            HandleAttributes(collection, std::forward<AttributesT>(attributes)...);
            if(!collection.IsEmpty())
                SetCustomAttributeCollection(std::move(collection));
        }

        bool operator ==(const ControlNode& other) const;
        bool operator !=(const ControlNode& other) const;
        std::string ToString() const;
        
        const Types::DecoratedTypeInfo& GetDecoratedTypeInfo() const;
        void SetDecoratedTypeInfo(const Types::DecoratedTypeInfo& val);

        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& val);

        //The control manager is shared among all control nodes of a tree.
        const std::shared_ptr<ControlNodeManager>& GetManager() const;
        TypeLibraries::TypeLibraryPointer GetTypeLibrary() const;
        ControlNode* GetParent() const;
        void SetParent(ControlNode* node);
        
        std::string GetCaption() const;
        bool HasCaption() const;
        void SetCaption(const std::string& caption);
       
        //Aspects, such as type, property, function argument, constructor.
        template<typename T, typename... Args>
        void AddAspect(Args&&... args) 
        { 
            for (auto& oldAspect : m_aspects)
            {
                if (typeid(*oldAspect) == typeid(T))
                {
                    oldAspect.reset(new T(std::forward<Args>(args)...));
                    return;
                }
            }
            m_aspects.emplace_front(std::unique_ptr<IControlAspect>(new T(std::forward<Args>(args)...)));
        }
        template<typename T>
        void RemoveAspect()
        {
            for (auto it = m_aspects.begin(); it != m_aspects.end(); ++it)
            {
                if (typeid(T) == typeid(**it))
                {
                    m_aspects.erase(it);
                    return;
                }
            }
        }
        const std::list<std::unique_ptr<IControlAspect>>& GetAspects() const { return m_aspects; }
        template<typename T>
        const T* TryGetAspect() const
        {
            for (const auto& aspect : m_aspects)
            {
                if (const T* ptr = dynamic_cast<const T*>(aspect.get()))
                    return ptr;
            }
            return nullptr;
        }

        template<typename T>
        const T* TryGetAttribute() const
        {
            for (const auto& aspect : m_aspects)
                if (aspect->GetAttributeCollection().HasAttribute<T>())
                    return &aspect->GetAttributeCollection().GetAttribute<T>();
            return nullptr;
        }
        template<typename T>
        const T& GetAspect() const {
            const T* attribute = TryGetAspect<T>();
            if (attribute)
                return *attribute;
            else
                throw std::runtime_error("Node does not have attribute of type " + std::string(typeid(T).name()) + ".");
        }
        //Children
        void AddChild(std::shared_ptr<ControlNode> node);
        bool HasChild(const std::string& name) const;
        std::shared_ptr<ControlNode> GetChild(const std::string& name) const;
        std::shared_ptr<ControlNode> LookupRelatedNode(const std::string& name, int recursionLevelParent = 1, int recursionLevelChild = 1) const;
        std::pair<ControlNode*, Members::MemberPointer> LookupRelatedMember(const std::string& name, int recursionLevelParent = 1, int recursionLevelChild = 1);
        const std::vector<std::shared_ptr<ControlNode>>& GetChildren() const { return m_children; }

        //Iterate over aspects to try to deduce content
        Contents::Content TryGetPropertyContent(const Contents::Content& parent, bool trySimplify = true) const;
        Contents::Content TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value);
        bool SupportUpdateParent() const;
        bool SupportAssign() const;
        bool HasMember() const;
        void SetCustomAttributeCollection(Attributes::AttributeCollection&& attributeCollection);
        void InitializeType(const Types::DecoratedTypeInfo& decoratedTypeInfo, bool finalizeInitialization = true);
        void InitializeMember(const Members::MemberPointer& member);
        void Initialize();
    private:
        void HandleAttributes(Attributes::AttributeCollection& collection)
        {}
        template<typename... AttributesT>
        void HandleAttributes(Attributes::AttributeCollection& collection, const char* name, AttributesT&&... attributes)
        {
            m_name = name;
            HandleAttributes(collection, std::forward<AttributesT>(attributes)...);
        }
        template<typename... AttributesT>
        void HandleAttributes(Attributes::AttributeCollection& collection, const std::string& name, AttributesT&&... attributes)
        {
            m_name = name;
            HandleAttributes(collection, std::forward<AttributesT>(attributes)...);
        }
        template<typename AttributeT, typename... AttributesT>
        void HandleAttributes(Attributes::AttributeCollection& collection, AttributeT&& attribute, AttributesT&&... attributes)
        {
            collection.AddAttribute(std::forward<AttributeT>(attribute));
            HandleAttributes(collection, std::forward<AttributesT>(attributes)...);
        }
        //Name of this node - typically property name or function name
        std::string m_name;
        //Caption of this node.
        std::string m_caption;
        //Type of this node. Typically the type of a property or the return value of a constructor.
        Types::DecoratedTypeInfo m_decoratedTypeInfo;
        //A collection of attributes. The attributes typically connects this node to the reflection library through member pointers, such as PropertyAspect, which says that this node represents a property.
        std::list<std::unique_ptr<IControlAspect>> m_aspects;
        //Immediate children. The ordering of these children controls the creation of the layout.
        std::vector<std::shared_ptr<ControlNode>> m_children;
        //The global type library.
        
        ControlNode* m_parent;
        std::shared_ptr<ControlNodeManager> m_manager;
    };

    template<class _E, class _Tr>
    inline std::basic_ostream<_E, _Tr>& __cdecl operator<<(std::basic_ostream<_E, _Tr>& _O, const ControlNode& node) {
        return _O << node.ToString();
    }

}}}}
