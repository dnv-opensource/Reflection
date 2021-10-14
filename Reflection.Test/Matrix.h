#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <vector>

namespace DNVS {namespace MoFa {namespace Space {

	class Matrix
    {
    protected:
        size_t m_noRows, m_noCols;
        std::vector<double> m_Matrix;
        void Initialize();
    public:
        // provides interface to be able to access matrix with [i][j] by reference
        class MatrixAsArrayAccessor
        {
        public:
            Matrix& m_matrix;
            size_t m_row;
            MatrixAsArrayAccessor(Matrix& matrix, size_t row) 
                : m_matrix(matrix), m_row(row)
            { }

            double& operator[](size_t col)
            {
                return m_matrix(m_row, col);
            }
        };

        // provides interface to be able to access matrix with [i][j] by value
        class ConstMatrixAsArrayAccessor
        {
        public:
            const Matrix& m_matrix;
            size_t m_row;
            ConstMatrixAsArrayAccessor(const Matrix& matrix, size_t row) 
                : m_matrix(matrix), m_row(row)
            { }

            double operator[](size_t col) const
            {
                return m_matrix(m_row, col);
            }
        };

        Matrix(void);
        Matrix(size_t n0);
        Matrix(size_t NoRows,size_t NoCols);
        Matrix(const Matrix&) = default;
        ~Matrix(void);
        const Matrix operator * (const Matrix& M) const;
        double& operator()(size_t i,size_t j);
        double operator()(size_t i,size_t j) const;
        
        MatrixAsArrayAccessor operator[](size_t i);
        ConstMatrixAsArrayAccessor operator[](size_t i) const;

        void SetSize(size_t NoRows,size_t NoCols);
        void SetSize(size_t n0);
        size_t GetNoRows() const;
        size_t GetNoCols() const;
        void Zero();
        void Identity();
        Matrix GetTranspose() const;
        bool IsZero() const;
        bool operator == (const Matrix& M) const;
    };

}}}