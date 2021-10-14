//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Matrix.h"

namespace DNVS {namespace MoFa {namespace Space {

    Matrix::Matrix(void)
    {
        m_noRows = 0; 
        m_noCols = 0;
    }

    Matrix::~Matrix(void)
    {
    }

    Matrix::Matrix(size_t n0)
    {
        SetSize(n0);
    }

    Matrix::Matrix(size_t noRows, size_t noCols)
    {
        SetSize(noRows, noCols);
    }

    void Matrix::SetSize(size_t n0)
    {
        SetSize(n0, n0);
    }

    void Matrix::SetSize(size_t noRows, size_t noCols)
    {
        m_noRows = noRows; 
        m_noCols = noCols;
        Initialize();
    }

    void Matrix::Initialize()
    {
        m_Matrix.resize(m_noRows * m_noCols);
    }

    size_t Matrix::GetNoRows() const
    {
        return m_noRows;
    }

    size_t Matrix::GetNoCols() const
    {
        return m_noCols;
    }

    void Matrix::Zero()
    {
        for(size_t i=0;i < m_Matrix.size(); i++)
        {
            m_Matrix[i] = 0.0;
        }
    }
    bool Matrix::IsZero() const
    {
        for(size_t i=0;i < m_Matrix.size(); i++)
        {
            if (m_Matrix[i] != 0.0)
                return false;
        }
        return true;
    }

    void Matrix::Identity()
    {
        for(size_t i = 0; i < m_noRows; i++)
        {
            for(size_t j = 0; j <m_noCols; j++)
            {
                if(i == j)
                    this->operator()(i, j) = 1;
                else
                    this->operator()(i, j) = 0;
            }
        }
    }

    Matrix::MatrixAsArrayAccessor Matrix::operator[](size_t row)
    {
        return MatrixAsArrayAccessor(*this, row);
    }

    Matrix::ConstMatrixAsArrayAccessor Matrix::operator[](size_t row) const
    {
        return ConstMatrixAsArrayAccessor(*this, row);
    }

    double& Matrix::operator()(size_t row,size_t col)
    {
        return m_Matrix.at(row + col * m_noRows);
    }

    double Matrix::operator()(size_t row,size_t col) const 
    {
        return m_Matrix.at(row + col * m_noRows);
    }

    const Matrix Matrix::operator * (const Matrix& matrix) const
    {
        size_t n = matrix.GetNoRows(); 
        size_t m = matrix.GetNoCols();
        if(m_noCols != n)
            return Matrix(0);

        Matrix R(m_noRows, m);
        for(size_t i = 0; i < m_noRows; i++)
        {
            for(size_t j = 0; j < m; j++)
            {
                for(size_t k = 0; k < n; k++)
                {
                    R(i, j) += this->operator()(i, k) * matrix(k, j);
                }
            }
        }
        return R;
    }

    Matrix Matrix::GetTranspose() const
    {
        Matrix T;
        if (m_noCols != m_noRows)
            return T;
        T.SetSize(m_noRows);
        for (size_t i=0; i < m_noRows; i++)
        {
            for (size_t j = 0 ; j < m_noRows; j++)
            {
                T(i, j) = this->operator()(j, i);
            }
        }
        return T;
    }

    bool Matrix::operator == (const Matrix& M) const
    {
        if (this->m_noCols != M.m_noCols)
            return false;
        if (this->m_noRows != M.m_noRows )
            return false;

        for (size_t i=0;i<m_Matrix.size();i++)
        {
            if (m_Matrix[i]!=M.m_Matrix[i])
            {
                return false;
            }
        }
        return true;
    }
}}}