//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsScript/jsDocumentation.h>
#include <jsScript/jsScriptable.h>
#include <jsScript/jsConstructor.h>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include "boost/lexical_cast.hpp"
#include "jsTypeLibrary.h"
#include "jsModelObject.h"
#include "jsVTBL.h"

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

jsDocumentation::jsDocumentation(jsTypeLibrary& typeLibrary,
    const std::string& path,
    const std::string& programName,
    const std::string& version,
    const std::string& versionDate)
    : m_title(programName)
    , m_directory(path)
    , m_typeLibrary(typeLibrary)
    , m_version(version)
    , m_versionDate(versionDate)
    , m_typeMap(typeLibrary.GetReflectionTypeLibrary())
{
    addType(m_typeLibrary.lookup(typeid(jsModelObject)));
    m_typeMap.CreateReflectionTypes();

    auto type = m_typeMap.GetOrCreateFromPrototype(m_typeLibrary.lookup(typeid(jsModelObject)));
    AddLeaves(type->GetMembers(DNVS::MoFa::Doc::IMember::Function), type);
    type = m_typeMap.GetOrCreateGlobalReflectionType();
    AddLeaves(type->GetMembers(DNVS::MoFa::Doc::IMember::Function), type);

    style(true);
    style(false);
    for (const auto& nameTypePair : m_typeMap.GetSortedTypes())
    {
        document(nameTypePair.second);//Document a class.
    }
    addIndex();
    addClassHierarchy();
}

jsDocumentation::jsDocumentation(jsTypeLibrary& typeLibrary)
    : m_typeLibrary(typeLibrary)
    , m_typeMap(typeLibrary.GetReflectionTypeLibrary())
{

}

void jsDocumentation::addType(jsValue* value)
{
    if (!value) return;
    if (m_typeMap.GetPrototypeToTypeMap().count(value) > 0 || !value->typeInfo())
    {
        addType(value->prototype());
        return;
    }
    jsTypeInfo* info = value->typeInfo();
    if (info->type()&jsTypeInfo::jsObject) {
        m_typeMap.GetOrCreateFromPrototype(value);
        jsValue::TypeInfoSet typeInfo;
        jsValue::TypeInfoSet::iterator it;
        value->getTypeInfo(typeInfo, jsTypeInfo::jsAll);
        for (it = typeInfo.begin(); it != typeInfo.end(); it++) {
            jsValue* value = (*it)->value();
            addType(value);
            addType(value->prototype());
        }
    }
    else if (info->type()&jsTypeInfo::jsMethods) {
        if (info->returned()) addType(m_typeLibrary.lookup(info->returned()->name()));
        for (size_t i = 0; i < info->arguments(); i++) {
            addType(m_typeLibrary.lookup(info->argument(i)->name()));
        }
    }
    addType(value->prototype());
}

jsDocumentation::~jsDocumentation()
{

}

