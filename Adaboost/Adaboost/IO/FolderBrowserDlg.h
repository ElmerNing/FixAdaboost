#pragma once
class FolderBrowserDlg
{
public:
	FolderBrowserDlg(void);
	~FolderBrowserDlg(void);
	
	//显示对话框
	bool ShowDlg(HWND hParent = NULL);
	//设置对话框标题
	tstring Tittle;
	//设置初始目录
	tstring InitialDir;
	//获取选中目录
	tstring SelectedPath;

private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
};

