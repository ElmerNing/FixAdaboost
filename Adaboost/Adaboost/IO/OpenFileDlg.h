#pragma once
class OpenFileDlg
{
public:
	OpenFileDlg(void);
	~OpenFileDlg(void);

	//显示对话框
	bool ShowDlg(HWND hParent = NULL);
	//设置过滤器
	tstring Filter;
	//设置对话框标题
	tstring Tittle;
	//设置初始加载目录
	tstring InitialDir;
	//获取选中的文件
	tstring FileName;
};

