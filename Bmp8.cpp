// Bmp8.cpp: implementation of the CBmp8 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bmp8.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBmp8::CBmp8()
{
	m_tImage.m_pData = NULL;
	m_tImage.m_nWidth = 0;
	m_tImage.m_nHeight = 0;
}

CBmp8::~CBmp8()
{
	Free();
}

void CBmp8::Create(int nWidth, int nHeight)
{
	m_tImage.m_nWidth = nWidth;
	m_tImage.m_nHeight = nHeight;

	m_tImage.m_pData = new BYTE[m_tImage.m_nWidth * m_tImage.m_nHeight];

	memset(m_tImage.m_pData, 0, m_tImage.m_nWidth * m_tImage.m_nHeight);
}

void CBmp8::Free()
{
	delete []m_tImage.m_pData;
	m_tImage.m_nWidth = 0;
	m_tImage.m_nHeight = 0;
}

bool CBmp8::LoadBMP(LPCTSTR lpFileName)
{
	bool bRet = false;
	
	FILE* pFile = fopen(lpFileName, "rb");
	if ( NULL != pFile )
	{
		Free();

		BITMAPFILEHEADER	tBmfh;
		BITMAPINFOHEADER	tBmih;

		// 解析文件头
		fread(&tBmfh, sizeof(BITMAPFILEHEADER), 1, pFile);

		if ( tBmfh.bfType == 0x4D42 )
		{
			// 解析信息头
			fread(&tBmih, sizeof(BITMAPINFOHEADER), 1, pFile);

			if ( 40 == tBmih.biSize && BI_RGB == tBmih.biCompression )
			{
				m_tImage.m_nWidth = tBmih.biWidth;
				m_tImage.m_nHeight = tBmih.biHeight;

				int nPadSize = (((tBmih.biWidth + 3) >> 2) << 2) - tBmih.biWidth;
				int nImageSize = tBmih.biWidth * tBmih.biHeight;
				m_tImage.m_pData = new BYTE[nImageSize];

				if ( 8 == tBmih.biBitCount )
				{
					fseek(pFile, sizeof(RGBQUAD) * 256, SEEK_CUR);

					for ( int i = tBmih.biHeight - 1; i >= 0; --i )
					{
						fread(m_tImage.m_pData + tBmih.biWidth * i, tBmih.biWidth, 1, pFile);

						fseek(pFile, nPadSize, SEEK_CUR);
					}

					bRet = true;
				}
			}
		}

		fclose(pFile);
	}

	return bRet;
}
