#pragma once
class PathX
{
public:

	//获取文件名
	static tstring GetFileName(const tstring& path);
	static void GetFileName(const tstring& path, tstring& outFileName);

	//获取无扩展名的文件名
	static tstring GetFileNameNoExtension(const tstring& path);
	static void GetFileNameNoExtension(const tstring& path, tstring& outFileName);

	//获取目录名
	static tstring GetDirectory(const tstring& path);
	static void GetDirectory(const tstring& path, tstring& outDirectory);

	//获取扩展名
	static tstring GetExtension(const tstring& path);
	static void GetExtension(const tstring& path, tstring& outExtension);

	//获取一个随机名字
	static tstring GetRandomFileName(int nameSize);
	static void GetRandomFileName(int nameSize, tstring& outRandomName);

private:
	// 常量 "\"
	const static tstring separator;
	// 常量 "."
	const static tstring extension;
};

