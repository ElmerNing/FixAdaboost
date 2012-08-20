#pragma once
class DirectoryX
{
public:
	//��ȡĿ¼�µ������ļ�, searchPattenΪͨ���
	static void GetFiles(const tstring& path, vector<tstring>& outFiles);
	static void GetFiles(const tstring& path, const tstring& searchPatten, vector<tstring>& outFiles);

	//��ȡĿ¼�µ�����Ŀ¼, searchPattenΪͨ���
	static void GetDirectories(const tstring& path, vector<tstring>& outDirs);
	static void GetDirectories(const tstring& path, const tstring& searchPatten, vector<tstring>& outDirs);

	//ɾ��ָ��Ŀ¼
	static bool Delete(tstring path);
	
	//ָ��Ŀ¼�Ƿ����
	static bool Exists(tstring path);
	
	//����һ��Ŀ¼
	static bool DirectoryX::CreateDirectory(const tstring& path )
	{
		return 0 != ::CreateDirectory(path.c_str(), NULL);
	}
};

