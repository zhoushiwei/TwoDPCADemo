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
		// 总样本数为400个，样本类型40种，取10个主分量
		for ( int nDim = 1; nDim <= 30; ++nDim )
		{
			if ( tpca.LoadData("D:\\课程相关资料\\模式识别技术\\ORL", 400, 40, nDim) )
			{
				// 每类样本中训练样本占总5个，其余为检验样本
				tpca.Train(5);
				double dCorrectRate = tpca.Test();

				cout<<"d取值："<<nDim<<"   分类正确率:     "<<dCorrectRate * 100<<"%"<<endl;
			}
		}
	}

	return nRetCode;
}


