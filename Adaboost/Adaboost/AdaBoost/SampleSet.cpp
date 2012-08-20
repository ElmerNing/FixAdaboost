#include "StdAfx.h"
#include "SampleSet.h"

static __inline void SplitX(const tstring& str, const tstring& delimiter,  vector<tstring>& dest)
{
	size_t pos = 0, found = 0;
	tstring tmpstr;
	while ( found != string::npos )
	{
		found = str.find(delimiter, pos);
		tmpstr = tstring(str, pos, found - pos);
		if (tmpstr == _T(""))
			return;
		dest.push_back(tmpstr);
		pos = found + 1;
	}
}

SampleSet::SampleSet(void)
{
	m_dim = 0;
	m_n = 0;
}

SampleSet::~SampleSet(void)
{
	Clear();
}

void SampleSet::Clear()
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

void SampleSet::AddOne( double* sample, int label, int dim )
{
	double * newsample = new double[dim];
	for (int i=0; i<dim; i++)
	{
		newsample[i] = sample[i];
	}
	this->m_labels.push_back(label);
	this->m_samples.push_back(newsample);
	this->m_n++;
	if (this->m_dim != 0)
	{
		this->m_dim = dim < this->m_dim ? dim : this->m_dim;
	}
	else
		this->m_dim = dim;
}

bool SampleSet::Read( tstring path)
{
	tifstream ifs(path);
	tstring line;
	Clear();
	do 
	{
		vector<tstring> elms;
		if ( ifs.eof() )
			break;

		getline(ifs, line).gcount();

		SplitX(line, _T(" "), elms );
		int size = elms.size();
		if (size<3)
			break;

		m_n++;
		if (m_dim != 0)
			m_dim = (size-1) < m_dim ? (size-1) : m_dim;
		else
			m_dim = size-1;

		double* sample = new double[size-1];
		for (int i=0; i<size-1; i++)
		{
			sample[i] = stod(elms[i+1],NULL);
		}
		m_samples.push_back(sample);
		m_labels.push_back(stoi(elms[0],NULL));

	} while (true);

	if (m_n == 0)
	{
		Clear();
		return false;
	}
	else
		return true;
}

void SampleSet::Write( tstring path )
{
	tofstream ofs(path);
	
	for (int i=0; i<m_n; i++)
	{
		ofs<<m_labels[i];
		for (int d=0; d<m_dim; d++)
		{
			ofs<<_T(" ")<<m_samples[i][d];
		}
        ofs<<endl;
	}
	ofs.close();
}