void jsDocumentation::style(bool screen)
{
    string fileName = "";

    if (screen) {
        fileName = m_directory + "\\" + "jsDocumentation.css";
    }
    else {
        fileName = m_directory + "\\" + "print.css";
    }
    fstream out(fileName.c_str(), ios_base::out);
    out << "tr.header { " << endl;
    out << "  background: #006fad;" << endl;
    out << "	 color: #fff;" << endl;
    out << "	 font-size:1em;" << endl;
    out << "}" << endl;

    out << "div.explanation { " << endl;
    out << "  color: #073;" << endl;
    out << "  font-style: italic;" << endl;
    out << "  margin-left: 40px; " << endl;
    out << "}" << endl;

    out << "div.propertyStatus { " << endl;
    out << "  font-style: italic;" << endl;
    out << "}" << endl;

    out << "tr { " << endl;
    out << "	 border: solid 1px silver; " << endl;
    out << "  color: #666; " << endl;
    out << "  padding:5px; " << endl;
    out << "  font-size:9pt; " << endl;
    out << "}" << endl;

    out << "p.function { " << endl;
    out << "  background: #eee;" << endl;
    out << "  font-size: smaller;" << endl;
    out << "  width: 58%; " << endl;
    out << "  padding: 10px; " << endl;
    out << "}" << endl;

    out << "body { " << endl;
    out << "  padding-left: 20px; " << endl;
    out << "  float: left; " << endl;
    out << "}" << endl;

    out << "* {" << endl;
    out << "  font-family: arial; " << endl;
    out << "  line-height:1.0em; " << endl;
    out << "}" << endl;

    out << "a {" << endl;
    out << "  color:#006fad; " << endl;
    out << "  text-decoration:none; " << endl;
    out << "}" << endl;

    out << "a:hover {" << endl;
    out << "  text-decoration:underline; " << endl;
    out << "}" << endl;

    out << "tr.own {" << endl;
    out << "  border: solid 1px silver;" << endl;
    out << "  color:#666; " << endl;
    out << "  padding:5px; " << endl;
    out << "  font-size:9pt; " << endl;
    out << "}" << endl;

    out << "tr.inherited {" << endl;
    out << "  border: solid 1px silver;" << endl;
    out << "  color:#666; " << endl;
    out << "  padding:5px; " << endl;
    out << "  font-size:9pt; " << endl;
    out << "}" << endl;

    out << "td.functionsummary {" << endl;
    out << "}" << endl;

    out << "td.returntype {" << endl;
    out << "  font-size: smaller;" << endl;
    out << "}" << endl;

    if (screen) {
        out << "td.NavBarCell1 {" << endl;
        out << "  background: #eee;" << endl;
        out << "  font-weight: bold; " << endl;
        out << "  border:1px solid #fff; " << endl;
        out << "}" << endl;
        //       out << "td.NavBarCell2 {"<< endl;
        //       out << "  background: white;" << endl;
        //       out << "  font-size: smaller;"<< endl;
        //       out << "  font-weight: bold"<< endl;
        //       out << "}"<< endl;   
    }
    else {
        out << "td.NavBarCell1 {" << endl;
        out << "  background: #eee;" << endl;
        out << "  border:1px solid #fff; " << endl;
        out << "}" << endl;
        //       out << "td.NavBarCell2 {"<< endl;
        //       out << "  display : none"<< endl;
        //       out << "}"<< endl;
    }
    out << "td.progver {" << endl;
    out << "  background: #eee;" << endl;
    out << "  font-weight: bold;" << endl;
    out << "  border:1px solid #fff; " << endl;
    out << "}" << endl;

    out << "td.example {" << endl;
    //   out << "  border: solid thin;"<< endl;
    out << "  background: #eee;" << endl;
    out << "  margin-left: 3em;" << endl;
    out << "  width: 68%;" << endl;
    out << "  border:1px solid #fff; " << endl;
    out << "}" << endl;

    out << "td.illegalexample {" << endl;
    out << "  border: solid red;" << endl;
    out << "  background: #eee;" << endl;
    out << "  margin-left: 3em;" << endl;
    out << "  border:1px solid #fff; " << endl;
    out << "}" << endl;

    out << "dt {" << endl;
    out << "  font-weight: bold;" << endl;
    out << "}" << endl;

    out << "td.overviewexplanation {" << endl;
    out << "  text-align : left;" << endl;
    out << "  text-indent : 10em;" << endl;
    out << "  border:1px solid #fff; " << endl;
    out << "}" << endl;

    out << "td.overviewheader {" << endl;
    out << "  text-align : left;" << endl;
    out << "  border:1px solid #fff; " << endl;
    out << "}" << endl;

    out << "table.overviewexplanation {" << endl;
    out << " border-collapse:collapse;" << endl;
    out << "}" << endl;

    out << "table {" << endl;
    out << "width:990px; " << endl;
    out << "}" << endl;

    out << "table td{ " << endl;
    out << "font-size:1.1em; " << endl;
    out << "} " << endl;

    out << "table td, table th {" << endl;
    out << "  border-right:1px solid #fff; " << endl;
    out << "  border-left:1px solid #fff; " << endl;
    out << "  border-top:1px solid #fff; " << endl;
    out << "  border-bottom:1px solid #fff; " << endl;
    out << "  line-height:1.5em; " << endl;
    out << "}" << endl;

    out << "p{ " << endl;
    out << "}" << endl;

    out << "h1 { " << endl;
    out << "  font-size:1.5em; " << endl;
    out << "  color:#00334e; " << endl;
    out << "  font-weight:normal; " << endl;
    out << "  text-shadow:0px 1px 1px #fff; " << endl;
    out << "}" << endl;

    out << "h2 { " << endl;
    out << "color:#00334e; " << endl;
    out << "font-weight:normal; " << endl;
    out << "text-shadow:0px 1px 1px #fff; " << endl;
    out << "}" << endl;

    out << "h3 { " << endl;
    out << "  font-size:1em; " << endl;
    out << "}" << endl;

    out << "hr { " << endl;
    out << "  margin-top:20px; " << endl;
    out << "  border:1px dotted #eee; " << endl;
    out << "  width:640px; " << endl;
    out << "}" << endl;

}


void jsDocumentation::document(const DNVS::MoFa::Doc::TypePointer& type)
{
    if (m_documented.find(type->GetName()) != m_documented.end()) return;
    m_documented.insert(make_pair(type->GetName(), type));
    string fileName = m_directory + "\\" + type->GetName() + ".html";
    fstream out(fileName.c_str(), ios_base::out);

    addTitle(out, type);
    addBody(out, type);

    out << "</body>" << endl;
    out << "</html>" << endl;
}

