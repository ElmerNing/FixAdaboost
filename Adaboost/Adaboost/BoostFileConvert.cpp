#include "StdAfx.h"
#include "BoostFileConvert.h"

#include "IO\IO.h"
#include "AdaBoost\AdaBoost.h"
#include "..\AdaBoostPredict\BoostPredict.h"

/******************************************************************************\
* static function declarations
\******************************************************************************/
//把一个弱分类器转为字符串
static tstring WeakClassifierToString(const WeakClassifier& wc);
//返回weight的归一化因子
static double WeightToFix(const vector <double>& inoutWeights);
//根据CBoostedCommittee生成代码
static void BoostWriteData(const CBoostedCommittee& boost, 
	const tostringstream& weakClassifierString,
	const tostringstream& weightString,
	int splitNum);

////////////////Split为2的函数////////////////////
//转换一棵树
static void BoostWriteOneSplit2(const vector <CSPHypothesis>::iterator& iHypotheses, 
	const vector <double>::iterator& iWeights,
	tostringstream& weakClassifierString,
	tostringstream& weightString,
	double scalepara);
//转换全部的树
static int BoostWriteDataSplit2(const CBoostedCommittee& boost, 
	tostringstream& weakClassifierString,
	tostringstream& weightString);


/******************************************************************************\
* static function definitions
\******************************************************************************/
static tstring WeakClassifierToString(const WeakClassifier& wc)
{
	tostringstream os;
	os<<_T("	{");
	os<<wc.dim<<_T(",");
	os<<wc.thr<<_T(",");
	os<<hex;
	os<<_T("0x")<<wc.leftW_and_rightW;
	os<<dec;
	os<<_T("},");
	return os.str();
}

static double WeightToFix(const vector <double>& inoutWeights)
{
	union{
		double dbl;
		unsigned long long ll;
	}maxWeight,minWeight;
	maxWeight.dbl = abs(inoutWeights[0]);
	minWeight.dbl = abs(inoutWeights[1]);
	for (vector<double>::const_iterator it = inoutWeights.begin(); it != inoutWeights.end(); it++)
	{
		double temp = abs(*it);
		if (temp > maxWeight.dbl)
			maxWeight.dbl = temp;
		if(temp < minWeight.dbl)
			minWeight.dbl = *it;
	}

	//以下根据double为IEEE标准的格式来操作
	long long Bias = 1023;
	int max_exp = ((maxWeight.ll&0x7fffffffffffffff)>>52)-1023;
	int min_exp = ((minWeight.ll&0x7fffffffffffffff)>>52)-1023;
//	验证	max_exp与min_exp正确性的代码,maxW与minW约等于maxWeight与minWeight
// 	double max_frac = (double)(maxWeight.ll&0x000fffffffffffff)/(double)0x0010000000000000;
// 	double min_frac = (double)(minWeight.ll&0x000fffffffffffff)/(double)0x0010000000000000;
// 	max_frac = max_frac+1;
// 	min_frac = min_frac+1;
// 	double maxW = max_frac*pow((double)2, (double)max_exp);
// 	double minW = min_frac*pow((double)2, (double)min_exp);
	
	if (abs(max_exp - min_exp) > 14)//确保最小值,转为整数后,至少有两位有效数字
	{
		exit(0);
	}

	return pow((double)2, (double)(14-max_exp));
}

static int BoostWriteData(const CBoostedCommittee& boost, 
	tostringstream& weakClassifierString,
	tostringstream& weightString,
	int splitNum)
{
	if (splitNum == 2)
	{
		return BoostWriteDataSplit2(boost, weakClassifierString, weightString);
	}
	else
	{
		exit(0);
	}
}

