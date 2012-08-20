#pragma once
#include "TrainingSet.h"
#include "Model.h"
#include "BoostedCommittee.h"

//树的一个分叉
class Stump;

//树的一个节点
class TreeNode;

class Training
{
public:
	Training(void);
	~Training(void);

	//训练样本集,生成一个model
	static void Train(const TrainingSet& trainset, Model& outModel, int splitNum=2, int iteration=200, int negLabel=-1);

	//训练样本集其中的一个标签posLabel, 生成一个CBoostedCommittee(相当于一个2类分类器(posLabel,非posLabel两类))
	static void Training::TrainOneVsAll( const TrainingSet& trainset,CBoostedCommittee& outBoost,int splitNum=2, int iteration=200, int posLabel=1);

private:
	static shared_ptr<TreeNode> WeakLearn(const TrainingSet& trainset, const double weight[], int splitNum, int posLabel);
	static Stump OptimalOneNode( const TrainingSet& trainset, const double weight[], int posLabel, int flag, int datasetFlags[] );
	static Stump OptimalOneDim( const SetNode sortedDim[], const double sortedWeight[], const vector<int>& labels, int posLabel,int flag, int datasetFlags[], int dim, double valueCollection[], double integralPos[], double integralNeg[], double errorPos[], double errorNeg[]);
	static void CutDataSet(TreeNode treeNode, const TrainingSet& trainset, int leftFlag, int rightFlag, int datasetFlag[]);
	static bool TreeNodeCompare(const shared_ptr<TreeNode>& left, const shared_ptr<TreeNode>& right);
	static void UpdateWeight(const TrainingSet& trainset, const shared_ptr<TreeNode>& tree, double weight[], int posLabel);
	static double Predict(const TrainingSet& trainset, int N, const shared_ptr<TreeNode>& tree);
	static void AddTreeToBoost(const shared_ptr<TreeNode>& tree, CBoostedCommittee &outBoost);
	static void TreeToHypothesisArray(const shared_ptr<TreeNode>& tree, CSPHypothesis currentHypothesis, vector<CSPHypothesis>& outHypotheses, vector<double>& outWeight);
};

class Stump
{
public:
	Stump(){
		Dim = 0;
		Thr = 0;
		Pm = 1;
		Pl = 1;
		Pr = 1;
		LeftWeight = 0;
		RightWeight = 0;
		PleftPos = 0;
		PleftNeg = 0;
		PrightPos = 0;
		PrightNeg = 0;
	};

	Stump(int dim, double threshold, double pm, double pl, double pr, double leftPos, double leftNeg, double rightPos, double rightNeg, double LeftWeight, double RightWeight)
	{
		Dim = dim;
		Thr = threshold;
		Pm = pm;
		Pl = pl;
		Pr = pr;
		PleftPos = leftPos;
		PleftNeg = leftNeg;
		PrightPos = rightPos;
		PrightNeg = rightNeg;
		this->LeftWeight = LeftWeight;
		this->RightWeight = RightWeight;
	}

	~Stump(){
	};

	bool operator<(const Stump &t1)
	{
		return this->Pm < t1.Pm;
	};

public:
	int Dim;
	double Thr;
	double Pm;
	double Pl;
	double Pr;
	double LeftWeight;
	double RightWeight;
	double PleftPos;
	double PleftNeg;
	double PrightPos;
	double PrightNeg;
};

class TreeNode{
public:
	TreeNode()
	{
		Flag = 0;
		Delta = 0;
	}
	~TreeNode()
	{}

public:
	Stump InnerStump;
	shared_ptr<TreeNode> Left;
	shared_ptr<TreeNode> Right;
	weak_ptr<TreeNode> Parent;
	int Flag;
	double Delta;

};
