#include "stdafx.h"
#include "DirectoryX.h"
#include "Shlwapi.h"

#pragma comment(lib, "Shlwapi.lib")

void DirectoryX::GetFiles( const tstring& path, vector<tstring>& outFiles )
{
	return GetFiles(path, _T(""), outFiles );
}

void DirectoryX::GetFiles( const tstring& path, const tstring& searchPatten, vector<tstring>& outFiles )
{
	//初始化
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	tstring searchPath;
	ZeroMemory(&findFileData, sizeof(WIN32_FIND_DATA));
	outFiles.clear();
	if (searchPatten == _T(""))
		searchPath = path + _T("\\*");
	else
		searchPath = path + _T("\\") + searchPatten;

	//寻找文件
	hFind = FindFirstFile(searchPath.c_str(), &findFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	
	do 
	{
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			outFiles.push_back( path + _T("\\") + findFileData.cFileName );
		}
		
	} while (FindNextFile(hFind, &findFileData));

	FindClose(hFind);
	return;
}

void DirectoryX::GetDirectories( const tstring& path, vector<tstring>& outDirs )
{
	GetDirectories(path, _T(""), outDirs);
}

void DirectoryX::GetDirectories( const tstring& path, const tstring& searchPatten, vector<tstring>& outDirs )
{
	//初始化
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	tstring searchPath;
	ZeroMemory(&findFileData, sizeof(WIN32_FIND_DATA));
	outDirs.clear();
	if (searchPatten == _T(""))
		searchPath = path + _T("\\*");
	else
		searchPath = path + _T("\\") + searchPatten;

	//寻找文件
	hFind = FindFirstFile(searchPath.c_str(), &findFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 

	do 
	{
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			tstring fileName = findFileData.cFileName;
			if (!(fileName == _T(".") || fileName == _T("..")))
			{
				outDirs.push_back( path + _T("\\") + fileName );
			}
		}

	} while (FindNextFile(hFind, &findFileData));

	FindClose(hFind);
	return;
}

bool DirectoryX::Exists( tstring path )
{
	return 0 != PathIsDirectory(path.c_str());
}

bool DirectoryX::Delete( tstring path )
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_NOERRORUI;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;

	/////////////////////////////////////////////////////
	//SHFILEOPSTRUCT::pFrom
	//Note  This string must be double-null terminated.
	//reference from msdn.
	int fromSize = path.size();
	LPTSTR lptstrFrom = new TCHAR[fromSize + 3];
	lstrcpy(lptstrFrom, path.c_str());
	lptstrFrom[fromSize+1] = _T('\0');
	lptstrFrom[fromSize+2] = _T('\0');
	FileOp.pFrom = lptstrFrom;
	////////////////////////////////////////////////////

	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}
