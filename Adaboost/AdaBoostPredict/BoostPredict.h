#ifndef BOOST_PREDICT_H_
#define BOOST_PREDICT_H_

#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
	unsigned short dim;
	short thr;
	unsigned short leftW_and_rightW;
} WeakClassifier;

typedef struct{
	const WeakClassifier* weakClassifier;
	const short* weight;
	int weakSize;
	int splitNum;
	int label;
} StrongClassifier;

typedef struct{
	const StrongClassifier* strongClassifier;
	int size;
} AdaBoostModel;

//��һ�����������strgclsfʶ��feature,����1����-1
short BoostPredictOne(short* feature, StrongClassifier strgclsf );
//��һ�����������modelȥʶ��feature,ʶ��ʧ�ܷ���-1,ʶ��ɹ�����feature�����
short BoostPredict(short* feature, AdaBoostModel model);

#ifdef __cplusplus
};
#endif	//__cplusplus
#endif	//BOOST_PREDICT_H_