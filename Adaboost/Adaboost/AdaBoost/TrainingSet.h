#pragma once
#include "SampleSet.h"

typedef struct 
{
	//样本中某一点的值
	double Value;
	//index为该样本的索引
	int N;
} SetNode;

//训练专用的样本集,预先对所有样本的每一维度进行了排序
class TrainingSet
{
public:
	TrainingSet(void);
	~TrainingSet(void);

	//获得样本集
	const vector<SetNode*>& Samples() const { return m_samples; }

	//获得样本集标签
	const vector<int>& Labels() const { return m_labels; }

	//获得样本集维数
	int Dim() const { return m_dim; }

	//获得样本集的样本数
	int N() const { return m_n; }

	//清空样本集
	void Clear();

	//从SampleSet中转换为TrainingSet
	void LoadFromSampleSet( const SampleSet& sampleSet );

	//从文件中加载样本集
	bool LoadFromFile(const tstring& path);

private:
	//m_sample[dim]代表所用样本第dim维度的特征的集合,并且进行了排序
	vector<SetNode*> m_samples;
	//m_labels[n]代表第n个样本的特征
	vector<int> m_labels;
	//代表样本的总维数
	int m_dim;
	//代表样本的总个数
	int m_n;
};


