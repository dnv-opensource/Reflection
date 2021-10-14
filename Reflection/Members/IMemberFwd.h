#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "boost\smart_ptr\intrusive_ptr.hpp"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    class IMember;
    typedef boost::intrusive_ptr<IMember> MemberPointer;
}}}}