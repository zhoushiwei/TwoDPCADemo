// TwoDPCADemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TwoDPCADemo.h"
#include "TPCA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment (lib, "libmatlb.lib")
#pragma comment (lib, "libmx.lib")
#pragma comment (lib, "libmatpm.lib")    // c++ maths library
#pragma comment (lib, "libmmfile.lib")

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		CTPCA tpca;
		// ��������Ϊ400������������40�֣�ȡ10��������
		for ( int nDim = 1; nDim <= 30; ++nDim )
		{
			if ( tpca.LoadData("D:\\�γ��������\\ģʽʶ����\\ORL", 400, 40, nDim) )
			{
				// ÿ��������ѵ������ռ��5��������Ϊ��������
				tpca.Train(5);
				double dCorrectRate = tpca.Test();

				cout<<"dȡֵ��"<<nDim<<"   ������ȷ��:     "<<dCorrectRate * 100<<"%"<<endl;
			}
		}
	}

	return nRetCode;
}


