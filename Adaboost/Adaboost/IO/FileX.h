#pragma once
class FileX
{
public:
	//读取文件的所有字节,成功返回true
	static bool ReadAllBytes(const tstring& path, vector<byte>& outBuff);

	//把指定的字节数组写入指定文件中,如果目标文件不存在存在,则会创建之.成功返回true.
	static bool WriteAllBytes(const tstring& path, byte buff[], DWORD size);

	//读取文件的每一行,成功返回true
	static bool ReadAllLines(const tstring& path, vector<tstring>& outStrings);

	//把每一行字符串写进指定文件中,如果文件不存在,则会创建之,成功返回true.
	static bool WriteAllLines(const tstring& path, const vector<tstring>& strings);

	//把每一行字符串追加到指定文件中,如果文件不存在,则会创建之并从头开始追加,成功返回true.
	static bool AppendAllLines(const tstring& path, const vector<tstring>& strings);

	//读取文件到一个字符串.成功返回true.
	static bool ReadAllText(const tstring& path, tstring& outString);

	//把字符串写进指定文件中,如果文件不存在,则会创建之.成功返回true.
	static bool WriteAllText(const tstring& path, const tstring& string);

	//把字符串追加进指定文件中,如果文件不存在,则会创建之并从头开始追加.成功返回true.
	static bool AppendText(const tstring& path, const tstring& string);

	//删除指定文件,成功返回true
	static bool Delete(const tstring& path);

	//指定文件是否存在,存在返回true
	static bool Exists(const tstring& path);

	//复制文件至指定的路径
	static bool Copy(const tstring& srcPath, const tstring& outPath);
	
	//移动文件到指定路径
	static bool Move(const tstring& srcPath, const tstring& outPath);

};

