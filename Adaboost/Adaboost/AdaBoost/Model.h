#pragma once
#include "BoostedCommittee.h"
#include "SampleSet.h"

class Training;

class Model
{
public:
	Model(void);
	~Model(void);

	friend class Training;

	//读取model文件
	bool Read(const tstring& folder);

	//把Model写到文件
	bool Write(const tstring& folder);

	//识别一个样本集,返回错误率,outErrorSamples返回错误样本
	double Predict(const SampleSet& inSamples, SampleSet* outNoRecSamples = NULL, SampleSet* outErrRecSamples = NULL, SampleSet* outRepRecSamples = NULL);

	//识别一个样本,返回值代表是否识别成功，outRegLabel返回识别到的标签
	enum RecResult{
		RESULT_NO_REC = 0,	//识别失败
		RESULT_SUC_REC = 1,	//识别成功
		RESULT_REP_REC = 2,	//重识
	};
	RecResult Model::Predict(const double* inSample, int& outRegLabel);
	
	//判断model是否加载
	bool IsLoaded()
	{
		if (m_model.size() > 0)
			return true;
		else
			return false;
	}

private:
	vector<CBoostedCommittee> m_model;

};