//////////////////SPlit为2///////////////////////////
static void BoostWriteOneSplit2(const vector <CSPHypothesis>::const_iterator& iHypotheses, 
	const vector <double>::const_iterator& iWeights,
	tostringstream& weakClassifierString,
	tostringstream& weightString,
	double scalepara)
{
	//输出weight
	for (int i=0; i<3; i++)
	{
		weightString<<_T('\t')<<(short)(iWeights[i]*scalepara)<<_T(",")<<endl;
	}

	//输出第一个stump
	WeakClassifier wc0;
	wc0.dim = (unsigned short)iHypotheses[0].m_vDims[0];
	wc0.thr = (short)(iHypotheses[0].m_vThresholds[0]+1);
	if (iHypotheses[0].m_vSignums[0] > 0)
	{
		//设置左右节点的Weight的索引值，当相应节点的Weight为0xff时候，表明当前节点下还有分支。
		//左节点有分支，右节点无分支并且其Weight索引为0。
		wc0.leftW_and_rightW = 0xff00;
	}
	else
	{
		wc0.leftW_and_rightW = 0x00ff;
	}
	weakClassifierString<<WeakClassifierToString(wc0)<<endl;

	//输出第二个stump
	WeakClassifier wc1;
	wc1.dim = (unsigned short)iHypotheses[1].m_vDims[1];
	wc1.thr = (short)(iHypotheses[1].m_vThresholds[1]+1);
	
	if (iHypotheses[1].m_vSignums[1] < 0)
		wc1.leftW_and_rightW = 0x0102;//左边节点Weight索引为1，右边节点的为2；
	else
		wc1.leftW_and_rightW = 0x0201;
	weakClassifierString<<WeakClassifierToString(wc1)<<endl;
}

static int BoostWriteDataSplit2(const CBoostedCommittee& boost, 
	tostringstream& weakClassifierString,
	tostringstream& weightString)
{
	int totalCnt = boost.m_vWeights.size()/3;
	int cnt = 0;
	double scalePara = WeightToFix(boost.m_vWeights);
	vector<CSPHypothesis>::const_iterator iHypotheses = boost.m_vHypotheses.begin();
	vector<double>::const_iterator iWeights = boost.m_vWeights.begin();
	do 
	{
		weakClassifierString<<_T("//")<<cnt<<endl;
		weightString<<_T("//")<<cnt<<endl;
		BoostWriteOneSplit2(iHypotheses, iWeights, weakClassifierString, weightString, scalePara);
		weakClassifierString<<endl;
		weightString<<endl;

		iHypotheses += 3;
		iWeights += 3;
		cnt++;
	} while (cnt<totalCnt);
	return totalCnt;
}

static void BoostGenerateHeader(const tstring& fileFolder, const vector<tstring>& files,const tstring& saveFile)
{
	tofstream outfile(saveFile);
	outfile<<"#ifndef BOOST_DATA_H_"<<endl;
	outfile<<"#define BOOST_DATA_H_"<<endl<<endl;
	outfile<<"#ifdef __cplusplus"<<endl;
	outfile<<"extern \"C\"{"<<endl;
	outfile<<"#endif"<<endl<<endl;

	outfile<<_T("#include \"BoostPredict.h\"")<<endl<<endl;
	for (vector<tstring>::const_iterator filePath=files.cbegin(); filePath!=files.cend(); filePath++)
	{
		tstring filename = PathX::GetFileNameNoExtension(*filePath);

		outfile<<"///////////////"<<filename<<"///////////////"<<endl;
		outfile<<"extern WeakClassifier g_weakClassFier_"<<filename<<"[];"<<endl;
		outfile<<"extern short g_weight_"<<filename<<"[];"<<endl;
		outfile<<"extern const int g_weakSize_"<<filename<<";"<<endl;
		outfile<<"extern const int g_splitNum_"<<filename<<";"<<endl;
		outfile<<"extern const int g_label_"<<filename<<";"<<endl<<endl;
	}
	outfile<<"#ifdef __cplusplus"<<endl;
	outfile<<"};"<<endl;
	outfile<<"#endif	//__cplusplus"<<endl;
	outfile<<"#endif	//BOOST_DATA_H_"<<endl;
	outfile.close();
}

static void BoostGenerateStrongClsf(tofstream& ofs, const tstring& fileName)
{
	tstring name = PathX::GetFileNameNoExtension(fileName);
	tstring weakClsfName = _T("g_weakClassifier_") + name;
	tstring weightName = _T("g_weight_") + name;
	tstring weakSizeName = _T("g_weakSize_") + name;
	tstring splitName = _T("g_splitNum_") + name;
	tstring labelName = _T("g_label_") + name;

	ofs<<"\t{"<<endl;
	ofs<<"\t\t"<<weakClsfName<<",//weakClassifier"<<endl;
	ofs<<"\t\t"<<weightName<<",//weight"<<endl;
	ofs<<"\t\t"<<weakSizeName<<",//weakSize"<<endl;
	ofs<<"\t\t"<<splitName<<",//split"<<endl;
	ofs<<"\t\t"<<labelName<<",//label"<<endl;
	ofs<<"\t},"<<endl;
}

