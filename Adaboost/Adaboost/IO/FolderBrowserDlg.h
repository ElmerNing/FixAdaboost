#pragma once
class FolderBrowserDlg
{
public:
	FolderBrowserDlg(void);
	~FolderBrowserDlg(void);
	
	//��ʾ�Ի���
	bool ShowDlg(HWND hParent = NULL);
	//���öԻ������
	tstring Tittle;
	//���ó�ʼĿ¼
	tstring InitialDir;
	//��ȡѡ��Ŀ¼
	tstring SelectedPath;

private:
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
};

