#pragma once
class PathX
{
public:

	//��ȡ�ļ���
	static tstring GetFileName(const tstring& path);
	static void GetFileName(const tstring& path, tstring& outFileName);

	//��ȡ����չ�����ļ���
	static tstring GetFileNameNoExtension(const tstring& path);
	static void GetFileNameNoExtension(const tstring& path, tstring& outFileName);

	//��ȡĿ¼��
	static tstring GetDirectory(const tstring& path);
	static void GetDirectory(const tstring& path, tstring& outDirectory);

	//��ȡ��չ��
	static tstring GetExtension(const tstring& path);
	static void GetExtension(const tstring& path, tstring& outExtension);

	//��ȡһ���������
	static tstring GetRandomFileName(int nameSize);
	static void GetRandomFileName(int nameSize, tstring& outRandomName);

private:
	// ���� "\"
	const static tstring separator;
	// ���� "."
	const static tstring extension;
};

