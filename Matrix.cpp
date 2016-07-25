// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	m_nRowCount = 0;
	m_nColumnCount = 0;
	m_ppData = NULL;
}

CMatrix::CMatrix(const CMatrix& matrix)
{
	Destroy();

	m_nRowCount = matrix.m_nRowCount;
	m_nColumnCount = matrix.m_nColumnCount;

	m_ppData = new double*[m_nRowCount];
	for ( int i = 0; i < m_nRowCount; ++i )
	{
		m_ppData[i] = new double[m_nColumnCount];
		for ( int j = 0; j < m_nColumnCount; ++j )
			m_ppData[i][j] = matrix.m_ppData[i][j];
	}
}

CMatrix::~CMatrix()
{
	Destroy();
}

CMatrix& CMatrix::operator= (const CMatrix& matrix)
{
	if ( this != &matrix )
	{
		Destroy();

		m_nRowCount = matrix.m_nRowCount;
		m_nColumnCount = matrix.m_nColumnCount;

		m_ppData = new double*[m_nRowCount];
		for ( int i = 0; i < m_nRowCount; ++i )
		{
			m_ppData[i] = new double[m_nColumnCount];
			for ( int j = 0; j < m_nColumnCount; ++j )
				m_ppData[i][j] = matrix.m_ppData[i][j];
		}
	}

	return *this;
}

void CMatrix::Destroy()
{
	for ( int i = 0; i < m_nRowCount; ++i )
	{
		delete []m_ppData[i];
	}
	delete []m_ppData;

	m_ppData = NULL;
	m_nRowCount = 0;
	m_nColumnCount = 0;
}

CSize CMatrix::GetSize()
{
	return CSize(m_nRowCount, m_nColumnCount);
}

void CMatrix::Create(int nRowCount, int nColumnCount, BYTE* pData)
{
	if ( NULL != pData )
	{
		Destroy();
		
		m_nColumnCount = nColumnCount;
		m_nRowCount = nRowCount;
		m_ppData = new double*[nRowCount];
		for ( int i = 0; i < nRowCount; ++i )
		{
			m_ppData[i] = new double[nColumnCount];
			int nBase = i * nColumnCount;
			for ( int j = 0; j < nColumnCount; ++j )
			{
				m_ppData[i][j] = pData[nBase + j];
			}
		}
	}
}

void CMatrix::Create(int nRowCount, int nColumnCount, double** ppData)
{
	Destroy();

	m_nColumnCount = nColumnCount;
	m_nRowCount = nRowCount;
	m_ppData = new double*[nRowCount];
	for ( int i = 0; i < nRowCount; ++i )
	{
		m_ppData[i] = new double[nColumnCount];
	}

	if ( NULL != ppData )
	{
		for ( int i = 0; i < m_nRowCount; ++i )
		{
			for ( int j = 0; j < m_nColumnCount; ++j)
				m_ppData[i][j] = ppData[i][j];
		}
	}
	else
	{
		for ( int i = 0; i < m_nRowCount; ++i )
		{
			for ( int j = 0; j < m_nColumnCount; ++j)
				m_ppData[i][j] = 0;
		}
	}
}

void CMatrix::CalMean(CMatrix& matrix)
{
	matrix.Create(m_nRowCount, 1);
	double** ppData = matrix.GetData();

	for ( int i = 0; i < m_nRowCount; ++i )
	{
		double d = 0;
		for ( int j = 0; j < m_nColumnCount; ++j )
		{
			d += m_ppData[i][j];
		}
		d /= m_nColumnCount;

		ppData[i][0] = d;
	}
}

void CMatrix::CalCovariance(CMatrix& matrix)
{
	matrix.Create(m_nRowCount, m_nRowCount);
	double** ppData = matrix.GetData();

	CMatrix matrixMean;
	CalMean(matrixMean);
	double** ppMean = matrixMean.GetData();

	for ( int i = 0; i < m_nRowCount; ++i )
	{
		for ( int j = i; j < m_nRowCount; ++j )
		{
			// 求取方差
			double d = 0;
			for ( int k = 0; k < m_nColumnCount; ++k )
			{
				d += (m_ppData[i][k] - ppMean[i][0]) * (m_ppData[j][k] - ppMean[j][0]);
			}
			d /= m_nColumnCount;

			ppData[i][j] = ppData[j][i] = d;
		}
	}
}

void CMatrix::CalInverse(CMatrix& matrix)
{
	// 求行列式
	double dDet = CalDeterminan();

	if ( dDet != 0 )
	{
		// 使用伴随矩阵求转置
		matrix.Create(m_nRowCount, m_nColumnCount);
		double** ppData = matrix.GetData();

		for ( int i = 0; i < m_nRowCount; ++i )
		{
			for ( int j = 0; j < m_nRowCount; ++j )
			{
				double d = CalDeterminan(j, i) * pow(-1, i + j);
				ppData[i][j] = d / dDet;
			}
		}
	}
}

