#pragma once
class DirectoryX
{
public:
	//获取目录下的所有文件, searchPatten为通配符
	static void GetFiles(const tstring& path, vector<tstring>& outFiles);
	static void GetFiles(const tstring& path, const tstring& searchPatten, vector<tstring>& outFiles);

	//获取目录下的所有目录, searchPatten为通配符
	static void GetDirectories(const tstring& path, vector<tstring>& outDirs);
	static void GetDirectories(const tstring& path, const tstring& searchPatten, vector<tstring>& outDirs);

	//删除指定目录
	static bool Delete(tstring path);
	
	//指定目录是否存在
	static bool Exists(tstring path);
	
	//创建一个目录
	static bool DirectoryX::CreateDirectory(const tstring& path )
	{
		return 0 != ::CreateDirectory(path.c_str(), NULL);
	}
};

