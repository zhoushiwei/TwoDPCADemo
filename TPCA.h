/******************************************************************************
* FileName		       : TPCA.h
* Description          : interface for the CTPCA class
* Author               : Jesse Gu at NJUST
* Date Of Creation     : 2010-06-10

*------------------------------------------------------------------------------
* Copyright (C) 2010 Gu Jicheng
*------------------------------------------------------------------------------
******************************************************************************/

#if !defined(AFX_TPCA_H__573D4D93_BDAD_469D_95B7_68E917056143__INCLUDED_)
#define AFX_TPCA_H__573D4D93_BDAD_469D_95B7_68E917056143__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Matrix.h"

// 在ORL数据库上实验
class CTPCA
{
public:
	CTPCA();
	virtual ~CTPCA();

	bool LoadData(LPCTSTR lpPath, int nTotalImgNum, int nClassNum, int nValidPrjDim);
	void Train(int nClassTrainSample);
	double Test();

private:
	void Free();

private:
	int m_nClassTrainSampleNum;
	int m_nTotalImgNum;
	int m_nClassNum;
	int m_nClassImgNum;

	int	m_nValidPrjDim;

	int m_nWidth;
	int m_nHeight;

	struct TData
	{
		CMatrix		m_oBmp;
		CMatrix		m_oFeature;
	};

	struct TDataItem
	{
		TDataItem()
		{
			m_pBmpData = NULL;
		}
		~TDataItem()
		{
			delete []m_pBmpData;
		}

		TData*		m_pBmpData;
		int			m_nBmpCount;
	};

	TDataItem*	m_pDataItem;
	CMatrix		m_oMatrixProj;
};


#endif // !defined(AFX_TPCA_H__573D4D93_BDAD_469D_95B7_68E917056143__INCLUDED_)