double CMatrix::CalDeterminan(int nDeleteRow, int nDeleteColumn)
{
	double dRet = 0;

	if ( m_nColumnCount == m_nRowCount )
	{
		// 求排列
		CArray<int, int> Sarr;
		CArray<int, int> Aarr;

		int nN = m_nColumnCount;
		if ( nDeleteColumn >= 0 && nDeleteRow >= 0 )
			nN -= 1;

		Aarr.SetSize(nN);
		for ( int i = 0; i < nN; ++i )
		{
			Sarr.Add(i);
		}

		int nCount = 1;
		for ( int k = nN; k > 1; --k )
		{
			nCount *= k;
		}

		CArray<int, int>* pRelt = new CArray<int, int> [nCount];

		int nReltIndex = 0;
		Permutation(Aarr, Sarr, 0, pRelt, nReltIndex);

		// 计算行列式
		for ( i = 0; i < nCount; ++i )
		{
			// 计算逆序数
			int n = 0;
			double d = 1;
			for ( int j = 0; j < nN; ++j )
			{
				int nRow = j;
				int nCol = pRelt[i].ElementAt(j);

				if ( nDeleteColumn >= 0 )
				{
					// 有删除某列要求时
					if ( nRow >= nDeleteRow )
						++nRow;

					if ( nCol >= nDeleteColumn )
						++nCol;
				}
				d *= m_ppData[nRow][nCol];

				for ( int k = j + 1; k < nN; ++k )
				{
					if ( pRelt[i].ElementAt(j) > pRelt[i].ElementAt(k) )
						++n;
				}
			}

			d *= pow(-1, n);

			dRet += d;
		}

		delete []pRelt;
	}

	return dRet;
}

void CMatrix::Permutation(CArray<int, int>& A, CArray<int, int>& S, int nIndex, CArray<int, int>* pRelt, int& nReltIndex)
{
	int nSize = S.GetSize();
	if ( 0 == nSize )
	{
		// 输出A
		pRelt[nReltIndex++].Copy(A);
	}
	else
	{
		for ( int k = 0; k < nSize; ++k )
		{
			A[nIndex] = S[k];
			
			CArray<int, int> SS;
			SS.Copy(S);
			SS.RemoveAt(k);

			Permutation(A, SS, nIndex + 1, pRelt, nReltIndex);
		}
	}
}

void CMatrix::Div(double d)
{
	for ( int i = 0; i < m_nRowCount; ++i )
		for ( int k = 0; k < m_nColumnCount; ++k )
			m_ppData[i][k] /= d;
}

bool CMatrix::Mul(const CMatrix& matrix, CMatrix* matrixRelt)
{
	bool bRet = false;

	if ( m_nColumnCount == matrix.m_nRowCount )
	{
		CMatrix matrixNew;
		matrixNew.Create(m_nRowCount, matrix.m_nColumnCount);

		for ( int i = 0; i < m_nRowCount; ++i )
		{
			for ( int k = 0; k < matrix.m_nColumnCount; ++k )
			{
				double d = 0;
				for ( int j = 0; j < m_nColumnCount; ++j )
				{
					d += m_ppData[i][j] * matrix.m_ppData[j][k];
				}

				matrixNew.m_ppData[i][k] = d;
			}
		}

		if ( NULL != matrixRelt )
			*matrixRelt = matrixNew;
		else
			*this = matrixNew;

		bRet = true;
	}

	return bRet;
}

bool CMatrix::Add(const CMatrix& matrix, CMatrix* matrixRelt)
{
	bool bRet = false;

	if ( m_nRowCount == matrix.m_nRowCount && m_nColumnCount == matrix.m_nColumnCount )
	{
		CMatrix matrixNew;
		matrixNew.Create(m_nRowCount, m_nColumnCount);

		for ( int i = 0; i < m_nRowCount; ++i )
		{
			for ( int j = 0; j < m_nColumnCount; ++j )
			{
				matrixNew.m_ppData[i][j] = m_ppData[i][j] + matrix.m_ppData[i][j];
			}
		}

		if ( NULL != matrixRelt )
			*matrixRelt = matrixNew;
		else
			*this = matrixNew;

		bRet = true;
	}

	return bRet;
}

bool CMatrix::Sub(const CMatrix& matrix, CMatrix* matrixRelt)
{
	bool bRet = false;

	if ( m_nRowCount == matrix.m_nRowCount && m_nColumnCount == matrix.m_nColumnCount )
	{
		CMatrix matrixNew;
		matrixNew.Create(m_nRowCount, m_nColumnCount);

		for ( int i = 0; i < m_nRowCount; ++i )
		{
			for ( int j = 0; j < m_nColumnCount; ++j )
			{
				matrixNew.m_ppData[i][j] = m_ppData[i][j] - matrix.m_ppData[i][j];
			}
		}

		if ( NULL != matrixRelt )
			*matrixRelt = matrixNew;
		else
			*this = matrixNew;

		bRet = true;
	}

	return bRet;
}

void CMatrix::Trans()
{
	CMatrix matrixNew;
	matrixNew.Create(m_nColumnCount, m_nRowCount);

	for ( int i = 0; i < m_nRowCount; ++i )
	{
		for ( int j = 0; j < m_nColumnCount; ++j )
		{
			matrixNew.m_ppData[j][i] = m_ppData[i][j];
		}
	}

	*this = matrixNew;
}

void CMatrix::UpdateColData(double* pData, int nColIndex)
{
	for ( int i = 0; i < m_nRowCount; ++i )
	{
		m_ppData[i][nColIndex] = pData[i];
	}
}

void CMatrix::UpdateRowData(double* pData, int nRowIndex)
{
	for ( int i = 0; i < m_nColumnCount; ++i )
	{
		m_ppData[nRowIndex][i] = pData[i];
	}
}

double CMatrix::GetData(int nRowIndex, int nColIndex)
{
	return m_ppData[nRowIndex][nColIndex];
}

double CMatrix::GetColEuclideanDis(const CMatrix& matrix)
{
	double dRet = 0;

	for ( int i = 0; i < m_nColumnCount; ++i )
	{
		double d = 0;
		for ( int j = 0; j < m_nRowCount; ++j )
		{
			d += (m_ppData[j][i] - matrix.m_ppData[j][i]) * (m_ppData[j][i] - matrix.m_ppData[j][i]);
		}
		dRet += sqrt(d);
	}

	return dRet;
}
