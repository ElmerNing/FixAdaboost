#pragma once
class OpenFileDlg
{
public:
	OpenFileDlg(void);
	~OpenFileDlg(void);

	//��ʾ�Ի���
	bool ShowDlg(HWND hParent = NULL);
	//���ù�����
	tstring Filter;
	//���öԻ������
	tstring Tittle;
	//���ó�ʼ����Ŀ¼
	tstring InitialDir;
	//��ȡѡ�е��ļ�
	tstring FileName;
};