static void BoostGenerateDataSrc(const tstring& fileFolder, const vector<tstring>& files,const tstring& saveFile)
{
	FileX::WriteAllText(saveFile, _T("#include \"..\\BoostPredict.h\""));
	for (vector<tstring>::const_iterator filePath=files.cbegin(); filePath!=files.cend(); filePath++)
	{
		vector<tstring> lines;
		FileX::ReadAllLines(*filePath, lines);
		FileX::AppendAllLines(saveFile, lines);
		FileX::Delete(*filePath);
	}

	tofstream ofs;
	tstring name = PathX::GetFileNameNoExtension(saveFile);
	tstring strongSize = _T("strongClsfSize");
	ofs.open(saveFile, ios::app);
	ofs<<_T("#define ")<<strongSize<<_T(" ")<<files.size()<<endl;
	ofs<<_T("const static StrongClassifier g_strongClsf_")<<name<<_T("[] = {")<<endl;
	
	for (int i=0; i<files.size(); i++)
	{
		BoostGenerateStrongClsf(ofs, files[i]);
	}
	ofs<<_T("};")<<endl;
	
	ofs<<_T("const AdaBoostModel g_model_")<<name<<_T(" = {")
		<<_T("g_strongClsf_")<<name<<_T(",")
		<<strongSize
		<<_T("};")<<endl;
	ofs.close();
}

/******************************************************************************\
* global function definitions
\******************************************************************************/
bool BoostFileConvert(const tstring& srcFilePath, const tstring& saveFilePath)
{
	CBoostedCommittee boost;
	if (!boost.LoadFromFile(srcFilePath))
		return false;

	tostringstream weakClassifierString;
	tostringstream weightString;

	//write array declare
	tstring name = PathX::GetFileNameNoExtension(srcFilePath);
	tstring weakClsfName = _T("g_weakClassifier_") + name;
	tstring weightName = _T("g_weight_") + name;
	tstring weakSizeName = _T("g_weakSize_") + name;
	tstring splitName = _T("g_splitNum_") + name;
	tstring labelName = _T("g_label_") + name;
	tstring strongClsfName = _T("g_strongClassifier_")+name;

	weakClassifierString<<_T("const static WeakClassifier ")<<weakClsfName<<_T("[] = {")<<endl;
	weightString<<_T("const static short ")<<weightName<<_T("[] = {")<<endl;

	//write weakClassifierString data
	int splitNum = boost.m_split;
	int weakSize = BoostWriteData(boost, weakClassifierString, weightString, splitNum);

	//write end of array
	weakClassifierString<<_T("};")<<endl;
	weightString<<_T("};")<<endl;

	//save file;
	tofstream out(saveFilePath);

	out<<_T("//////////////////////////////////////////")
		<<name
		<<_T("//////////////////////////////////////////")<<endl;

	out<<weakClassifierString.str()<<weightString.str()<<endl;

	out<<_T("#define ")<<weakSizeName<<_T(" ")<<weakSize<<endl;
	out<<_T("#define ")<<splitName<<_T(" ")<<splitNum<<endl;
	out<<_T("#define ")<<labelName<<_T(" ")<<boost.m_label<<endl;


	out<<_T("//////////////////////////////////////////")
		<<_T("end of ")<<name
		<<_T("//////////////////////////////////////////")<<endl<<endl;
	
	out.flush();
	out.close();
	return true;
}

void BoostFolderConvert(const tstring& srcFileFolder, const tstring& saveFileFolder, const tstring& modelName)
{
	vector<tstring> srcFiles, dstFiles;
	DirectoryX::GetFiles(srcFileFolder,_T("*.txt"),srcFiles);

	for (vector<tstring>::iterator filePath=srcFiles.begin(); filePath!=srcFiles.end(); filePath++)
	{
		if (!DirectoryX::Exists(saveFileFolder))
		{
			if (!DirectoryX::CreateDirectory(saveFileFolder))
				return;
		}

		tstring saveFilePath = saveFileFolder + _T("\\") + PathX::GetFileNameNoExtension(*filePath) + _T(".c");
		dstFiles.push_back(saveFilePath);
		BoostFileConvert(*filePath,saveFilePath);
	}

	tstring mergeHeaderPath = saveFileFolder + _T("\\") + modelName + _T(".h");
	tstring mergeDataPath = saveFileFolder + _T("\\") + modelName + _T(".c");
	
	//BoostGenerateHeader(saveFileFolder, dstFiles, mergeHeaderPath);
	BoostGenerateDataSrc(saveFileFolder, dstFiles, mergeDataPath);
}


