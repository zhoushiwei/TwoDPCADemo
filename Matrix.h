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

	// �����ֵ
	void CalMean(CMatrix& matrix);

	// ����Э����
	void CalCovariance(CMatrix& matrix);

	// ���������
	void CalInverse(CMatrix& matrix);

	// ��������ʽ
	double CalDeterminan(int nDeleteRow = -1, int nDeleteColumn = -1);

	// ����˷������matrixReltΪNULL�������ڸ�����
	bool Mul(const CMatrix& matrix, CMatrix* matrixRelt = NULL);

	// ����������һ��ֵ
	void Div(double d);

	// �������
	bool Sub(const CMatrix& matrix, CMatrix* matrixRelt = NULL);

	// ����ӷ�
	bool Add(const CMatrix& matrix, CMatrix* matrixRelt = NULL);

	// ��ת��
	void Trans();

	// ����һ������
	void UpdateColData(double* pData, int nColIndex);

	// ����һ������
	void UpdateRowData(double* pData, int nRowIndex);

	double GetData(int nRowIndex, int nColIndex);

	double** GetData() { return m_ppData; }

	CSize GetSize();

	// ���پ���
	void Destroy();

	// �����֮���ŷʽ����
	double GetColEuclideanDis(const CMatrix& matrix);

private:
	// ��ȡ����
	void Permutation(CArray<int, int>& A, CArray<int, int>& S, int nIndex, CArray<int, int>* pRelt, int& nReltIndex);

private:
	int			m_nRowCount;
	int			m_nColumnCount;
	double**	m_ppData;
};

#endif // !defined(AFX_MATRIX_H__01D0E309_9B5D_4863_84A1_1FF95652B922__INCLUDED_)