void jsDocumentation::addTitle(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" http://www.w3.org/TR/html4/loose.dtd>" << endl;
    out << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << endl;
    out << "<head>" << endl;
    out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />" << endl;
    out << "<title>";
    out << m_title << " " << m_version << " " << m_versionDate;
    if (type) out << ": " << type->GetName() << endl;
    out << "</title>" << endl;
    out << "<link rel =\"stylesheet\" type=\"text/css\" href=\"jsDocumentation.css\" media=\"screen\">" << endl;
    out << "<link rel =\"stylesheet\" type=\"text/css\" href=\"print.css\" media=\"print\">" << endl;
    out << "<script type=\"text/javascript\" src=\"js/jquery-1.4.2.min.js\"></script> " << endl;
    out << "<script type=\"text/javascript\" src=\"js/genie.table.filter.min.js\"></script>" << endl;
    out << "<script type=\"text/javascript\" src=\"js/filterscript.js\"></script>" << endl;
    out << "</head>" << endl;
}

void jsDocumentation::addBody(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    out << "<body >" << endl;
    addNavBar(out);
    out << "<h1>Command: ";
    if (type) out << type->GetName() << endl;
    out << "</h1>" << endl;
    addClassData(out, type);
    addConstructorSummary(out, type);
    addFunctionSummary(out, type);
    addPropertySummary(out, type);
    addDetail(out, type);
}

void jsDocumentation::addDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    out << "<!-- ============ FIELD DETAIL =========== -->" << endl;
    out << endl;
    out << "<p>" << endl;
    addConstructorDetail(out, type);
    addFunctionDetail(out, type);
    addPropertyDetail(out, type);
}

void jsDocumentation::addNavBar(ostream& out)
{
    out << "<!-- ========== START OF NAVBAR ========== -->" << endl;
    out << "<a name=\"navbar_top\"><!-- --></a>" << endl;
    out << "<table border=\"0\" width=\"100%\" cellpadding=\"1\" cellspacing=\"0\">" << endl;
    out << "<tr>" << endl;
    out << "<td colspan=\"3\" class=\"NavBarCell1\">" << endl;
    out << "<a name=\"navbar_top_firstrow\"><!-- --></a>" << endl;
    out << "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"3\">" << endl;
    out << "  <tr valign=\"top\">" << endl;
    out << "  <td class=\"NavBarCell1\">" << endl;
    out << "<a href=\"index.html\" class=\"NavBarFont1\">Overview</a></td>" << endl;
    out << "  <td class=\"NavBarCell1\">" << endl;
    out << "<a href=\"ClassHierarchy.html\" class=\"NavBarFont1\">Class Hierarchy</a>&nbsp;</td>" << endl;
    out << "<td class=\"progver\">" << m_title << " " << m_version << " " << m_versionDate << " </td>";

    out << "  </tr>" << endl;
    out << "</table>" << endl;
    //    out << "<tr>" << endl;
    //    out << "<td class=\"NavBarCell2\">" << endl;
    //    out << "&nbsp;PREV&nbsp;" << endl;
    //    out << "&nbsp;NEXT</TD>" << endl;
    //    out << "<td class=\"NavBarCell2\">" << endl;
    //    out << "<a href=\"index.html\" target=\"_top\">FRAMES</a>  &nbsp;" << endl;
    //    out << "</tr>" << endl;
    out << "</table>" << endl;
    out << "<!-- =========== END OF NAVBAR =========== -->" << endl;
}

void jsDocumentation::addClassHierarchy(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto bases = GetAllBases(type);
    if (bases.size() > 0) {
        out << "<br/>" << endl;
        out << "<dl>" << endl;
        out << "<dt><h1>Details</h1></dt>" << endl;
        out << "<dl>" << endl << "<dt>Class hierarchy:</dt><dd>" << endl << "</dl>" << endl;;
        out << "<pre>" << endl;

        std::string spacing;
        for (auto it = bases.rbegin(); it != bases.rend(); it++) {
            auto base = *it;
            out << makeHREF(base->GetName(), type) << endl;
            out << spacing << "  |" << endl;
            out << spacing << "  +--";
            spacing += "     ";
        }
        out << makeHREF(type->GetName(), type, true) << endl;
        out << "</pre>" << endl;
    }

}
void jsDocumentation::addClassChildren(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto derivedTypes = type->GetDerivedClasses();
    if (derivedTypes.empty())
        return;
    std::map<std::string, DNVS::MoFa::Doc::TypePointer> sortedDerived;
    for (auto derivedType : derivedTypes)
        sortedDerived[derivedType->GetName()] = derivedType;

    out << "<dl>" << endl;
    out << "<dt>Direct Known Subclasses:</dt> <dd>";
    bool first = true;
    for (const auto& nameDerivedPair : sortedDerived) 
    {
        if (!first) out << ", ";
        first = false;
        out << makeHREF(nameDerivedPair.first, type, true);
    }
    out << "</dd>" << endl;
    out << "</dl>" << endl;
}


void jsDocumentation::addClassData(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    out << "<!-- ======== START OF CLASS DATA ======== -->" << endl;

    if(!type->GetDocumentation().empty())
    {
        out << "<dd>" << endl;
        out << "<dl><dt>Description:</dt>" << endl;
        out << type->GetDocumentation() << endl;
        out << "</dl>" << endl;
        out << "<dl>" << endl;
    }
    if (!type->GetExample().empty()) {
        out << makeExample(type->GetExample()) << endl;
    }
    out << makeExamples(type->GetExamples());


    addClassHierarchy(out, type);
    addClassChildren(out, type);


    out << "<hr/>" << endl;
}

