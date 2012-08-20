#pragma once
class SampleSet
{
public:
	SampleSet(void);
	~SampleSet(void);

	//获取样本集合
	const vector<double*>& Samples() const { return m_samples; }

	//获取样本的标签
	const vector<int>& Labels() const { return m_labels; }

	//获取样本集的维度
	int Dim() const { return m_dim; }

	//获取样本的个数
	int N() const { return m_n; }
	
	//向样本集中添加一个样本
	void AddOne( double* sample, int label, int dim );

	//清空样本集
	void Clear();

	//从文件中读入样本集
	bool Read(tstring path);

	//把样本集写入文件
	void Write(tstring path);

private:
	vector<double*> m_samples;
	vector<int> m_labels;
	int m_dim;
	int m_n;

};

