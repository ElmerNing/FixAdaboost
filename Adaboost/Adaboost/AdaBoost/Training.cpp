#include "StdAfx.h"
#include "Training.h"

#include <algorithm>

Training::Training(void)
{
}

Training::~Training(void)
{
}

void Training::Train( const TrainingSet& trainset, Model& outModel, int splitNum/*=2*/, int iteration/*=200*/, int negLabel/*=-1*/ )
{
	//获取样本的种类
	vector<int> labelTypes;
	for (int n=0; n<trainset.N(); n++)
	{
		bool isExist = false;
		int label = trainset.Labels()[n];
		for (int i=0; i<(int)labelTypes.size(); i++)
		{
			if (label == labelTypes[i])
			{
				isExist = true;
				break;
			}
		}
		if (isExist == false)
			labelTypes.push_back(label);
	}

	//对每一种类进行训练
	outModel.m_model.clear();
	for (int i=0; i<(int)labelTypes.size(); i++)
	{
		if (labelTypes[i] == negLabel)
			continue;

		//对当前类进行训练
		CBoostedCommittee boost;
		TrainOneVsAll(trainset, boost, splitNum, iteration, labelTypes[i]);
		outModel.m_model.push_back(boost);
	}
}

void Training::TrainOneVsAll( const TrainingSet& trainset,CBoostedCommittee& outBoost,int splitNum/*=2*/, int iteration/*=200*/, int posLabel/*=1*/ )
{
	//初始化weight
	double* weight = new double[trainset.N()];
	int posNum=0, negNum=0;
	for (int n=0; n<trainset.N(); n++)
	{
		if (trainset.Labels()[n] == posLabel)
			posNum++;
		else
			negNum++;
	}

	for (int n=0; n<trainset.N(); n++)
	{
		if (trainset.Labels()[n] == posLabel)
			weight[n] = 0.5/posNum;
		else
			weight[n] = 0.5/negNum;
	}

	outBoost.m_label = posLabel;
	outBoost.m_split = splitNum;
	//迭代
	for (int t=0; t<iteration; t++)
	{
		shared_ptr<TreeNode> tree = WeakLearn(trainset, weight, splitNum, posLabel);
		UpdateWeight(trainset, tree, weight, posLabel);
		AddTreeToBoost(tree, outBoost);
	}
	//alpha归一化
	double sum = 0;
	for (int i=0; i<(int)outBoost.m_vWeights.size(); i++)
	{
		sum += abs(outBoost.m_vWeights[i]);
	}
	for (int i=0; i<(int)outBoost.m_vWeights.size(); i++)
	{
		outBoost.m_vWeights[i] = outBoost.m_vWeights[i]/sum;
	}
	delete[] weight;
}

