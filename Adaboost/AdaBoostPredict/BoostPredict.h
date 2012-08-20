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

//用一个二类分类器strgclsf识别feature,返回1或者-1
short BoostPredictOne(short* feature, StrongClassifier strgclsf );
//用一个多类分类器model去识别feature,识别失败返回-1,识别成功返回feature的类别
short BoostPredict(short* feature, AdaBoostModel model);

#ifdef __cplusplus
};
#endif	//__cplusplus
#endif	//BOOST_PREDICT_H_