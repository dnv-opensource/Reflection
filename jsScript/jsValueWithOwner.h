#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsScript_config.h"
#include "jsValue.h"

class JSSCRIPT_IMPORT_EXPORT jsValueWithOwner : public jsValue 
{
public:
    static jsValue* Wrap(jsValue* value, jsValue* owner);
    jsValueWithOwner(jsValue* value, jsValue* owner);

    virtual jsValue* lookup(const std::string& identifier,jsValue* owner=NULL) override;
    virtual jsValue* duplicate(jsValue* owner=NULL) override;
    virtual jsValue* assign(jsValue* owner,jsValue* op2) override;

    virtual std::string typeName() override;
    jsValue* GetValue() const { return m_value; }
private:
    mofa::ref<jsValue> m_value;
    mofa::ref<jsValue> m_owner;
};
