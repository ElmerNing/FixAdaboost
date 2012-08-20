#include "StdAfx.h"
#include "Model.h"
#include "..\IO\IO.h"


Model::Model(void)
{
}

Model::~Model(void)
{
}

bool Model::Read(const tstring& folder)
{
	vector<tstring> files;
	m_model.clear();
	DirectoryX::GetFiles(folder, _T("*.txt"), files);

	for (vector<tstring>::iterator it = files.begin(); it != files.end(); it++)
	{
		CBoostedCommittee boost;
		if(boost.LoadFromFile(*it))
			m_model.push_back(boost);
	}

	if (m_model.size() <= 0)
		return false;
	else
		return true;
}

bool Model::Write( const tstring& folder )
{
	if (DirectoryX::Exists(folder))
	{
		if(!DirectoryX::Delete(folder))
			return false;
	}

	if(!DirectoryX::CreateDirectory(folder))
		return false;

	for (int i=0; i<(int)m_model.size(); i++)
	{
		tostringstream s;
		s<<folder<<_T("\\")<<m_model[i].m_label<<_T(".txt");
		m_model[i].WriteToFile(s.str());
	}

	return true;
}

double Model::Predict(const SampleSet& inSamples, SampleSet* outNoRecSamples, SampleSet* outErrRecSamples, SampleSet* outRepRecSamples)
{
	if (outNoRecSamples != NULL)
		outNoRecSamples->Clear();

	int n = inSamples.N();
	if (n == 0)
		return 0.0;

	int rightNum = n;
	for (int i=0; i<n; i++)
	{
		int label = 0;
		Model::RecResult result = Predict( inSamples.Samples()[i], label);

		if (result == RESULT_NO_REC)
		{
			rightNum--;
			if(outNoRecSamples != NULL)
			{
				outNoRecSamples->AddOne(inSamples.Samples()[i], inSamples.Labels()[i], inSamples.Dim());
			}
		}

		if (result == RESULT_SUC_REC && label !=  inSamples.Labels()[i])
		{
			rightNum--;
			if(outErrRecSamples != NULL)
			{
				outErrRecSamples->AddOne(inSamples.Samples()[i], inSamples.Labels()[i], inSamples.Dim());
			}
		}

		if (result == RESULT_REP_REC)
		{
			rightNum--;
			if(outRepRecSamples != NULL)
			{
				outRepRecSamples->AddOne(inSamples.Samples()[i], inSamples.Labels()[i], inSamples.Dim());
			}
		}
	}
	return (double)rightNum/n;
}

Model::RecResult Model::Predict(const double* inSample, int& outRegLabel)
{
	int size = m_model.size();
	int posNum = 0;

	if (size < 0)
		return RESULT_NO_REC;

	outRegLabel = -1;
	
	double* sample = const_cast<double*>(inSample);
	for (int i=0; i<size; i++)
	{
		if(m_model[i].Predict(sample) > 0)
		{
			outRegLabel = m_model[i].m_label;
			posNum++;
		}
	}

	short sampletest[16*9];
	for (int i = 0; i < 16*9; i++)
	{
		sampletest[i] = (short)sample[i];
	}
	if (posNum == 0)
		return RESULT_NO_REC;
	else if (posNum > 1)
		return RESULT_REP_REC;
	else
		return RESULT_SUC_REC;
}
