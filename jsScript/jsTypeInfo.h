#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#pragma warning(disable : 4786)
#include <jsScript/jsScript_config.h>
#include <Reflection/Members/ArgumentList.h>
#include <Reflection/Members/Arg.h>
#include <string>
#include <vector>
#include <memory>

struct jsTypeInfoImpl;
struct jsArgInfoImpl;
class jsValue;

class JSSCRIPT_IMPORT_EXPORT jsArgInfo
{
public:
    jsArgInfo(const std::string& name);
    virtual ~jsArgInfo();

    virtual void document(const std::string& varName, const std::string& varDoc, size_t index = 0);
    virtual void document(const std::string& varName, size_t index = 0);
    virtual void varName(const std::string& varName, size_t index = 0);
    virtual void varDoc(const std::string& varDoc, size_t index = 0);
    virtual void setName(const std::string& name);

    std::string name() const;
    std::string varName(size_t index = 0) const;
    size_t varNameCount() const;
    std::string docShort(size_t index = 0) const;

private:
    std::unique_ptr<jsArgInfoImpl> m_impl;
};

class JSSCRIPT_IMPORT_EXPORT jsExampleInfo
{
public:
    jsExampleInfo();
    jsExampleInfo(const std::string& title);

    void AddScript(const std::string& script);
    void AddComment(const std::string& comment);

    const std::string& getTitle() const
    {
        return m_title;
    }

    const std::string& getScript() const
    {
        return m_script;
    }

    const std::string& getComment() const
    {
        return m_comment;
    }

    bool IsSimple() const
    {
        return !m_title.empty() && m_script.empty() && m_comment.empty();
    }

private:
    std::string m_title;
    std::string m_script;
    std::string m_comment;
};

class JSSCRIPT_IMPORT_EXPORT jsTypeInfo : public DNVS::MoFa::TypeUtilities::IntrusiveClass<jsTypeInfo>
{
public:
    enum jsValueType
    {
        jsUnknown = 0,
        jsFunction = 1,
        jsPropertyGet = 2,
        jsPropertyPut = 4,
        jsConstructor = 8,
        jsMethods = 15,
        jsObject = 16,
        jsAll = 0xFFFF,
    };

    enum jsDocumentType
    {
        jsAutomatic,
        jsShort,
        jsVerbose,
        jsExample,
    };

    jsTypeInfo(jsValue* value, const std::string& name, int type = jsUnknown, int parameters = 0);
    virtual ~jsTypeInfo();

    jsTypeInfo& AddDocumentation(const std::string& doc);
    jsExampleInfo& AddExample(const std::string& example);

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<1, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<2, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<3, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<4, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<5, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<6, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<7, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<8, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<9, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<10, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<11, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);
        argsVector.push_back(args[10]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<12, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);
        argsVector.push_back(args[10]);
        argsVector.push_back(args[11]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<13, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);
        argsVector.push_back(args[10]);
        argsVector.push_back(args[11]);
        argsVector.push_back(args[12]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<14, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);
        argsVector.push_back(args[10]);
        argsVector.push_back(args[11]);
        argsVector.push_back(args[12]);
        argsVector.push_back(args[13]);

        return AddSignatureFromArguments(argsVector);
    }

    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<15, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);
        argsVector.push_back(args[10]);
        argsVector.push_back(args[11]);
        argsVector.push_back(args[12]);
        argsVector.push_back(args[13]);
        argsVector.push_back(args[14]);

        return AddSignatureFromArguments(argsVector);
    }
    
    template <typename Derived>
    jsTypeInfo& AddSignature(const DNVS::MoFa::Reflection::Members::ArgumentList<16, Derived>& args)
    {
        std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer> argsVector;
        argsVector.push_back(args[0]);
        argsVector.push_back(args[1]);
        argsVector.push_back(args[2]);
        argsVector.push_back(args[3]);
        argsVector.push_back(args[4]);
        argsVector.push_back(args[5]);
        argsVector.push_back(args[6]);
        argsVector.push_back(args[7]);
        argsVector.push_back(args[8]);
        argsVector.push_back(args[9]);
        argsVector.push_back(args[10]);
        argsVector.push_back(args[11]);
        argsVector.push_back(args[12]);
        argsVector.push_back(args[13]);
        argsVector.push_back(args[14]);
        argsVector.push_back(args[15]);

        return AddSignatureFromArguments(argsVector);
    }

    void arguments(size_t nargs);
    size_t arguments() const;

    void setType(int type);
    int type() const;

    std::string name() const;
    std::vector<jsExampleInfo> getExamples() const;

    // Return various documentation strings
    std::string documentation(jsDocumentType docType = jsAutomatic) const;
    std::string format(bool sortable = false);

    void nameArguments(const std::string& argNames);
    jsArgInfo* returned(const std::string& name = "");
    jsArgInfo* argument(size_t index, const std::string& name = "");

    jsValue* value();

    const std::string& nameSpace() const;

    static const std::string& getNameSpaceStatic();
    static void setNameSpaceStatic(std::string const& ns);

private:
    void document(const std::string& text, jsDocumentType docType = jsAutomatic);
    void document(const std::string& text, const std::string& argNames);

    void one_liner(const std::string& text)
    {
        document(text, jsShort);
    }
    void one_liner(const std::string& text, const std::string& argNames);

    void description(const std::string& text)
    {
        document(text, jsVerbose);
    }

    void example(const std::string& text)
    {
        document(text, jsExample);
    }

    jsTypeInfo& AddSignatureFromArguments(
        const std::vector<DNVS::MoFa::Reflection::Members::ArgumentInfoPointer>& args);

    std::unique_ptr<jsTypeInfoImpl> m_impl;
};

class jsNameSpace
{
public:
    jsNameSpace(const std::string& ns) : m_backup(jsTypeInfo::getNameSpaceStatic())
    {
        jsTypeInfo::setNameSpaceStatic(ns);
    }

    ~jsNameSpace()
    {
        jsTypeInfo::setNameSpaceStatic(m_backup);
    }

private:
    std::string m_backup;
};