std::string jsDocumentation::makeHREF(const std::string& typeName, const DNVS::MoFa::Doc::TypePointer& type, bool bold /*= false*/, bool href /*= true*/, std::string titleTip /*= ""*/) const
{
    if (typeName.empty() || typeName == " ") return "";

    std::string text;
    if (bold) text += "<b>";
    if (type != nullptr && type->GetName() == typeName) href = false;
    else if (typeName == "double" ||
        typeName == "string" ||
        typeName == "bool" ||
        typeName == "int" ||
        typeName == "void" ||
        typeName == "...")
    {
        href = false;
    }
    if (href) {
        text += "<a href=\"";
        text += typeName;
        text += ".html\"";
        if (!titleTip.empty()) {
            text += " title=\"";
            text += titleTip;
            text += "\"";
        }
        text += ">";
    }
    if (typeName != "void") text += typeName;
    if (typeName == "void") text += "&nbsp;";
    if (href) text += "</a>";
    if (bold) text += "</b>";
    return text;

}

std::string jsDocumentation::makeHREF(const DNVS::MoFa::Doc::MemberPointer& member, const DNVS::MoFa::Doc::TypePointer& type, bool bold /*= false*/, bool href /*= true*/, std::string titleTip /*= ""*/) const
{
    std::string ref = member->GetName();
    ref += "(";
    for (size_t i = 0; i < member->GetArity(); i++) {
        if (i != 0)
            ref += ", ";
        ref += member->GetArgumentTypeName(i);;
    }
    ref += ")";
    std::string text;
    if (bold) text += "<b>";
    if (href) {
        text += "<a href=\"";

        text += type->GetName();
        text += ".html#";
    }
    else {
        text += "<a name=\"";
    }
    text += ref;
    text += "\"";
    if (titleTip.size() > 0) {
        text += " title=\"";
        text += titleTip;
        text += "\"";
    }
    text += ">";
    if (href) text += member->GetName();
    text += "</a>";
    if (bold) text += "</b>";
    return text;
}

