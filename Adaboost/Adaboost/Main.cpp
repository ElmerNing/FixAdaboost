// Adaboost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BoostFileConvert.h"
#include "AdaBoost\AdaBoost.h"
#include "IO\IO.h"

//static variable definition
static Model s_model;

//static function
static void ConvertMerge()
{
	FolderBrowserDlg dlg;
	if (dlg.ShowDlg())
	{
		BoostFolderConvert(dlg.SelectedPath, dlg.SelectedPath, _T("Sample"));
		//BoostFolderMerge(dlg.SelectedPath, name);
		tcout<<_T("finish!")<<endl;
	}
}

static void LoadModel()
{
	FolderBrowserDlg dlg;
	if (dlg.ShowDlg())
	{
		if (s_model.Read(dlg.SelectedPath))
			tcout<<_T("finish!")<<endl;
		else
			tcout<<_T("failed!")<<endl;
	}
}

static void Pridict()
{
	if (!s_model.IsLoaded())
	{
		tcout<<_T("not load model!")<<endl;
	}

	OpenFileDlg dlg;
	dlg.Filter = _T("*.datset");
	if (dlg.ShowDlg())
	{
		SampleSet set;
		
		if(!set.Read(dlg.FileName))
			tcout<<"file bda format!"<<endl;

		SampleSet errorSet;
		double rate = s_model.Predict(set, &errorSet);

		tstring dir = PathX::GetDirectory(dlg.FileName);
		errorSet.Write(dir + _T("\\error.datset"));

		tcout<<"rate:"<<rate<<" errorNum:"<<errorSet.N()<<" totalNum:"<<set.N()<<endl;
	}
}

static void Train()
{
	OpenFileDlg dlg;
	if (!dlg.ShowDlg())
		return;

	int splitNum = 2;
	int iteroter = 300;
	int negLabel = -1;

	TrainingSet set;
	set.LoadFromFile(dlg.FileName);

	Training::Train(set, s_model, splitNum, iteroter, negLabel);

	s_model.Write(PathX::GetDirectory(dlg.FileName) + _T("\\_model"));

	tcout<<"finish"<<endl;
}

static int imageCheck(byte*image, int width, int height, int stride)
{
	short subWidth = 65, subHeight = 10;
	double* feature = new double[subWidth * subHeight];
	for (int xoff=0;xoff <= width-subWidth; xoff++)
	{
		for (int yoff=0; yoff <= height-subHeight; yoff++)
		{
			for (int i=0; i<subWidth; i++)
			{
				for (int j=0; j<subHeight; j++)
				{
					feature[j*subWidth + i] = image[(j+yoff)*stride + i+xoff];
				}
			}
			int result = 0;
			if (s_model.Predict(feature, result) != Model::RESULT_SUC_REC)
			{
				continue;
			}		
			if(result == 3)
			{
				delete[] feature;
				return 1;
			}
		}
	}
	delete[] feature;
	return 0;
}

static void test()
{
// 	vector<tstring> strings;
// 	FileX::ReadAllLines(_T("C:\\123.c"), strings);
// 	FileX::AppendAllLines(_T("C:\\123.c"), strings);
	s_model.Read(_T("D:\\LiuTuDeng\\bbs\\_model"));

	vector<tstring> files;
	FolderBrowserDlg dlg;
	if (!dlg.ShowDlg())
	{
		return;
	}
	DirectoryX::GetFiles(dlg.SelectedPath, _T("*.dat"), files);
	int count = 0;
	int negCount = 0;
	for (int i=0; i<files.size(); i++)
	{
		vector<byte> buff;
		FileX::ReadAllBytes(files[i], buff);
		byte* pbuff = buff._Myfirst;
		int width = *((int*)pbuff);
		pbuff += 4;
		int height = *((int*)pbuff);
		pbuff += 4;
		int stride = 0;
		if (width%4!=0)
			stride = (width+4)&0x7FFFFFFC;
		else
			stride = width;

		tstring pngName = PathX::GetFileNameNoExtension( files[i] ) + _T(".png");
		tstring datName = PathX::GetFileNameNoExtension( files[i] ) + _T(".dat");
		tstring srcdir = PathX::GetDirectory(files[i]) + _T("\\");
		tstring dstdir = _T("C:\\abc\\");
		tstring cpypngcmd = _T("copy \"") + srcdir + pngName + _T("\"") + 
			_T(" ") + _T("\"") + dstdir + pngName + _T("\"");
		tstring cpydatcmd = _T("copy \"") + srcdir + pngName + _T("\"") + 
			_T(" ") + _T("\"") + dstdir + datName + _T("\"");
		dstdir.c_str();
		if (!DirectoryX::Exists(dstdir))
			DirectoryX::CreateDirectory(dstdir);

		if (imageCheck(pbuff, width, height, stride) != 0)
		{
			_wsystem(cpypngcmd.c_str());
			_wsystem(cpydatcmd.c_str());
			negCount++;
		}
		else
			count++;
	}
	tcout<<"neg"<<negCount<<endl;
	tcout<<"pos"<<count<<endl;
}

//global fuction : main
int _tmain(int argc, _TCHAR* argv[])
{
	do 
	{
		tstring cmd;
		tcin>>cmd;
		if (cmd == _T("-c"))
			ConvertMerge();
		if (cmd == _T("-l"))
			LoadModel();
		if (cmd == _T("-p"))
			Pridict();
		if (cmd == _T("-t"))
			Train();
		if (cmd == _T("-test"))
			test();
		else if (cmd == _T("quit"))
			break;
	} while (true);
}

