#include "StdAfx.h"
#include "FileX.h"


bool FileX::Delete( const tstring& path )
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION;
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

bool FileX::Exists( const tstring& path )
{
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	ZeroMemory(&findFileData, sizeof(WIN32_FIND_DATA));

	//я╟урнд╪Ч
	hFind = FindFirstFile(path.c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE && (!(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)))
		return true;
	else
		return false;
}

bool FileX::ReadAllBytes( const tstring& path, vector<byte>& outBuff )
{
	outBuff.clear();
	HANDLE hdev = CreateFile(path.c_str(),GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hdev == INVALID_HANDLE_VALUE)
		return false;

	LARGE_INTEGER length;
	if(FALSE == GetFileSizeEx(hdev,&length) || length.HighPart != 0)
	{
		CloseHandle(hdev);
		return false;
	}

	DWORD hasread = 0;
	bool returnValue = true;
	outBuff.resize(length.LowPart);
	if(FALSE == ReadFile(hdev, outBuff._Myfirst, length.LowPart, &hasread, NULL) || hasread != length.LowPart )
	{
		returnValue = false;
		outBuff.clear();
	}
	CloseHandle(hdev);
	return returnValue;
}

bool FileX::WriteAllBytes( const tstring& path, byte buff[], DWORD size )
{
	if (buff == NULL || size == NULL)
		return false;

	HANDLE hdev = CreateFile(path.c_str(),GENERIC_WRITE,FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hdev == INVALID_HANDLE_VALUE)
		return NULL;

	WriteFile(hdev, buff, size, &size, NULL);
	CloseHandle(hdev);
	return true;
}

bool FileX::ReadAllLines( const tstring& path, vector<tstring>& outStrings )
{
	tifstream ifs(path);
	if (!ifs)
		return false;

	tstring line;
	outStrings.clear();
	do 
	{
		getline(ifs, line);
		outStrings.push_back(line);
	} while (!ifs.eof());

	ifs.close();
	return true;
}

bool FileX::WriteAllLines( const tstring& path, const vector<tstring>& strings )
{
	tofstream ofs(path);
	if (!ofs)
		return false;

	for (int i=0; i<(int)strings.size(); i++)
	{
		ofs<<strings[i]<<endl;
	}

	ofs.close();
	return true;
}

bool FileX::AppendAllLines( const tstring& path, const vector<tstring>& strings )
{
	tofstream ofs;
	ofs.open(path, ios::app);
	if (!ofs)
		return false;

	ofs<<endl;
	for (int i=0; i<(int)strings.size(); i++)
	{
		ofs<<strings[i]<<endl;
	}

	ofs.close();
	return true;
}

bool FileX::ReadAllText( const tstring& path, tstring& outString )
{
	vector<tstring> strings;
	if (ReadAllLines(path, strings))
	{
		outString.clear();
		for (vector<tstring>::iterator it = strings.begin(); it != strings.end(); it++)
		{
			outString += (*it);
		}
		return true;
	}
	else
		return false;
	

}

bool FileX::WriteAllText( const tstring& path, const tstring& string )
{
	tofstream out(path);
	if (!out)
		return false;
	out<<string;
	out.close();
	return true;
}

bool FileX::Copy( const tstring& srcPath, const tstring& dstPath )
{
	return TRUE == CopyFile(srcPath.c_str(), dstPath.c_str(), FALSE);
}

bool FileX::Move( const tstring& srcPath, const tstring& outPath )
{
	return TRUE == MoveFile(srcPath.c_str(), srcPath.c_str());
}





