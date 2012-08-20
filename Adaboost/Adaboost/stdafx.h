// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <Windows.h>

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

#ifdef UNICODE
typedef wstring tstring;

typedef wiostream tiostream;
typedef wistream tistream;
typedef wostream tostream;

typedef wfstream tfstream;
typedef wifstream tifstream;
typedef wofstream tofstream;

typedef wstringstream tstringstream;
typedef wistringstream tistringstream;
typedef wostringstream tostringstream;

#else
typedef string tstring;

typedef iostream tiostream;
typedef istream tistream;
typedef ostream tostream;

typedef wiostream tiostream;
typedef wistream tistream;
typedef wostream tostream;

typedef fstream tfstream;
typedef ifstream tifstream;
typedef ofstream tofstream;

typedef stringstream tstringstream;
typedef istringstream tistringstream;
typedef ostringstream tostringstream;
#endif

#ifdef UNICODE
#define  tcin wcin
#define  tcout wcout
#else
#define tcin cin
#define tcout cout
#endif
