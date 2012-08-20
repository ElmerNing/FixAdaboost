#include "BoostPredict.h"
#include <stdio.h>

static short BoostPredictOneTree(short* feature,const WeakClassifier* weakClassFier,const short* weight,int splitNum)
{
	short stumpIndex = 0;
	do 
	{
		short dim = weakClassFier[stumpIndex].dim;
		short thr = weakClassFier[stumpIndex].thr;
		if (feature[dim] >= thr)
		{
			//右分支
			short rightW = (weakClassFier[stumpIndex].leftW_and_rightW & 0xff);
			if (rightW!=0xff)
			{
				return weight[rightW];
			}
			else
				stumpIndex++;
		}
		else
		{
			//左分支
			short leftW = (weakClassFier[stumpIndex].leftW_and_rightW >> 8);
			if (leftW!=0xff)
			{
				return weight[leftW];
			}
			else
				stumpIndex++;
		}
	} while (1);
}

short BoostPredictOne(short* feature, StrongClassifier strgclsf )
{
	int i=0;
#ifdef CHIP_DM42
	int sum = 0;
#else
	long sum = 0;
#endif

	for (i=0; i<strgclsf.weakSize; i++)
	{
		short temp = BoostPredictOneTree(feature, strgclsf.weakClassifier, strgclsf.weight, strgclsf.splitNum);
		sum += temp;
		strgclsf.weakClassifier += strgclsf.splitNum;
		strgclsf.weight += (strgclsf.splitNum+1);
	}

	if (sum >0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

short BoostPredict(short* feature, AdaBoostModel model)
{
	int i;
	int regNum = 0;
	short label = -1;

	for (i=0; i<model.size; i++)
	{
		if (BoostPredictOne(feature, model.strongClassifier[i]) > 0)
		{
			label = model.strongClassifier[i].label;
			regNum++;
		}
	}
	if (regNum == 1)
		return label;
	else
		return -1;
}