std::string ToUpper(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

std::string ToLower(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    return text;
}
std::string jsDocumentation::GetMethodHeader(const std::string& methodType, const std::string& headerType, size_t colSpan) const
{
    std::string doc;
    if (colSpan == 2)
        doc += "&nbsp;\n";
    doc += "<!-- ========= " + ToUpper(methodType) + " " + ToUpper(headerType) + " ======== -->\n";
    doc += "\n";
    doc += "<a name=\"" + ToLower(methodType) + "_" + ToLower(headerType) +"\"><!-- --></a>\n";
    doc += "<table border=\"1\" cellpadding=\"3\" cellspacing=\"0\" width=\"100%\">\n";
    doc += "<tr class=\"header\">\n";
    doc += "<td colspan=" + boost::lexical_cast<std::string>(colSpan) +">\n";
    doc += methodType + " " + headerType + "</td>\n";
    doc += "</tr>\n";
    if(colSpan == 1) 
        doc += "</table>\n";
    return doc;
}

std::string jsDocumentation::argument(const DNVS::MoFa::Doc::TypePointer& type, const DNVS::MoFa::Doc::MemberPointer& member, std::size_t iarg, bool stripped /*= false*/) const
{
    std::string typeName = member->GetArgumentTypeName(iarg);
    string arg;
    std::size_t size = member->GetArity();
    if (stripped || !member->HasArgument(iarg)) 
        return typeName;
    if (typeName == "...") {
        arg += "[";
        auto nameSize = member->GetNamedVarargCount(iarg);;
        for (auto j = 0; j < nameSize; j++) 
        {
            arg += member->GetVariableName(iarg, j);
            arg += " ...";
            if (j != nameSize - 1) arg += ", ";
        }
        if (nameSize == 0) arg += "...";
        arg += "]";
    }
    else {
        std::string variableName = member->GetVariableName(iarg);
        std::string variableDoc = member->GetArgumentDocumentation(iarg);
        arg += makeHREF(typeName, type, true);
        if (!variableName.empty())
        {
            if (!variableDoc.empty()) {
                arg += "<span title=\"";
                arg += variableDoc;
                arg += "\">";
            }
            if (!member->GetName().empty() && member->GetName() !=  " ") 
                arg += " ";
            arg += variableName;
            if (!variableDoc.empty()) {
                arg += "</span>";
            }
        }
    }
    return arg;
}

std::string jsDocumentation::argumentList(const DNVS::MoFa::Doc::TypePointer& type, const DNVS::MoFa::Doc::MemberPointer& member) const
{
    std::string args = "(";
    for (size_t i = 0; i < member->GetArity(); i++)
    {
        if (i > 0)
            args += ", ";
        args += argument(type, member, i);
    }
    args += ")";
    return args;
}

std::string jsDocumentation::argumentDocumentation(const DNVS::MoFa::Doc::MemberPointer& member) const
{
    bool hasArgumentDoc = false;
    std::string doc = "<dl><dt>Parameters:</dt>\n";
    for (size_t i = 0; i < member->GetArity(); i++)
    {
        if (member->HasArgument(i))
        {
            for (size_t j = 0; j < member->GetNamedVarargCount(i); j++) {
                if (!member->GetArgumentDocumentation(i, j).empty())
                {
                    doc += "<dd><code>"
                        + member->GetVariableName(i, j)
                        + "</code>"
                        + " - "
                        + member->GetArgumentDocumentation(i, j);
                    hasArgumentDoc = true;
                }
            }
        }
    }
    doc += "</dl>\n";
    if (hasArgumentDoc)
        return doc;
    else
        return "";
}

std::string jsDocumentation::argumentReturnDocumentation(const DNVS::MoFa::Doc::MemberPointer& member) const
{
    if (!member->GetReturnedDocumentation().empty())
    {
        return  
            "<dl>\n"
            "<dt>Returns:</dt>\n"
            "<dd>\n"
            "</dl>\n";
    }
    return "";
}

void jsDocumentation::addConstructorSummary(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto constructors = type->GetMembers(DNVS::MoFa::Doc::IMember::Constructor);

    if (constructors.size() == 0) return;

    out << GetMethodHeader("Constructor", "Summary", 2);

    for (const auto& constructor : constructors)
    {
        out << "<tr >" << endl;

        out << "<td><code>" << makeHREF(constructor, type, true);
        out << argumentList(type, constructor);
        out << "</code>" << endl;

        out << "<div class=\"explanation\">" << endl;
        if (constructor->GetDocumentation(false).empty()) m_docError[type->GetName() + "::" + constructor->GetName()] |= jsTypeInfo::jsShort;
        out << constructor->GetDocumentation(false) << "</div></td>" << endl;
        out << "</tr>" << endl;
    }
    out << "</table>" << endl;

}

void jsDocumentation::addFunctionSummaryContent(std::map<std::string, std::string >& functionInfo,
    const DNVS::MoFa::Doc::TypePointer& type, bool inherited)
{
    auto functions = type->GetMembers(DNVS::MoFa::Doc::IMember::Function);

    for (const auto& function : functions)
    {
        std::string text;
        text = "<tr \n";
        if (inherited) text += "class=\"inherited\">\n";
        if (!inherited) text += "class=\"own\">\n";
        text += "<td class=\"returntype\" align=\"right\" valign=\"top\" width=\"1%\">";
        if (!function->GetReturnedTypeName().empty()) {
            text += "<code>&nbsp;";
            text += makeHREF(function->GetReturnedTypeName(), type, true);
            text += "</code>\n";
        }
        text += "</td>\n";
        text += "<td class=\"functionsummary\">";

        text += "<code>";
        text += makeHREF(function, type, true);
        text += argumentList(type, function);
        text += "</code>\n";
        text += "<div class=\"explanation\">\n";
        if (function->GetDocumentation(false).empty()) m_docError[type->GetName() + "::" + function->GetName()] |= jsTypeInfo::jsShort;
        text += function->GetDocumentation(false);
        text += "</div></td>\n</tr>\n";
        functionInfo.insert(make_pair(function->ToString(true), text));
    }

}

void jsDocumentation::addFunctionSummary(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto bases = GetAllBases(type);
    std::map<string, string > functionInfo;
    addFunctionSummaryContent(functionInfo, type, false);
    for (auto base : bases)
    {
        addFunctionSummaryContent(functionInfo, base, true);
    }
    if (functionInfo.size() == 0) return;

    out << GetMethodHeader("Function", "Summary", 2);

    for (std::map<string, string >::iterator ic = functionInfo.begin(); ic != functionInfo.end(); ic++) {
        out << ic->second;
    }
    out << "</table>" << endl;

}

void jsDocumentation::addPropertySummaryContent(std::map<std::string, std::string >& propertyInfo,
    const DNVS::MoFa::Doc::TypePointer& type, bool inherited)
{
    auto properties = type->GetMembers(DNVS::MoFa::Doc::IMember::Property);
    for (const auto& property : properties)
    {
        string text = "<tr \n";
        if (inherited) text += "class=\"inherited\">\n";
        if (!inherited) text += "class=\"own\">\n";
        text += "<td align=\"right\" valign=\"top\" width=\"1%\">\n";
        //Return value
        if (!property->GetReturnedTypeName().empty()) {
            text += "<code>&nbsp;";
            text += makeHREF(property->GetReturnedTypeName(), type, true);
            text += "</code>\n";
        }
        text += "</td>\n";

        std::string status = " ";
        if (property->GetMemberType() == DNVS::MoFa::Doc::IMember::PropertyGet)
            status += "read only";
        else if(property->GetMemberType() == DNVS::MoFa::Doc::IMember::PropertySet)
            status += "write only";
        else
            status += "read/write";


        //Property name
        text += "<td width=\"90%\"><code>" + makeHREF(property, type, true) + "</code>";
        text += " <span class=\"propertyStatus\">" + status + "</span>\n";

        text += "<div class=\"explanation\">\n";

        if (property->GetDocumentation(false).empty()) 
            m_docError[type->GetName() + "::" + property->GetName()] |= jsTypeInfo::jsShort;
        text += property->GetDocumentation(false) + "</div></td>\n";

        text += "</tr>\n";
        propertyInfo.insert(make_pair(property->GetName(), text));
    }
}

void jsDocumentation::addPropertySummary(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto bases = GetAllBases(type);
    std::map<string, string > propertyInfo;
    addPropertySummaryContent(propertyInfo, type, false);
    for (auto base : bases)
    {
        addPropertySummaryContent(propertyInfo, base, true);
    }
    if (propertyInfo.size() == 0) return;

    out << GetMethodHeader("Property", "Summary", 2);
    for (std::map<string, string >::iterator ic = propertyInfo.begin(); ic != propertyInfo.end(); ic++) {
        out << ic->second;
    }
    out << "</table>" << endl;

}

void jsDocumentation::addItems(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    bool found = false;
    for (const auto& documentedPair : m_documented)
    {
        const auto& documentedType = documentedPair.second;
        auto baseClasses = documentedType->GetBaseClasses();
        if (baseClasses.count(type) != 0 || (type == nullptr && baseClasses.empty()))
        {
            if (!found) out << "<ul id=\"sitemap\">";
            found = true;
            out << "<li>" << makeHREF(documentedPair.first, nullptr) << endl;
            addItems(out, documentedType);
        }
    }
    if (found) out << "</ul>";
}

void jsDocumentation::addClassHierarchy()
{
    string fileName = m_directory + "\\ClassHierarchy.html";
    fstream out(fileName.c_str(), ios_base::out);
    addTitle(out, NULL);
    out << "<link href=\"css/sitemapstyler.css\" rel=\"stylesheet\" type=\"text/css\" media=\"screen\" />" << endl;
    out << "<script type=\"text/javascript\" src=\"css/sitemapstyler.js\"></script>" << endl;
    out << "<!--[if IE]>" << endl;
    out << "<link rel=\"stylesheet\" href=\"css/style_ie.css\" type=\"text/css\" media=\"all\" />" << endl;
    out << "<![endif]--> " << endl;
    out << "<body>" << endl;
    addNavBar(out);
    out << "<div class=\"wrapper\">" << endl;
    out << "<div class=\"header\">" << endl;
    out << "</div>" << endl;
    out << "<div class=\"instructions\">" << endl;
    out << "<h2>JScript Command Library: Hierarchy</h2>" << endl;
    out << "<p>" << endl;
    out << "Filter through the sitemap hierarchy by using the search field or manually expand or close the hierarchy by using the";
    out << "options \"Expand All\" or \"Close All\". To start a new search, please click the text \"Clear Filters\" ";
    out << "For more information, <a href=\"mailto:software.support@dnv.com\" target=\"_blank\" title=\"Opens in a new window\">contact us</a>. " << endl;
    out << "</p>" << endl;
    out << "</div>" << endl;
    out << "<div id=\"container\"> " << endl;
    out << "<div class=\"content\" align=\"left\">" << endl;
    out << "<label for=\"search\">Search Hierarchy</label> <input type=\"text\" id=\"search\" name=\"search\" /><a href=\"\" onclick=\"window.location.reload()\">&nbsp;Clear Filters</a>" << endl;
    out << "<noscript><p><strong>Sorry, this page needs JavaScript to show the Class Hierarchy correctly</strong></p></noscript>" << endl;
    out << "<p><a href=\"javascript:openMap();\">Expand All</a>&nbsp; |&nbsp; <a href=\"javascript:closeMap();\">Collapse All </a></p>" << endl;
    addItems(out, nullptr);
    out << "</div>" << endl;
    out << "</div>" << endl;
    out << "</div>" << endl;
    out << "<script>" << endl;
    out << "//call after page loaded" << endl;
    out << "window.onload=openMap(); " << endl;
    out << "</script>" << endl;
    out << "<script type=\"text/javascript\" src=\"js/links_search.js\"></script>" << endl;
    out << "<script src=\"js/jsl.js\" type=\"text/javascript\"></script> " << endl;

    out << "</body>" << endl;
    out << "</html>" << endl;
}


void jsDocumentation::addIndexItems(ostream& out)
{
    bool found = false;
    SortedString::iterator it;
    for (it = m_leaves.begin(); it != m_leaves.end(); it++) {
        out << it->second;
    }
}


void jsDocumentation::addIndex()
{
    string fileName = m_directory + "\\index.html";
    fstream out(fileName.c_str(), ios_base::out);
    addTitle(out, NULL);
    out << "<link href=\"css/styletable.css\" rel=\"stylesheet\" type=\"text/css\" />" << endl;

    out << "<body>" << endl;
    addNavBar(out);
    out << "<div class=\"wrapper\">" << endl;
    out << "<div class=\"header\">" << endl;
    out << "<div class=\"instructions\">" << endl;
    out << "<h2>JScript Command Library: Overview</h2>" << endl;
    out << "<p>" << endl;
    out << "Filter through the cells by selecting the dropdowns at the top of the columns. You can also filter using checkboxes for different<br/>";
    out << "categories, the main search bar or search bars for the columns Title and Description. This functionality currently works for <br/>";
    out << "Google Chrome, Internet Explorer and Opera, <b>not Firefox</b>. Please use one of the working browsers<br/>";
    out << "For more information, <a href=\"mailto:software.support@dnv.com\" target=\"_blank\" title=\"Opens in a new window\">contact us</a>.<br/>";
    out << "</p>" << endl;
    out << "<noscript><p><strong>Sorry, this page needs JavaScript to show the Class Hierarchy correctly</strong></p></noscript>" << endl;
    out << "</div>" << endl;
    out << "</div>  " << endl;

    out << "</div>" << endl;
    out << "<div class=\"content\">" << endl;
    out << "<div class=\"column0\">	" << endl;
    out << "<table width=\"990px\" id='demotable0'>" << endl;
    out << "<thead>" << endl;
    out << "<tr>" << endl;
    out << "<th width=\"10%\" filter='false'>Analysis: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyanalysis\"/> </th>" << endl;
    out << "<th width=\"12%\" filter='false'>Capacity Checks: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlycapacity\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Compartments: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlycompartments\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Environment: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyenvironment\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Exchange: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyexchange\"/></th>" << endl;
    out << "<th width=\"12%\" filter='false'>Guiding Geometry: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyguidinggeometry\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Loads: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyloads\"/> </th>" << endl;
    out << "</tr>" << endl;
    out << "<tr>" << endl;
    out << "<th width=\"10%\" filter='false'>Properties: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyproperties\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Reports: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyreports\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Sets: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlysets\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Structure: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlystructure\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Units: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyunits\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Graphics: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlygraphics\"/></th>" << endl;
    out << "<th width=\"10%\" filter='false'>Other: <input type=\"checkbox\" class=\"Checkbox\" id=\"onlyother\"/></th>" << endl;
    out << "</tr>" << endl;
    out << "</thead>" << endl;
    out << "</table>" << endl;
    out << "</div>" << endl;
    out << "<h4>Search Library: <input type=\"text\" id=\"quickfind\"/>" << endl;
    out << "<a id=\"cleanfilters\" href=\"index.html\">Clear Filters</a></h4> " << endl;
    out << "<div class=\"clear\"></div>" << endl;

    out << "<div class=\"column1\">	" << endl;
    out << "<table id='demotable1'>" << endl;
    out << "<thead>" << endl;
    out << "<tr>" << endl;
    out << "<th width=\"20%\" filter-type='ddl'>Category</th>" << endl;
    out << "<th width=\"25%\">Title</th>" << endl;
    out << "<th width=\"45%\">Description</th>" << endl;
    out << "</thead>" << endl;
    out << "<tbody>" << endl;
    addIndexItems(out);
    out << "</tbody>" << endl;
    out << "</table>" << endl;
    out << "</div>" << endl;

    out << "</body>" << endl;
    out << "</html>" << endl;
}

std::string jsDocumentation::memberToString(const DNVS::MoFa::Doc::MemberPointer& member, const DNVS::MoFa::Doc::TypePointer& type)
{
    std::string doc;
    doc += makeHREF(member, type, false, false) + "\n";
    doc += "<h3>" + member->GetName() + "</h3>\n";
    doc += "<p class=\"function\">\n";
    if(member->GetMemberType() != DNVS::MoFa::Doc::IMember::Constructor)
        doc += makeHREF(member->GetReturnedTypeName(), type, true) + " ";
    doc += "<b>" + member->GetName() + "</b>";
    doc += argumentList(type, member);
    doc += "</p>\n";
    doc += "<p>\n";
    return doc;
}

std::list<DNVS::MoFa::Doc::TypePointer> jsDocumentation::GetAllBases(const DNVS::MoFa::Doc::TypePointer& type) const
{
    std::list<DNVS::MoFa::Doc::TypePointer> allTypes;
    std::list<DNVS::MoFa::Doc::TypePointer> unprocessed;
    unprocessed.push_back(type);
    std::set<DNVS::MoFa::Doc::TypePointer> processed;
    while (!unprocessed.empty())
    {
        auto thisType = unprocessed.front();
        unprocessed.pop_front();
        if(thisType != type)
            allTypes.push_back(thisType);
        if (processed.insert(thisType).second)
        {
            for (auto base : thisType->GetBaseClasses())
            {
                unprocessed.push_back(base);
            }
        }
    }
    return allTypes;
}

void jsDocumentation::AddLeaves(const std::vector<DNVS::MoFa::Doc::MemberPointer>& members, const DNVS::MoFa::Doc::TypePointer& type)
{
    for (const auto& member : members)
    {
        string headerTxt = makeHREF(member, type, false, true);
        string overviewTxt = member->GetDocumentation(false);
        m_leaves.insert(make_pair(member->ToString(true), makeOverviewLine(member->GetNamespace(), headerTxt, overviewTxt)));
    }
}

std::string jsDocumentation::memberDocumentation(const DNVS::MoFa::Doc::MemberPointer& member) const
{
    std::string doc;
    if (!member->GetDocumentation().empty()) {
        doc += "<dl>\n";
        doc += "<dt>Description:</dt>\n";
        doc += "<dd>" + member->GetDocumentation() + "\n";
        doc += "</dl>\n";
    }
    return doc;
}

void jsDocumentation::addConstructorDetail(ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto constructors = type->GetMembers(DNVS::MoFa::Doc::IMember::Constructor);
    if (constructors.empty())
        return;

    string headerTxt = makeHREF(type->GetName(), NULL, false, true);
    string overviewTxt = type->GetDocumentation();

    m_leaves[type->ToString(true)] = makeOverviewLine(type->GetNamespace(), headerTxt, overviewTxt);

    out << GetMethodHeader("Constructor", "Detail");

    for (auto constructor : constructors)
    {
        out << memberToString(constructor, type);
        out << memberDocumentation(constructor);
        out << argumentDocumentation(constructor);
        out << makeExamples(constructor->GetExamples());

        out << "<hr>" << endl;
        out << endl;
    }
}

void jsDocumentation::addFunctionDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto functions = type->GetMembers(DNVS::MoFa::Doc::IMember::Function);
    if (functions.empty())
        return;

    out << GetMethodHeader("Function", "Detail");
    for (auto function : functions)
    {
        out << memberToString(function, type);
        out << memberDocumentation(function);
        out << argumentDocumentation(function);
        out << argumentReturnDocumentation(function);


        out << makeExamples(function->GetExamples());

        out << "<hr>" << endl;
        out << endl;
    }
}

