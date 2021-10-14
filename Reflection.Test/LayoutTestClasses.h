#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Enums/Enum.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/LayoutAttribute.h"
#include "EnumOrValue.h"
#include "Reflection/Attributes/ExpandMemberAttribute.h"
#include "Reflection/Attributes/CaptionAttribute.h"
#include "Vector3d.h"
#include "Reflection/Attributes/SaveFileAttribute.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes { namespace Tests {
    enum MyEnumForLayout2
    {
        first, second
    };
    enum MyEnumForLayout4
    {
        myA, myB, myC, myD
    };

    enum MyEnumAsCombo
    {
        Red, Green, Blue
    };

    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MyEnumForLayout2**)
    {
        using namespace Enums;
        Enum<MyEnumForLayout2> cls(typeLibrary, "MyEnumForLayout2");
        cls.EnumValue("first", first);
        cls.EnumValue("second", second);
    }
    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MyEnumForLayout4**)
    {
        using namespace Enums;
        Enum<MyEnumForLayout4> cls(typeLibrary, "MyEnumForLayout4");
        cls.EnumValue("myA", myA);
        cls.EnumValue("myB", myB);
        cls.EnumValue("myC", myC);
        cls.EnumValue("myD", myD);
    }
    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MyEnumAsCombo**)
    {
        using namespace Enums;
        Enum<MyEnumAsCombo> cls(typeLibrary, "MyEnumAsCombo");
        cls.EnumValue("Red", Red);
        cls.EnumValue("Green", Green);
        cls.EnumValue("Blue", Blue);
        cls.AddAttribute<LayoutAttribute>(ComboBox());
    }
    class MyTestClassForLayout {
    public:
        MyTestClassForLayout(double, MyEnumForLayout2, MyEnumForLayout4) {}
        double GetA() const { throw; }
        MyEnumForLayout2 GetEnum1() const { throw; }
        MyEnumForLayout4 GetEnum2() const { throw; }
        typedef EnumOrValue<MyEnumForLayout2, first, double> EnumOrDouble;
        EnumOrDouble GetEnumOrValue() const { throw; }
        bool GetBool() const { throw; }
        std::vector<long> GetVector() const { throw; }
        void CallWithEnumAsCombo(MyEnumAsCombo) const { throw; }
        MyEnumAsCombo GetEnumAsCombo() const { throw; }
    };

    class TypeToEmbed {
    public:
        double GetX() const { throw; }
        void SetX(double) {}
        double GetY() const { throw; }
        void SetY(double) {}
    };

    class EmbedderType {
    public:
        TypeToEmbed GetPoint() const { throw; }
        void SetPoint(TypeToEmbed) {}
    };
    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TypeToEmbed**)
    {
        using namespace Classes;
        Class<TypeToEmbed> cls(typeLibrary, "TypeToEmbed");
        cls.SetGet("X", &TypeToEmbed::SetX, &TypeToEmbed::GetX);
        cls.SetGet("Y", &TypeToEmbed::SetY, &TypeToEmbed::GetY);
    }
    class A {
    public:
        A(int i) {}
        A(double x, double y) {}
        double GetX() const { throw; }
        double GetY() const { throw; }
        int GetI() const { throw; }
    };
    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, A**)
    {
        using namespace Classes;
        Class<A> cls(typeLibrary, "A");
        cls.AddAttribute<ExpandMemberAttribute>(ExpandOptions::CanExpand);
        cls.Constructor<int>().AddSignature("i");
        cls.Constructor<double, double>().AddSignature("x", "y");
        cls.Get("X", &A::GetX);
        cls.Get("Y", &A::GetY);
        cls.Get("I", &A::GetI).AddAttribute<CaptionAttribute>("Index");
    }

    class B {
    public:
        B(const A& a, const A& a2) {}
        A GetA() const { throw; }
        void SetA(const A&) {}
        A GetA2() const { throw; }
        void SetA2(const A&) {}
        std::vector<A> GetAllA() const { throw; }
    };
    class MyRow {
    public:
        MyRow(double x, double y, size_t layers) : m_x(x), m_y(y), m_layers(layers) {}
        double GetX() const { return m_x; }
        double GetY() const { return m_y; }
        void SetX(double x) { m_x = x; }
        void SetY(double y) { m_y = y; }
        void SetLayers(size_t layers) { m_layers = layers; }
        size_t GetLayers() const { return m_layers; }
    private:
        double m_x, m_y;
        size_t m_layers;
    };

    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, MyRow**)
    {
        using namespace Classes;
        Class<MyRow> cls(typeLibrary, "MyRow");
        cls.Constructor<double, double, size_t>().AddSignature("x", "y", "layers");
        cls.SetGet("X", &MyRow::SetX, &MyRow::GetX);
        cls.SetGet("Y", &MyRow::SetY, &MyRow::GetY);
        cls.SetGet("Layers", &MyRow::SetLayers, &MyRow::GetLayers).AddAttribute<CaptionAttribute>("Number of layers");
    }

    class MyGrid {
    public:
        void SetRows(const std::vector<MyRow>& rows) { m_rows = rows; }
        std::vector<MyRow> GetRows() const { return m_rows; }
    private:
        std::vector<MyRow> m_rows;
    };

    class C {
    public:
        static void TestFunction(int a, double b) {}
    };

    class UtilityClass {
    public:
        UtilityClass() : m_opt(0) {}
        void TestFunction(int a, double b) {}
        bool GetOptionBool() const { return m_optBool; }
        void SetOptionBool(bool opt) { m_optBool = opt; }
        int GetOption() const { return m_opt; }
        void SetOption(int opt) { m_opt = opt; }
        int GetOption2() const { return m_opt2; }
        void SetOption2(int opt) { m_opt2 = opt; }
    private:
        int m_opt;
        int m_opt2;
        bool m_optBool;
    };

    class EliminateEdgeClass {
    public:
        EliminateEdgeClass() : m_eliminateRelativeLength(false), m_eliminateAbsoluteLength(false), m_relativeLengthLimit(0), m_eliminateShortEdges(false)
        {}
        bool GetEliminateRelativeLength() const { return m_eliminateRelativeLength; }
        void SetEliminateRelativeLength(bool flag) { m_eliminateRelativeLength = flag; }
        double GetRelativeLengthLimit() const { return m_relativeLengthLimit; }
        void SetRelativeLengthLimit(double limit) { m_relativeLengthLimit = limit; }
        bool GetEliminateAbsoluteLength() const { return m_eliminateAbsoluteLength; }
        void SetEliminateAbsoluteLength(bool flag) { m_eliminateAbsoluteLength = flag; }
        void SetEliminateShortEdges(bool flag) { m_eliminateShortEdges = flag; }
        bool GetEliminateShortEdges() const { return m_eliminateShortEdges; }
    private:
        double m_relativeLengthLimit;
        bool m_eliminateRelativeLength;
        bool m_eliminateAbsoluteLength;
        bool m_eliminateShortEdges;
    };
    class ForceMoment {
    public:
        ForceMoment(const Vector3d& force, const Vector3d& moment) : m_force(force), m_moment(moment) {}
        const Vector3d& GetForce() const { return m_force; }
        void SetForce(const Vector3d& force) { m_force = force; }
        const Vector3d& GetMoment() const { return m_moment; }
        void SetMoment(const Vector3d& moment) { m_moment = moment; }
    private:
        Vector3d m_force;
        Vector3d m_moment;
    };

    class Plate {
    public:
        Plate* Divide(double x) { return nullptr; }
        Plate* Divide(int a, int b, int c) { return nullptr; }
        void Explode() {}
    };

    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Plate**)
    {
        using namespace Classes;
        Class<Plate> cls(typeLibrary, "Plate");
        cls.Function<Plate*(double)>("Divide", &Plate::Divide).AddSignature("x");
        cls.Function<Plate*(int, int, int)>("Divide", &Plate::Divide).AddSignature("a", "b", "c");
        cls.Function("Explode", &Plate::Explode);
    }

    class FileExporter {
    public:
        FileExporter() {}
        void SetValue(double v) {}
        double GetValue() const { return 0.0; }
        void DoExport(const std::string& fileName) {}
    };
    inline void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, bool enforceJournalling, FileExporter**)
    {
        using namespace Reflection::Classes;
        Class<FileExporter> cls(typeLibrary, "FileExporter");
        cls.Constructor();
        cls.SetGet("Value", &FileExporter::SetValue, &FileExporter::GetValue);
        if (enforceJournalling)
        {
            cls.Function("DoExport", &FileExporter::DoExport)
                .AddSignature("fileName")
                .AddAttribute<SaveFileAttribute>(FileType("js", "Javascript files"), JournallingRequired);
        }
        else
        {
            cls.Function("DoExport", &FileExporter::DoExport)
                .AddSignature("fileName")
                .AddAttribute<SaveFileAttribute>(FileType("js", "Javascript files"));
        }
    }
}}}}}