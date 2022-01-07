//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructorsFromTypeExtracter.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Attributes/DeprecatedAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/AttributeCollectionService.h"
#include "Reflection/Attributes/DefaultMemberAttribute.h"
#include "Reflection/Attributes/ListAsConstructorAttribute.h"
#include "MemberItem.h"
#include "Reflection/Attributes/SerializationAttribute.h"
#include "ConstructorSelector.h"
#include "CallableConstructor.h"
#include "../TypeLibraries/ExtendedTypeTraits.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    

    ConstructorsFromTypeExtracter::ConstructorsFromTypeExtracter(TypeLibraries::TypeLibraryPointer typeLibrary)
        : m_typeLibrary(typeLibrary)
    {

    }

    ConstructorsFromTypeExtracter::~ConstructorsFromTypeExtracter()
    {

    }

    std::set<Members::MemberPointer> ConstructorsFromTypeExtracter::CollectAllConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo, bool allowAll) const
    {
        std::set<Members::MemberPointer> members;
        if (!m_typeLibrary)
            return members;

        for (auto typePair : m_typeLibrary->GetAllTypes())
        {
            for (auto memberPair : typePair.second->GetAllMembers())
            {
                for (Members::MemberPointer overload : memberPair.second->GetOverloads())
                {
                    if (IsConstructorMember(overload, decoratedTypeInfo, allowAll))
                        members.insert(overload);
                }
            }
        }
        return members;
    }

    std::set<Members::MemberPointer> ConstructorsFromTypeExtracter::CollectAllSerializationConstructorMembersOfThisAndDerivedClasses(const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        std::set<Members::MemberPointer> members;
        if (!m_typeLibrary)
            return members;

        for (auto typePair : m_typeLibrary->GetAllTypes())
        {
            if (typePair.second)
            {
                for (auto memberPair : typePair.second->GetAllMembers())
                {
                    for (Members::MemberPointer overload : memberPair.second->GetOverloads())
                    {
                        if (IsSerializationConstructorMember(overload, decoratedTypeInfo))
                            members.insert(overload);
                    }
                }
            }
        }
        return members;
    }

    Members::MemberPointer ConstructorsFromTypeExtracter::GetBestMemberFromType(const std::set<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        std::list<Members::MemberPointer> legalMembers = GetLegalMembers(members, decoratedTypeInfo);
        if (legalMembers.size() == 1)
            return *legalMembers.begin();
        std::list<Members::MemberPointer> nonDeprecatedMembers = GetNonDeprecatedMembers(legalMembers);
        if (nonDeprecatedMembers.size() == 1)
            return *nonDeprecatedMembers.begin();
        std::list<Members::MemberPointer> concreteMembers = GetMembersOfConcreteType(nonDeprecatedMembers, decoratedTypeInfo);
        if (concreteMembers.size() == 1)
            return *concreteMembers.begin();
        std::list<Members::MemberPointer> deducibleMembers = GetDeducibleMembers(concreteMembers);
        if (deducibleMembers.size() == 1)
            return *deducibleMembers.begin();
        std::list<Members::MemberPointer> membersWithMoreThanZeroArguments = GetMembersWithMoreThanZeroArguments(deducibleMembers);
        if (membersWithMoreThanZeroArguments.size() == 1)
            return *membersWithMoreThanZeroArguments.begin();
        std::list<Members::MemberPointer> defaultMembers = GetDefaultMembers(membersWithMoreThanZeroArguments);
        if (defaultMembers.size() == 1)
            return *defaultMembers.begin();
        std::list<Members::MemberPointer> membersThatMatchesTypeExact = GetMembersWithSameReturnType(defaultMembers, decoratedTypeInfo);
        if (membersThatMatchesTypeExact.size() == 1)
            return *membersThatMatchesTypeExact.begin();
        return nullptr;
    }

    Members::MemberPointer ConstructorsFromTypeExtracter::GetBestMemberFromItem(const Objects::Object& object, const std::set<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        if (object.IsConvertibleTo<TypeLibraries::TypePointer>())
        {
            TypeLibraries::TypePointer type = object.As<TypeLibraries::TypePointer>();
            return GetBestMemberFromType(members, Types::DecoratedTypeInfo(type->GetType(), decoratedTypeInfo.GetDecoration()));
        }
        return Utilities::ConstructorSelector(object).GetBestConstructor(members).GetMember();
    }

    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetMembersOfConcreteType(const std::list<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        std::list<Members::MemberPointer> concreteMembers;
        for (auto member : members)
        {
            if (member->GetReturnType().GetTypeInfo() == decoratedTypeInfo.GetTypeInfo())
                concreteMembers.emplace_back(member);
        }
        return concreteMembers;
    }

    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetNonDeprecatedMembers(const std::list<Members::MemberPointer>& members) const
    {
        std::list<Members::MemberPointer> nonDeprecatedMembers;
        for (auto member : members)
        {
            if (!Attributes::GetPointerToAttributeOrNull<Attributes::DeprecatedAttribute>(member))
                nonDeprecatedMembers.push_back(member);
        }
        return nonDeprecatedMembers;
    }
    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetLegalMembers(const std::set<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        std::list<Members::MemberPointer> legalMembers;
        for (auto member : members)
        {
            auto conversion = m_typeLibrary->GetConversionGraph()->GetConversionSequence(member->GetReturnType(), decoratedTypeInfo);
            if (conversion && conversion->IsValid())
                legalMembers.emplace_back(member);
        }
        return legalMembers;
    }

    bool ConstructorsFromTypeExtracter::IsConstructorMember(Members::MemberPointer overload, const Types::DecoratedTypeInfo& decoratedTypeInfo, bool allowAll) const
    {
        if (!IsConstructorMember(overload, allowAll))
            return false;
		if(TypeLibraries::ExtendedTypeTraits(m_typeLibrary).IsBaseOf(decoratedTypeInfo.GetTypeInfo(), overload->GetReturnType().GetTypeInfo()))
        {
            return true;
        }
        return false;
    }

    bool ConstructorsFromTypeExtracter::IsConstructorMember(Members::MemberPointer overload, bool allowAll) const
    {
        if (!overload)
            return false;
        //Ignore deprecated attributes.
        if (overload->GetAttributeCollection().HasAttribute<Attributes::DeprecatedAttribute>())
            return false;
        //ListAsConstructorAttribute allows non-constructors to be included, and allows constructors to be ignored.
        else if (overload->GetAttributeCollection().HasAttribute<Attributes::ListAsConstructorAttribute>())
        {
            switch (overload->GetAttributeCollection().GetAttribute<Attributes::ListAsConstructorAttribute>().GetOption())
            {
            case Attributes::ListAsConstructorAttribute::ListNever:
                return false;
            case Attributes::ListAsConstructorAttribute::ListTopLevel:
                if (!allowAll)
                    return false;
            }
        }
        //By default, don't accept non-constructors here.
        else if (overload->GetMemberType() != Members::MemberType::TypeConstructor)
            return false;
        if (!Utilities::MemberItem(overload, m_typeLibrary).AreConstructorArgumentsNamed())
            return false;
        return true;
    }

    bool ConstructorsFromTypeExtracter::IsSerializationConstructorMember(Members::MemberPointer overload, const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        //Only consider serialization attributes.
        if (!overload->GetAttributeCollection().HasAttribute<Attributes::SerializationAttribute>())
            return false;
        //By default, don't accept non-constructors here.
        if (overload->GetMemberType() != Members::MemberType::TypeConstructor)
            return false;
        if (!Utilities::MemberItem(overload, m_typeLibrary).AreConstructorArgumentsNamed())
            return false;

        auto conversionSequence = m_typeLibrary->GetConversionGraph()->GetConversionSequence(overload->GetReturnType(), decoratedTypeInfo);
        if (conversionSequence && conversionSequence->IsValid() && conversionSequence->Quality() < TypeConversions::ArgumentConversionQuality(TypeConversions::ConversionType::Type::UserConversion))
        {
            return true;
        }
        return false;
    }

    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetDeducibleMembers(const std::list<Members::MemberPointer>& members) const
    {
        std::list<Members::MemberPointer> deducibleMembers;
        for (auto member : members)
        {
            if (MemberItem(member, m_typeLibrary).AreConstructorArgumentBoundToObjectProperties())
                deducibleMembers.emplace_back(member);
        }
        return deducibleMembers;
    }

    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetMembersWithMoreThanZeroArguments(const std::list<Members::MemberPointer>& members) const
    {
        std::list<Members::MemberPointer> result;
        for (auto member : members)
        {
            if(member->GetArity() > 0)
                result.emplace_back(member);
        }
        return result;
    }

    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetDefaultMembers(const std::list<Members::MemberPointer>& members) const
    {
        std::list<Members::MemberPointer> defaultMembers;
        for (const auto& member : members)
        {
            if (member->GetAttributeCollection().HasAttribute<Attributes::DefaultMemberAttribute>())
                defaultMembers.emplace_back(member);
        }
        return defaultMembers;
    }

    std::list<Members::MemberPointer> ConstructorsFromTypeExtracter::GetMembersWithSameReturnType(const std::list<Members::MemberPointer>& members, const Types::DecoratedTypeInfo& decoratedTypeInfo) const
    {
        std::list<Members::MemberPointer> membersWithCorrectReturnType;
        for(const auto& member : members)
        {
            if (member->GetReturnType().GetTypeInfo() == decoratedTypeInfo.GetTypeInfo())
                membersWithCorrectReturnType.push_back(member);
        }
        return membersWithCorrectReturnType;
    }

}}}}