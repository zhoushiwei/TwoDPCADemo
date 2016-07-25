// Bmp8.h: interface for the CBmp8 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMP8_H__99640DCA_21EF_4E4C_B85E_06E086E53C25__INCLUDED_)
#define AFX_BMP8_H__99640DCA_21EF_4E4C_B85E_06E086E53C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct TImage
{
	BYTE*	m_pData;
	int		m_nWidth;
	int		m_nHeight;
};

class CBmp8
{
public:
	CBmp8();
	virtual ~CBmp8();

	void Create(int nWidth, int nHeight);
	bool LoadBMP(LPCTSTR lpFileName);

	const TImage* GetImage() { return &m_tImage; }

	int GetWidth() { return m_tImage.m_nWidth; }
	int GetHeight() { return m_tImage.m_nHeight; }

private:
	void Free();

private:
	TImage	m_tImage;
};

#endif // !defined(AFX_BMP8_H__99640DCA_21EF_4E4C_B85E_06E086E53C25__INCLUDED_)
