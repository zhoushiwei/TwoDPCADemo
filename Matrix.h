// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__01D0E309_9B5D_4863_84A1_1FF95652B922__INCLUDED_)
#define AFX_MATRIX_H__01D0E309_9B5D_4863_84A1_1FF95652B922__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMatrix;
typedef void (CMatrix::* PFUN)(CArray<int, int>& A);

class CMatrix
{
public:
	CMatrix();
	CMatrix(const CMatrix& matrix);
	virtual ~CMatrix();

	CMatrix& operator= (const CMatrix& matrix);

 	void Create(int nRowCount, int nColumnCount, double** ppData = NULL);
	void Create(int nRowCount, int nColumnCount, BYTE* pData);

	// 计算均值
	void CalMean(CMatrix& matrix);

	// 计算协方差
	void CalCovariance(CMatrix& matrix);

	// 计算逆矩阵
	void CalInverse(CMatrix& matrix);

	// 计算行列式
	double CalDeterminan(int nDeleteRow = -1, int nDeleteColumn = -1);

	// 矩阵乘法，如果matrixRelt为NULL则结果存于该类中
	bool Mul(const CMatrix& matrix, CMatrix* matrixRelt = NULL);

	// 除法，除以一个值
	void Div(double d);

	// 矩阵减法
	bool Sub(const CMatrix& matrix, CMatrix* matrixRelt = NULL);

	// 矩阵加法
	bool Add(const CMatrix& matrix, CMatrix* matrixRelt = NULL);

	// 求转置
	void Trans();

	// 更新一列数据
	void UpdateColData(double* pData, int nColIndex);

	// 更新一行数据
	void UpdateRowData(double* pData, int nRowIndex);

	double GetData(int nRowIndex, int nColIndex);

	double** GetData() { return m_ppData; }

	CSize GetSize();

	// 销毁矩阵
	void Destroy();

	// 获得列之间的欧式距离
	double GetColEuclideanDis(const CMatrix& matrix);

private:
	// 求取排列
	void Permutation(CArray<int, int>& A, CArray<int, int>& S, int nIndex, CArray<int, int>* pRelt, int& nReltIndex);

private:
	int			m_nRowCount;
	int			m_nColumnCount;
	double**	m_ppData;
};

#endif // !defined(AFX_MATRIX_H__01D0E309_9B5D_4863_84A1_1FF95652B922__INCLUDED_)
