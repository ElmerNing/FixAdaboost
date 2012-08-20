#include "StdAfx.h"
#include "OpenFileDlg.h"


OpenFileDlg::OpenFileDlg(void)
:Filter(_T("*.*"))
,Tittle(_T(""))
,InitialDir(_T(""))
{
}


OpenFileDlg::~OpenFileDlg(void)
{
}

bool OpenFileDlg::ShowDlg(HWND hParent)
{
	TCHAR szPathname[_MAX_PATH];
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner = hParent;
	ofn.nMaxFile = _countof(szPathname);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;// | OFN_ALLOWMULTISELECT ;

	//////////////////////////////////////////////////////////
	//A buffer containing pairs of null-terminated filter strings. 
	//The last string in the buffer must be terminated by two NULL characters.
	//reference from msdn.
	int lptstrfilterSize = this->Filter.size();
	LPTSTR lptstrfilter = new TCHAR[lptstrfilterSize + 3];
	lstrcpy(lptstrfilter,this->Filter.c_str());
	lptstrfilter[lptstrfilterSize+1] = _T('\0');
	lptstrfilter[lptstrfilterSize+2] = _T('\0');
	ofn.lpstrFilter = lptstrfilter;
	/////////////////////////////////////////////////////////

	ofn.lpstrTitle = this->Tittle.c_str();
	ofn.lpstrInitialDir = this->InitialDir.c_str();
	szPathname[0] = _T('\0');
	ofn.lpstrFile = szPathname;

	BOOL bOk = GetOpenFileName(&ofn);
	delete[] lptstrfilter;
	
	if (!bOk)
		FileName = _T("");
	else
		FileName = szPathname;
	return bOk != 0;
}

