#pragma once
#include "SampleSet.h"

typedef struct 
{
	//������ĳһ���ֵ
	double Value;
	//indexΪ������������
	int N;
} SetNode;

//ѵ��ר�õ�������,Ԥ�ȶ�����������ÿһά�Ƚ���������
class TrainingSet
{
public:
	TrainingSet(void);
	~TrainingSet(void);

	//���������
	const vector<SetNode*>& Samples() const { return m_samples; }

	//�����������ǩ
	const vector<int>& Labels() const { return m_labels; }

	//���������ά��
	int Dim() const { return m_dim; }

	//�����������������
	int N() const { return m_n; }

	//���������
	void Clear();

	//��SampleSet��ת��ΪTrainingSet
	void LoadFromSampleSet( const SampleSet& sampleSet );

	//���ļ��м���������
	bool LoadFromFile(const tstring& path);

private:
	//m_sample[dim]��������������dimά�ȵ������ļ���,���ҽ���������
	vector<SetNode*> m_samples;
	//m_labels[n]�����n������������
	vector<int> m_labels;
	//������������ά��
	int m_dim;
	//�����������ܸ���
	int m_n;
};


