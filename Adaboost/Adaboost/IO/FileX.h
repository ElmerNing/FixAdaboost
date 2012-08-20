#pragma once
class FileX
{
public:
	//��ȡ�ļ��������ֽ�,�ɹ�����true
	static bool ReadAllBytes(const tstring& path, vector<byte>& outBuff);

	//��ָ�����ֽ�����д��ָ���ļ���,���Ŀ���ļ������ڴ���,��ᴴ��֮.�ɹ�����true.
	static bool WriteAllBytes(const tstring& path, byte buff[], DWORD size);

	//��ȡ�ļ���ÿһ��,�ɹ�����true
	static bool ReadAllLines(const tstring& path, vector<tstring>& outStrings);

	//��ÿһ���ַ���д��ָ���ļ���,����ļ�������,��ᴴ��֮,�ɹ�����true.
	static bool WriteAllLines(const tstring& path, const vector<tstring>& strings);

	//��ÿһ���ַ���׷�ӵ�ָ���ļ���,����ļ�������,��ᴴ��֮����ͷ��ʼ׷��,�ɹ�����true.
	static bool AppendAllLines(const tstring& path, const vector<tstring>& strings);

	//��ȡ�ļ���һ���ַ���.�ɹ�����true.
	static bool ReadAllText(const tstring& path, tstring& outString);

	//���ַ���д��ָ���ļ���,����ļ�������,��ᴴ��֮.�ɹ�����true.
	static bool WriteAllText(const tstring& path, const tstring& string);

	//���ַ���׷�ӽ�ָ���ļ���,����ļ�������,��ᴴ��֮����ͷ��ʼ׷��.�ɹ�����true.
	static bool AppendText(const tstring& path, const tstring& string);

	//ɾ��ָ���ļ�,�ɹ�����true
	static bool Delete(const tstring& path);

	//ָ���ļ��Ƿ����,���ڷ���true
	static bool Exists(const tstring& path);

	//�����ļ���ָ����·��
	static bool Copy(const tstring& srcPath, const tstring& outPath);
	
	//�ƶ��ļ���ָ��·��
	static bool Move(const tstring& srcPath, const tstring& outPath);

};

