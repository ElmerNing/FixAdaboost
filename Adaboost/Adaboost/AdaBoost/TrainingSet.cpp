#include "StdAfx.h"
#include "TrainingSet.h"

static int compare( const void *arg1, const void *arg2 )
{
	return (int)(((SetNode*)arg1)->Value - ((SetNode*)arg2)->Value);
}

TrainingSet::TrainingSet(void)
{
	m_dim = 0;
	m_n = 0;
}


TrainingSet::~TrainingSet(void)
{
	Clear();
}

void TrainingSet::Clear()
{
	for (int i=0; i<(int)this->m_samples.size(); i++)
	{
		delete[] m_samples[i];
	}
	m_samples.clear();
	m_labels.clear();
	m_dim = 0;
	m_n = 0;
}

void TrainingSet::LoadFromSampleSet( const SampleSet& sampleSet )
{
	Clear();
	m_labels = sampleSet.Labels();
	m_dim = sampleSet.Dim();
	m_n = sampleSet.N();
	for (int dim=0; dim<m_dim; dim++)
	{
		SetNode* nodes = new SetNode[m_n];
		for (int n=0; n<m_n; n++)
		{
			nodes[n].N = n;
			nodes[n].Value = sampleSet.Samples()[n][dim];
		}
		qsort(nodes, m_n, sizeof(SetNode), compare);
		m_samples.push_back(nodes);
	}
}

bool TrainingSet::LoadFromFile( const tstring& path )
{
	SampleSet set;
	if(!set.Read(path))
		return false;

	this->LoadFromSampleSet(set);

	set.Clear();

	return true;
}
