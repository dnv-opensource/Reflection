#pragma once
#include <string>

namespace DNVS {namespace MoFa {namespace Services {
    class INameManglingService {
    public:
        virtual ~INameManglingService() {}
        virtual std::string MangleName(const std::string& input) const = 0;
        virtual std::string UnmangleName(const std::string& input) const = 0;
    };
}}}
