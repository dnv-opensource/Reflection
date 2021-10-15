#pragma once
#include "Plugins\ISubsystem.h"
#include <list>
#include <string>
#include "Plugins\IOrdered.h"

namespace DNVS { namespace MoFa { namespace Plugins { namespace Test {
    class IFakeSubsystem 
        :   public ISubsystem
        ,   public IOrdered
    {
    public:
    };
    class FakeSubsystem : public IFakeSubsystem
    {
    public:
        FakeSubsystem(const std::string& name, const std::list<std::string>& prerequisites = std::list<std::string>())
            : m_name(name)
            , m_prerequisites(prerequisites)
        {}
        virtual std::string GetName() const { return m_name; }
        virtual std::list<std::string> GetPrerequisites() const { return m_prerequisites; }
    private:
        std::string m_name;
        std::list<std::string> m_prerequisites;
    };
}}}}