shared_ptr<TreeNode> Training::WeakLearn(const TrainingSet& trainset, const double weight[], int splitNum, int posLabel)
{
	int* datasetFlags = new int[trainset.N()];
	memset(datasetFlags,0,trainset.N()*sizeof(int));
	int flag = 0;

	Stump stump = OptimalOneNode(trainset, weight, posLabel, flag, datasetFlags);
	if (stump.Pm > 0.5)
		throw tstring(_T("不可分!"));

	shared_ptr<TreeNode> root(new TreeNode());
	root->InnerStump = stump;
	root->Flag = flag;
	root->Delta = 0.5-stump.Pm;

	vector<shared_ptr<TreeNode>> priorityQueue;
	priorityQueue.push_back(root);

	double Pm = stump.Pm;
	for (int splitIndex=0; splitIndex<splitNum; splitIndex++)
	{
		shared_ptr<TreeNode> treeNode;
		do 
		{
			treeNode = priorityQueue[0];
			priorityQueue.erase(priorityQueue.begin());
		} while (treeNode->InnerStump.Pm >0.5 && priorityQueue.size() != 0);

		if (treeNode->InnerStump.Pm > 0.5)
			break;

		if (treeNode->Parent.lock() != nullptr)
		{
			if (treeNode->Flag % 2 != 0)
				treeNode->Parent.lock()->Left = treeNode;
			else
				treeNode->Parent.lock()->Right = treeNode;
			Pm = Pm - treeNode->Delta;
		}

		int leftFlag = ++flag;
		int rightFlag = ++flag;
		CutDataSet(*treeNode, trainset, leftFlag, rightFlag, datasetFlags);

		shared_ptr<TreeNode> leftNode(new TreeNode());
		shared_ptr<TreeNode> rightNode(new TreeNode());
		if (treeNode->InnerStump.Pm > numeric_limits<double>::epsilon())
		{
			stump = OptimalOneNode(trainset, weight, posLabel, leftFlag, datasetFlags);
			if (stump.Pm < 0.5)
			{
				leftNode->InnerStump = stump;
				leftNode->Parent = treeNode;
				leftNode->Flag = leftFlag;
				leftNode->Delta = treeNode->InnerStump.Pl - stump.Pm;
			}
		}
		if (treeNode->InnerStump.Pr > numeric_limits<double>::epsilon())
		{
			stump = OptimalOneNode(trainset, weight, posLabel, rightFlag, datasetFlags);
			if (stump.Pm < 0.5)
			{
				rightNode->InnerStump = stump;
				rightNode->Parent = treeNode;
				rightNode->Flag = rightFlag;
				rightNode->Delta = treeNode->InnerStump.Pr - stump.Pm;
			}
		}
		priorityQueue.push_back(leftNode);
		priorityQueue.push_back(rightNode);
		sort(priorityQueue.begin(), priorityQueue.end(), TreeNodeCompare);
	}

	delete[] datasetFlags;

	return root;
}

struct{
	double* weight;
	TrainingSet* pTrainSet;
	int posLabel;
	int flag;
	int* dataSetFlags;
	int startDim;
	int endDim;
};

Stump Training::OptimalOneNode( const TrainingSet& trainset, const double weight[], int posLabel, int flag, int datasetFlags[] )
{
	double* sortedWeight = new double[trainset.N()];

	double* valueCollection = new double[trainset.N()];
	double* integralPos = new double[trainset.N()];
	double* integralNeg = new double[trainset.N()];
	double* errorPos = new double[trainset.N()];
	double* errorNeg = new double[trainset.N()];

	vector<Stump> stumpArray;
	Stump bestStump;
	for (int dim = 0; dim < trainset.Dim(); dim++)
	{
		for (int n=0; n<trainset.N(); n++)
		{
			sortedWeight[n] = weight[ trainset.Samples()[dim][n].N ];
		}
		
		Stump stump = OptimalOneDim(trainset.Samples()[dim], sortedWeight, trainset.Labels(), posLabel, flag, datasetFlags, dim,
			valueCollection, integralPos, integralNeg, errorPos, errorNeg);
		if (stump.Pm < 0.5)
		{
			if (stump.Pm < bestStump.Pm)
			{
				bestStump = stump;
			}
		}
	}

	delete[] sortedWeight;

	delete[] errorPos;
	delete[] errorNeg;
	delete[] integralPos;
	delete[] integralNeg;
	delete[] valueCollection;

	return bestStump;
	
}