void jsDocumentation::addPropertyDetail(std::ostream& out, const DNVS::MoFa::Doc::TypePointer& type)
{
    auto properties = type->GetMembers(DNVS::MoFa::Doc::IMember::Property);
    if (properties.empty())
        return;

    out << GetMethodHeader("Property", "Detail");

    for (const auto& property : properties)
    {
        out << memberToString(property, type);
        out << memberDocumentation(property);
    }
}

std::string jsDocumentation::makeExample(std::string exampleTxt, bool legal)
{
    if (exampleTxt.size() == 0) return exampleTxt;
    string txt = "<dl>\n<dt>Example:</dt>\n<dd>\n<table>\n<tr>";
    if (legal) {
        txt += "<td class=\"example\">\n";
    }
    else {
        txt += "<td class=\"illegalexample\">\n";
    }

    txt += "<pre>\n";
    txt += exampleTxt;
    txt += "</pre>\n</td></tr>\n</table>\n</dd></dl>";
    return txt;
}

std::string jsDocumentation::makeExamples(const std::vector<jsExampleInfo>& examples)
{
    if (examples.empty()) return "";
    std::string result = "<dt>Example of use: </dt></dl>";
    BOOST_FOREACH(auto& info, examples) {
        result += "<table border=\"1\" cellpadding=\"3\" cellspacing=\"0\" width=\"100%\">";
        result += "<tr class=\"header\">";
        result += "  <td colspan=2>" + info.getTitle() + "</td>";
        result += "</tr>";
        result += "<tr>";
        result += "  <td>";
        result += "     <code>";
        result += info.getScript();
        result += "     </code>";
        result += "		<div class=\"explanation\">";
        result += info.getComment();
        result += "		</div>";
        result += "   </td>";
        result += "</tr>";
        result += "</table>";
    }
    return result;
}

std::string jsDocumentation::makeOverviewLine(const std::string& nameSpace, const std::string& headerTxt, const std::string& explanationTxt)
{
    string txt = "";
    if (nameSpace == "")
    {
        txt = "<tr><td>Other</td><td class=\"overviewheader\" title=\"";
        txt += explanationTxt;
        txt += "\">\n";
        txt += headerTxt;
        txt += "</td>\n";
        txt += "<td class=\"explanation\">";
        txt += explanationTxt + "&nbsp;";
        txt += "</td>\n";
        txt += "</tr>";
    }
    else if (nameSpace != "")
    {
        txt = "<tr><td>" + nameSpace + "</td><td class=\"overviewheader\" title=\"";
        txt += explanationTxt;
        txt += "\">\n";
        txt += headerTxt;
        txt += "</td>\n";
        txt += "<td class=\"explanation\">";
        txt += explanationTxt + "&nbsp;";
        txt += "</td>\n";
        txt += "</tr>";
    }

    return txt;
}
