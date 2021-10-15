//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsAutoCompletionService.h"
#include "Reflection\Objects\Object.h"
#include "Scripting\Scripting.h"



jsAutoCompletionService::jsAutoCompletionService(std::shared_ptr<DNVS::MoFa::Scripting::IAutoCompletionContext> context)
    : m_context(context)
{
}

std::shared_ptr<jsAutoCompletion> jsAutoCompletionService::TryAutocomplete(const std::string& str, bool functionCompletion)
{
    std::shared_ptr<jsAutoCompletion> autoCompletion;
    try
    {
        int completeText = -1;
        int i = static_cast<int>(str.size() - 1);
        i = findIdentifierStart(str, i);

        if (i < (int)str.size())
        {
            completeText = i;
            i--;
        }

        using DNVS::MoFa::Reflection::Objects::Object;
        Object function;

        bool inFunction = false;
        bool inMethod = false;
        int startFunction = -1;
        int endFunction = -1;

        if (i >= (int)str.size())
            i = (int)(str.size() - 1);

        int startIndex = i;

        while (i >= 0)
        {
            if (str[i] == '.' && i > 0)
            {
                int index = --i;
                i = parseMember(str, index);

                if (i > index)
                    i = index;

                else if (!inMethod)
                {
                    startIndex = index + 1;
                    inMethod = false;
                }
            }
            else if (inFunction)
            {
                try
                {
                    startFunction = i--;

                    if (startFunction > endFunction)
                        function = Object();
                    else
                    {
                        int j = endFunction;

                        while (str[j] != '.' && j > startFunction)
                            j--;

                        if (str[j] == '.')
                            j--;

                        try
                        {
                            if (j >= startFunction)
                            {
                                m_context->SetNestedContext(DNVS::MoFa::Scripting::Test(str.substr(startFunction, j - startFunction + 1)));
                                function = m_context->lookup(str.substr(j + 2, endFunction - (j + 2) + 1));
                            }
                        }
                        catch (...)
                        {
                        }

                        if (!function.IsValid())
                        {
                            std::string fn = str.substr(startFunction, endFunction - startFunction + 1);
                            function = DNVS::MoFa::Scripting::Test(fn);
                        }                            
                        break;
                    }
                }
                catch (...)
                {
                }
            }
            else if (!functionCompletion && str[i] == ' ')
                --i;
            else if (!functionCompletion && str[i] == '(')
                break;
            else if (!autoCompletion)
            {
                try
                {
                    if (i > startIndex)
                        m_context->SetGlobalContext();
                    else
                        m_context->SetNestedContext(DNVS::MoFa::Scripting::Test(str.substr(i, startIndex - i)));
                }
                catch (...)
                {
                }

                autoCompletion = std::make_shared<jsAutoCompletion>(str, completeText);
            }
            else
            {
                if (str[i] == ')')
                {
                    while (i > 0 && str[i] != '(')
                        i--;
                    if (i > 0 && str[i] == '(')
                        i--;
                }

                if (str[i] == '(' && i > 0)
                {
                    inFunction = true;
                    endFunction = --i;
                    startIndex = endFunction;
                    i = parseMember(str, endFunction);
                }

                else if (str[i] == ';')
                    break;
                else
                    --i;
            }
        }

        if (!autoCompletion)
            autoCompletion = std::make_shared<jsAutoCompletion>(str, completeText);

        // Extract variable names
        if (!m_context->HasContext())
            m_context->SetGlobalContext();
        if (!m_context->IsGlobalContext() || completeText >= 0)
            m_context->CollectMembers(autoCompletion);
        if (function.IsValid())
        {
            m_context->CollectFunctions(function, autoCompletion);
            autoCompletion->setFunction(startFunction);
        }
    }
    catch (std::exception)
    {
        return nullptr;
    }
    return autoCompletion;
}

bool jsAutoCompletionService::isIdentifierStart(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool jsAutoCompletionService::isIdentifierPart(char c)
{
    return isIdentifierStart(c) || (c >= '0' && c <= '9');
}

int jsAutoCompletionService::findIdentifierStart(const std::string& str, int index)
{
    while (index >= 0 && isIdentifierPart(str[index]))
        --index;

    while (++index <= (int)str.size() && index < (int)str.size() && !isIdentifierStart(str[index]));

    return index;
}

int jsAutoCompletionService::parseMember(const std::string& str, int index)
{
    if (isIdentifierPart(str[index]))
    {
        int i = findIdentifierStart(str, index);

        if (i > 1 && (i < index || (i == index && isIdentifierStart(str[i]))))
        {
            if (str[i - 1] == '.')
            {
                int i2 = parseMember(str, i - 2);

                if (i2 < i - 1)
                    return i2;
            }
        }

        return i;
    }
    else if (str[index] == ')')
    {
        int pCount = 1;
        int i = index - 1;

        while (pCount > 0 && i >= 0)
        {
            if (str[i] == ')')
                pCount++;
            else if (str[i] == '(')
                pCount--;
            i--;
        }

        index = i;

        while (i >= 0 && str[i] == ' ')
            i--;

        i = findIdentifierStart(str, i);

        if (i > 1 && (i < index || (i == index && isIdentifierStart(str[i]))))
        {
            if (str[i - 1] == '.')
            {
                int i2 = parseMember(str, i - 2);

                if (i2 < i - 1)
                    return i2;
            }
        }
        return i;
    }
    else if (str[index] == ']')
    {
        int pCount = 1;
        int i = index - 1;

        while (pCount > 0 && i >= 0)
        {
            if (str[i] == ']')
                pCount++;
            else if (str[i] == '[')
                pCount--;

            i--;
        }
        if (i > 1 && i < index)
        {
            if (str[i - 1] == '.')
            {
                int i2 = parseMember(str, i - 2);

                if (i2 < i - 1)
                    return i2;
            }
        }
        return i;
    }
    else
        return index;
}