Stump Training::OptimalOneDim( const SetNode sortedDim[], 
	const double sortedWeight[], 
	const vector<int>& labels, 
	int posLabel,int flag, 
	int datasetFlags[], 
	int dim,
	double valueCollection[],
	double integralPos[],
	double integralNeg[],
	double errorPos[],
	double errorNeg[])
{
	//integral of (yn*wn)  for speed improvement 
	int N = labels.size();
	int integralCount = 0;
	int valueCollectionCount = 0;

	memset(valueCollection, 0, N*sizeof(double));
	memset(integralPos, 0, N*sizeof(double));
	memset(integralNeg, 0, N*sizeof(double));

	for (int n = 0; n < N; )
	{
		if (datasetFlags[sortedDim[n].N] != flag)
		{
			n++;
			continue;
		}

		int offset = 0;
		while (n + offset < N && sortedDim[n].Value == sortedDim[n + offset].Value)
		{
			if (offset == 0)
			{
				integralCount++;
			}
			int nx = n + offset;
			if (datasetFlags[sortedDim[nx].N] == flag)
			{
				if ( labels[ sortedDim[n + offset].N ] == posLabel)
					integralPos[integralCount - 1] += sortedWeight[nx];
				else
					integralNeg[integralCount - 1] += sortedWeight[nx];
			}
			offset++;
		}
		valueCollectionCount++;
		valueCollection[valueCollectionCount - 1] = sortedDim[n].Value;
		n += offset;
	}

	for (int v = 1; v < integralCount; v++)
	{
		integralPos[v] += integralPos[v - 1];
		integralNeg[v] += integralNeg[v - 1];
	}

	if (integralCount <= 1)
	{
		return Stump();
	}

	// calculate error rates of (Count-1) split
	for (int v = 0; v < integralCount - 1; v++)
	{
		errorPos[v] = integralPos[v] + integralNeg[integralCount - 1] - integralNeg[v];
		errorNeg[v] = integralNeg[v] + integralPos[integralCount - 1] - integralPos[v];
	}

	//find the min error rate as Pm and return the best stump of this dim
	int minPosIndex = 0, minNegIndex = 0;
	double errorPosMin = 1, errorNegMin = 1;
	for (int v = 1; v < integralCount - 1; v++)
	{
		if (errorPosMin >= errorPos[v])
		{
			minPosIndex = v;
			errorPosMin = errorPos[v];
		}
		if (errorNegMin >= errorNeg[v])
		{
			minNegIndex = v;
			errorNegMin = errorNeg[v];
		}
	}

	double sign = 1, threshold = 0, Pm = 0.49, Pl = 0.49, Pr = 0.49;
	double PleftPos=0, PleftNeg=0, PrightPos=0, PrightNeg=0, LeftWeight = 0, RightWeight = 0;
	if (errorNegMin > errorPosMin)
	{
		sign = 1;
		threshold = 0.5 * (valueCollection[minPosIndex] + valueCollection[minPosIndex + 1]);
		Pm = errorPosMin;
		Pl = integralPos[minPosIndex];
		Pr = integralNeg[integralCount - 1] - integralNeg[minPosIndex];
		PleftPos = integralPos[minPosIndex];
		PleftNeg = integralNeg[minPosIndex];
		PrightPos = integralPos[integralCount-1] - PleftPos;
		PrightNeg = integralNeg[integralCount - 1] - PleftNeg;
	}
	else
	{
		threshold = 0.5 * (valueCollection[minNegIndex] + valueCollection[minNegIndex + 1]);
		sign = -1;
		Pm = errorNegMin;
		Pl = integralNeg[minNegIndex];
		Pr = integralPos[integralCount - 1] - integralPos[minNegIndex];
		PleftPos = integralPos[minNegIndex];
		PleftNeg = integralNeg[minNegIndex];
		PrightPos = integralPos[integralCount-1] - PleftPos;
		PrightNeg = integralNeg[integralCount - 1] - PleftNeg;
	}

	LeftWeight = (PleftPos-PleftNeg)/(PleftNeg+PleftPos);
	RightWeight = (PrightPos-PrightNeg)/(PrightNeg+PrightPos);

	return Stump(dim, threshold, Pm, Pl, Pr, PleftPos, PleftNeg, PrightPos, PrightNeg, LeftWeight, RightWeight);
}

void Training::CutDataSet( TreeNode treeNode, const TrainingSet& trainset, int leftFlag, int rightFlag, int datasetFlag[] )
{
	int N = trainset.N();
	const SetNode* sortedDim = trainset.Samples()[treeNode.InnerStump.Dim];
	double Thr = treeNode.InnerStump.Thr;
	for (int n = 0; n < N;  n++)
	{
		//SortedNode node = sortedDim[n];
		if (datasetFlag[sortedDim[n].N] == treeNode.Flag)
		{
			if (sortedDim[n].Value > Thr)
				datasetFlag[sortedDim[n].N] = rightFlag;
			else
				datasetFlag[sortedDim[n].N] = leftFlag;
		}
	}
}

