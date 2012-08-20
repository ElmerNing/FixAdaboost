#include "stdafx.h"
#include "PathX.h"

const tstring PathX::separator = _T("\\");
const tstring PathX::extension = _T(".");

tstring PathX::GetFileName(const tstring& path)
{
	tstring outFileName;
	GetFileName(path, outFileName);
	return outFileName;
}

void PathX::GetFileName(const tstring& path, tstring& outFileName)
{
	tstring::size_type pos = path.find_last_of(separator);
	if (pos == tstring::size_type(-1))
	{
		outFileName = path;
		return;
	}
	else if(pos == tstring::size_type(path.length()-1))
	{
		outFileName.clear();
		return;
	}
	else
	{
		outFileName = path.substr(pos+1, path.length());
		return;
	}
}

tstring PathX::GetFileNameNoExtension(const tstring& path)
{
	tstring outFileName;
	GetFileNameNoExtension(path, outFileName);
	return outFileName;
}

void PathX::GetFileNameNoExtension(const tstring& path, tstring& outFileName)
{
	tstring fileName;
	GetFileName(path, fileName);
	tstring::size_type pos = fileName.find_last_of(extension);
	if (pos == tstring::size_type(-1))
	{
		outFileName = path;
		return;
	}
	else if(pos == tstring::size_type(0))
	{
		outFileName.clear();
		return;
	}
	else
	{
		outFileName = fileName.substr(0, pos);
		return;
	}
}

tstring PathX::GetDirectory(const tstring& path)
{
	tstring outDirectory;
	GetDirectory(path, outDirectory);
	return outDirectory;
}

void PathX::GetDirectory(const tstring& path, tstring& outDirectory)
{
	tstring::size_type pos = path.find_last_of(separator);
	if (pos == tstring::size_type(-1))
	{
		outDirectory.clear();
		return;
	}
	else
	{
		outDirectory = path.substr(0, pos);
		return;
	}
}

tstring PathX::GetExtension(const tstring& path)
{
	tstring outExtension;
	GetExtension(path, outExtension);
	return outExtension;
}

void PathX::GetExtension(const tstring& path, tstring& outExtension)
{
	tstring::size_type pos = path.find_last_of(extension);
	if (pos == tstring::size_type(-1))
	{
		outExtension.clear();
		return;
	}
	else if(pos == tstring::size_type(path.length()-1))
	{
		outExtension.clear();
		return;
	}
	else
	{
		outExtension = path.substr(pos+1, path.length());
		return;
	}
}

tstring PathX::GetRandomFileName(int nameSize)
{
	tstring outRandomName;
	GetRandomFileName(nameSize, outRandomName);
	return outRandomName;
}

void PathX::GetRandomFileName(int nameSize, tstring& outRandomNam )
{
	TCHAR* num = new TCHAR[nameSize+1];
	for (int i=0; i<nameSize; i++)
	{
		int r = rand()%26;
		wchar_t A = _T('A');
		num[i] = A+r;
	}
	num[nameSize] = _T('\0');
	outRandomNam = num;
	delete[] num;
}
