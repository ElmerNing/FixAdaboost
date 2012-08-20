// BoostedCommittee.cpp: implementation of the CBoostedCommittee class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BoostedCommittee.h"

#include <iomanip>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoostedCommittee::CBoostedCommittee()
:m_split(2)
,m_label(0)
{

}

CBoostedCommittee::~CBoostedCommittee()
{

}

double CBoostedCommittee::Predict(double * in_Sample)
{
  double final_prediction = 0;
  for (int i = 0; i < (int)m_vWeights.size(); i++)
  {
    final_prediction += m_vWeights[i] * m_vHypotheses[i].Predict(in_Sample);
  }
  return final_prediction;
}

bool CBoostedCommittee::LoadFromFile(FILE* in_File)
{
  int TotalHypothesis;

  if(!fscanf_s(in_File, "%d", &TotalHypothesis))
    return false;

  if (!fscanf_s(in_File, "%d", &m_split))
    return false;

  if (!fscanf_s(in_File, "%d", &m_label))
	  return false;

  m_vHypotheses.resize(TotalHypothesis);
  m_vWeights.resize(TotalHypothesis);

  for (int i = 0; i < TotalHypothesis; i++)
  {
    float WeightsBuff;
    if(!fscanf_s(in_File, "%f", &WeightsBuff))
      return false;
    m_vWeights[i] = WeightsBuff;
    if(!m_vHypotheses[i].LoadFromFile(in_File))
      return false;
  }
  
  return true;
}

bool CBoostedCommittee::LoadFromFile(const tstring& path)
{
	FILE *fid;
#ifdef UNICODE
	if(_wfopen_s(&fid,path.c_str(),L"r") != 0)
		return false;
#else
	if(fopen_s(&fid,path.c_str(),"r") != 0)
		return false;
#endif
	bool r = LoadFromFile(fid);
	fclose(fid);
	return r;
}

void CBoostedCommittee::WriteToFile(const tstring& path)
{
	tofstream out(path);
	WriteToFile(out);
	out.close();
}

void CBoostedCommittee::WriteToFile(tostream& outStream)
{
	outStream<<m_vHypotheses.size()<<endl;
	outStream<<m_split<<endl;
	outStream<<m_label<<endl;
	outStream<<setprecision(6);
	for (int i=0; i<(int)m_vHypotheses.size(); i++)
	{
		int N = m_vHypotheses[i].m_vDims.size();

		outStream<<setw(12)<<m_vWeights[i];
		outStream<<setw(5)<<N;
		for (int n=0; n<N; n++)
		{
			outStream<<setw(7)<<m_vHypotheses[i].m_vDims[n];
			outStream<<setw(9)<<(double)m_vHypotheses[i].m_vThresholds[n];
			outStream<<setw(5)<<(double)m_vHypotheses[i].m_vSignums[n];
		}
		outStream<<endl;
	}
}
