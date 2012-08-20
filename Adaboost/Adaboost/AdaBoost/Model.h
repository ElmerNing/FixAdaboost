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

	//��ȡmodel�ļ�
	bool Read(const tstring& folder);

	//��Modelд���ļ�
	bool Write(const tstring& folder);

	//ʶ��һ��������,���ش�����,outErrorSamples���ش�������
	double Predict(const SampleSet& inSamples, SampleSet* outNoRecSamples = NULL, SampleSet* outErrRecSamples = NULL, SampleSet* outRepRecSamples = NULL);

	//ʶ��һ������,����ֵ�����Ƿ�ʶ��ɹ���outRegLabel����ʶ�𵽵ı�ǩ
	enum RecResult{
		RESULT_NO_REC = 0,	//ʶ��ʧ��
		RESULT_SUC_REC = 1,	//ʶ��ɹ�
		RESULT_REP_REC = 2,	//��ʶ
	};
	RecResult Model::Predict(const double* inSample, int& outRegLabel);
	
	//�ж�model�Ƿ����
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
