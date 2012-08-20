#include "StdAfx.h"
#include "FolderBrowserDlg.h"
#include <ShlObj.h>

FolderBrowserDlg::FolderBrowserDlg(void):
SelectedPath(_T("")),
Tittle(_T("")),
InitialDir(_T(""))
{
}


FolderBrowserDlg::~FolderBrowserDlg(void)
{
}


bool FolderBrowserDlg::ShowDlg(HWND hParent)
{
	//��ʼ��
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner = hParent;
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
	bi.lpszTitle = Tittle.c_str();
	bi.lpfn = FolderBrowserDlg::BrowseCallbackProc;
	bi.lParam = (LPARAM)const_cast<TCHAR*>(InitialDir.c_str());

	//�������
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
	{
		SelectedPath = _T("");
		return false;
	}

	//��ȡѡ��·��
	TCHAR path[MAX_PATH];
	SHGetPathFromIDList(pidl, path);
	SelectedPath = path;

	//�ͷ���Դ
	CoTaskMemFree(pidl);
	return true;
}

int CALLBACK FolderBrowserDlg::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}