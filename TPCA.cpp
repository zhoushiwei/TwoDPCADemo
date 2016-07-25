/******************************************************************************
* FileName		       : TPCA.cpp
* Description          : implementation of the CTPCA class
* Author               : Jesse Gu at NJUST
* Date Of Creation     : 2010-06-10

*------------------------------------------------------------------------------
* Copyright (C) 2010 Gu Jicheng
*------------------------------------------------------------------------------
******************************************************************************/

#include "stdafx.h"
#include "TPCA.h"
#include "matlab.hpp"
#include "bmp8.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTPCA::CTPCA()
{
	m_nClassTrainSampleNum = 5;
	m_nTotalImgNum = 0;
	m_nClassNum = 0;
	m_nClassImgNum = 0;

	m_nValidPrjDim = 50;

	m_pDataItem = NULL;
}

CTPCA::~CTPCA()
{
	delete []m_pDataItem;
}

void CTPCA::Free()
{
	if ( NULL != m_pDataItem )
	{
		delete []m_pDataItem;
		m_pDataItem = NULL;
	}
}

bool CTPCA::LoadData(LPCTSTR lpPath, int nTotalImgNum, int nClassNum, int nValidPrjDim)
{
	bool bRet = false;

	Free();

	m_nTotalImgNum = nTotalImgNum;
	m_nClassNum = nClassNum;
	m_nClassImgNum = m_nTotalImgNum / m_nClassNum;
	m_nValidPrjDim = nValidPrjDim;

	m_pDataItem = new TDataItem[nClassNum];
	for ( int i = 0; i < nClassNum; ++i )
	{
		m_pDataItem[i].m_pBmpData = new TData[m_nClassImgNum];
		m_pDataItem[i].m_nBmpCount = m_nClassImgNum;
	}

	CString str;
	CBmp8 bmp8;
	for ( i = 0; i < m_nClassNum; ++i )
	{
		int nBase = i * m_nClassImgNum;
		for ( int j = 0; j < m_nClassImgNum; ++j )
		{
			str.Format("%s\\ORL%03d.bmp", lpPath, nBase + j + 1);
			bmp8.LoadBMP(str);
			const TImage* p = bmp8.GetImage();
			m_pDataItem[i].m_pBmpData[j].m_oBmp.Create(p->m_nHeight, p->m_nWidth, p->m_pData);
			m_pDataItem[i].m_pBmpData[j].m_oFeature.Create(p->m_nHeight, m_nValidPrjDim);

			m_nWidth = p->m_nWidth;
			m_nHeight = p->m_nHeight;
		}
	}

	m_oMatrixProj.Create(m_nWidth, m_nValidPrjDim);

	bRet = true;

	return bRet;
}

void CTPCA::Train(int nClassTrainSample)
{
	m_nClassTrainSampleNum = nClassTrainSample;

	CMatrix matrixAvgA,matrixGt, matrix, matrixTrans;
	matrixGt.Create(m_nWidth, m_nWidth);
	matrixAvgA.Create(m_nHeight, m_nWidth);

	// 求图像均值
	for ( int i = 0; i < m_nClassNum; ++i )
	{
		for ( int j = 0; j < m_nClassTrainSampleNum; ++j )
		{
			matrixAvgA.Add(m_pDataItem[i].m_pBmpData[j].m_oBmp);
		}
	}
	matrixAvgA.Div(m_nClassTrainSampleNum * m_nClassNum);

	// 求Gt
	for ( i = 0; i < m_nClassNum; ++i )
	{
		for ( int j = 0; j < m_nClassTrainSampleNum; ++j )
		{
			m_pDataItem[i].m_pBmpData[j].m_oBmp.Sub(matrixAvgA, &matrix);

			matrixTrans = matrix;
			matrixTrans.Trans();

			matrixTrans.Mul(matrix);
			matrixGt.Add(matrixTrans);
		}
	}
	matrixGt.Div(m_nClassTrainSampleNum * m_nClassNum);

	// 求特征值和特征向量
	double* d = new double[m_nWidth * m_nWidth];
	double** ppData = matrixGt.GetData();
	for ( int j = 0; j < m_nWidth; ++j )
	{
		int nBase = j * m_nWidth;
		for ( int k = 0; k < m_nWidth; ++k )
		{
			d[nBase + k] = ppData[k][j];
		}
	}

	mwArray A(m_nWidth, m_nWidth, d);
	mwArray arrVal,arrVec;
	arrVec = eig(&arrVal,A);

	double* pVal = new double[m_nWidth];
	double* pvt = mxGetPr(arrVal.GetData());
	double* pVec = mxGetPr(arrVec.GetData());
	double dMax = 0;
	for ( j = 0; j < m_nWidth; ++j )
	{
		int nIndex = (j + 1) * (m_nWidth + 1) - (m_nWidth + 1);
		pVal[j] = pvt[nIndex];
	}
	// 排序找到前m_nValidPrjDim个最大特征
	for ( j = 0; j < m_nValidPrjDim; ++j )
	{
		double dMax = 0;
		int nMaxIndex;
		for ( int k = 0; k < m_nWidth; ++k )
		{
			if ( pVal[k] > dMax )
			{
				dMax = pVal[k];
				nMaxIndex = k;
			}
		}
		pVal[nMaxIndex] = 0;

		m_oMatrixProj.UpdateColData(pVec + nMaxIndex * m_nWidth, j);
	}

	// 计算所有样本图像的特征
	for ( i = 0; i < m_nClassNum; ++i )
	{
		for ( int j = 0; j < m_nClassImgNum; ++j )
		{
			m_pDataItem[i].m_pBmpData[j].m_oBmp.Mul(m_oMatrixProj, &m_pDataItem[i].m_pBmpData[j].m_oFeature);
		}
	}

	delete []d;
	delete []pVal;
}

double CTPCA::Test()
{
	int nCorrectCount = 0;

	for ( int i = 0; i < m_nClassNum; ++i )
	{
		for ( int j = m_nClassTrainSampleNum; j < m_nClassImgNum; ++j )
		{
			double dMinDis = 9999999999999;
			int nClassID = -1;
			for ( int m = 0; m < m_nClassNum; ++m )
			{
				for ( int n = 0; n < m_nClassTrainSampleNum; ++n )
				{
					double d = m_pDataItem[i].m_pBmpData[j].m_oFeature.GetColEuclideanDis(m_pDataItem[m].m_pBmpData[n].m_oFeature);
					if ( d < dMinDis )
					{
						dMinDis = d;
						nClassID = m;
					}
				}
			}

			// 检验类别是否相同
			if ( i == nClassID )
			{
				++nCorrectCount;
			}
		}
	}

	return (double)nCorrectCount / (m_nTotalImgNum - m_nClassNum * m_nClassTrainSampleNum);
}

