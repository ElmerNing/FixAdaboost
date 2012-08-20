#pragma once
class SampleSet
{
public:
	SampleSet(void);
	~SampleSet(void);

	//��ȡ��������
	const vector<double*>& Samples() const { return m_samples; }

	//��ȡ�����ı�ǩ
	const vector<int>& Labels() const { return m_labels; }

	//��ȡ��������ά��
	int Dim() const { return m_dim; }

	//��ȡ�����ĸ���
	int N() const { return m_n; }
	
	//�������������һ������
	void AddOne( double* sample, int label, int dim );

	//���������
	void Clear();

	//���ļ��ж���������
	bool Read(tstring path);

	//��������д���ļ�
	void Write(tstring path);

private:
	vector<double*> m_samples;
	vector<int> m_labels;
	int m_dim;
	int m_n;

};