bool Training::TreeNodeCompare( const shared_ptr<TreeNode>& left, const shared_ptr<TreeNode>& right )
{
	return (left->Delta > right->Delta);
}

double Training::Predict( const TrainingSet& trainset, int N, const shared_ptr<TreeNode>& tree )
{
	const SetNode* nodes = trainset.Samples()[tree->InnerStump.Dim];
	for (int n=0; n<trainset.N(); n++)
	{
		if (nodes[n].N == N)
		{
			if (nodes[n].Value > tree->InnerStump.Thr)
			{
				if (tree->Right == nullptr)
					return tree->InnerStump.RightWeight;
				else
					return Predict(trainset, N, tree->Right);
			}
			else
			{
				if (tree->Left == nullptr)
					return tree->InnerStump.LeftWeight;
				else
					return Predict(trainset, N, tree->Left);
			}
		}
	}
	return -1;
}

void Training::UpdateWeight(const TrainingSet& trainset, const shared_ptr<TreeNode>& tree, double weight[], int posLabel)
{
	double sum = 0;
	for (int n=0; n<trainset.N(); n++)
	{
		double alpha = Predict(trainset, n, tree);
		int label = trainset.Labels()[n] == posLabel ? 1 : -1;
		weight[n] = weight[n] * exp(-1*label*alpha);
		sum += weight[n];
	}
	for (int n=0; n<trainset.N(); n++)
	{
		weight[n] = weight[n] / sum;
	}
}

void Training::AddTreeToBoost( const shared_ptr<TreeNode>& tree, CBoostedCommittee &outBoost )
{
	int start=outBoost.m_vWeights.size();
	TreeToHypothesisArray(tree, CSPHypothesis(), outBoost.m_vHypotheses, outBoost.m_vWeights);

	int end = outBoost.m_vWeights.size();

	if (end <= start)
		return;

	//排序,node深度较浅的排前面.
	for (int i=start; i<end; i++)
	{
		for (int j=start+1; j<end; j++)
		{
			CSPHypothesis tempH;
			double tempW;
			if ( outBoost.m_vHypotheses[j].m_vDims.size() < outBoost.m_vHypotheses[i].m_vDims.size() )
			{
				tempH = outBoost.m_vHypotheses[i];
				tempW = outBoost.m_vWeights[i];

				outBoost.m_vHypotheses[i] = outBoost.m_vHypotheses[j];
				outBoost.m_vWeights[i] = outBoost.m_vWeights[j];

				outBoost.m_vHypotheses[j] = tempH;
				outBoost.m_vWeights[j] = tempW;
			}
		}
	}
}

void Training::TreeToHypothesisArray( const shared_ptr<TreeNode>& tree, CSPHypothesis currentHypothesis, vector<CSPHypothesis>& outHypotheses, vector<double>& outWeight )
{
	//为了和matlab同步, 维度索引从1开始, 所以维度加1
	currentHypothesis.m_vDims.push_back(tree->InnerStump.Dim + 1);
	currentHypothesis.m_vThresholds.push_back(tree->InnerStump.Thr);
	currentHypothesis.m_vSignums.push_back(0);
	
	//left
	currentHypothesis.m_vSignums[currentHypothesis.m_vSignums.size()-1] = -1;
	if (tree->Left != nullptr)
	{
		TreeToHypothesisArray(tree->Left, currentHypothesis, outHypotheses, outWeight);
	}
	else
	{
		outHypotheses.push_back(currentHypothesis);
		outWeight.push_back(tree->InnerStump.LeftWeight);
	}

	//right
	currentHypothesis.m_vSignums[currentHypothesis.m_vSignums.size()-1] = 1;
	
	if (tree->Right != nullptr)
	{
		TreeToHypothesisArray(tree->Right, currentHypothesis, outHypotheses, outWeight);
	}
	else
	{
		outHypotheses.push_back(currentHypothesis);
		outWeight.push_back(tree->InnerStump.RightWeight);
	}
}
