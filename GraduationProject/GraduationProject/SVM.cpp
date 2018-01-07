#include "stdafx.h"
#include "SVM.h"
#include <opencv2\ml\ml.hpp>

using namespace ml;

SVMTest::SVMTest()
{
	svm = SVM::create();
}

bool SVMTest::Initialize()
{
	svm->setC(c);
	svm->setCoef0(coef);
	svm->setDegree(degree);
	svm->setGamma(gamma);
	svm->setKernel(kernel);
	svm->setNu(nu);
	svm->setP(p);
	svm->setType(svmType);

	//svm->setTermCriteria(TermCriteria(TermCriteria::EPS, 1000, FLT_EPSILON)); // based on accuracy
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6)); // based on the maximum number of iterations
	return true;
}

SVMTest::~SVMTest()
{
}

void SVMTest::Train(Mat &featureVectorsOfSample, Mat &classOfSample)
{
	// train
	ParamGrid cGrid = ParamGrid(1, 1, 0.0);
	ParamGrid nuGrid = ParamGrid(1, 1, 0.0);
	ParamGrid coeffGrid = ParamGrid(1, 1, 0.0);
	ParamGrid degreeGrid = ParamGrid(1, 1, 0.0);
	ParamGrid pGrid = ParamGrid(1, 1, 0.0);

	double time1, time2;
	time1 = getTickCount();

	Mat featureVectorsOfSample_;
	Mat classOfSample_;
	featureVectorsOfSample.convertTo(featureVectorsOfSample_, CV_32FC1);
	classOfSample.convertTo(classOfSample_, CV_32SC1);

	// for save data to load
    #ifdef SAVE_PARAM
	ofstream outvecotr("train-kernel-vector.txt");
	outvecotr << featureVectorsOfSample_ << endl;
	outvecotr.close();

	ofstream outlabel("train-kernel-label.txt");
	outlabel << classOfSample_ << endl;
	outlabel.close();
    #endif

	Ptr<TrainData>sampleData = TrainData::create(featureVectorsOfSample_, ROW_SAMPLE, classOfSample_);
	
	switch (svm->getKernelType())
	{
	case 0: //null
		svm->trainAuto(sampleData); 
		break;
	case 1: // gamma, degree, coef
		svm->trainAuto(sampleData, 10, cGrid,
			svm->getDefaultGrid(SVM::GAMMA),
			pGrid, nuGrid, 
			svm->getDefaultGrid(SVM::COEF), 
			svm->getDefaultGrid(SVM::DEGREE));
		break;
	case 2: // gamma
		svm->trainAuto(sampleData, 10, cGrid,
			svm->getDefaultGrid(SVM::GAMMA),
			pGrid, nuGrid, coeffGrid, degreeGrid);
		break;
	case 3: // gamma, coef0
		svm->trainAuto(sampleData, 10, cGrid,
			svm->getDefaultGrid(SVM::GAMMA),
			pGrid, nuGrid,
			svm->getDefaultGrid(SVM::COEF),
			degreeGrid);
	default: // gamma, coef0
		svm->trainAuto(sampleData, 10, cGrid,
			svm->getDefaultGrid(SVM::GAMMA),
			pGrid, nuGrid, 
			svm->getDefaultGrid(SVM::COEF),
		    degreeGrid);
		break;
	}
	time2 = getTickCount();
	printf("训练时间:%f\n", (time2 - time1)*1000. / getTickFrequency());
	printf("training done!\n");

	// save model
	svm->save(svmModelFilePath);
}

void SVMTest::Predict(Mat &featureVectorsOfSample, Mat &classOfSample)
{

    #ifdef SAVE_PARAM
	ofstream outvector("predict-kernel-vecotr.txt");
	outvector << featureVectorsOfSample << endl;
	outvector.close();

	ofstream outlabel("predict-kernel-label.txt");
	outlabel << classOfSample << endl;
	outlabel.close();
    #endif

	// predict
	int numberOfRight_0 = 0;
	int numberOfError_0 = 0;
	int numberOfRight_1 = 0;
	int numberOfError_1 = 0;

	std::ofstream fileOfPredictResult(predictResultFilePath, ios::out); //最后识别的结果
	double sum_Predict = 0, sum_ExtractFeature = 0;
	char line2[256] = { 0 };
	for (int i = 0; i < featureVectorsOfSample.rows; ++i)
	{
		// extract feature
		double time1_ExtractFeature = getTickCount();
		Mat featureVectorOfTestImage = featureVectorsOfSample.row(i);
		Mat featureVectorOfTestImage_;
		featureVectorOfTestImage.convertTo(featureVectorOfTestImage_, CV_32FC1);
		double time2_ExtractFeature = getTickCount();
		sum_ExtractFeature += (time2_ExtractFeature - time1_ExtractFeature) * 1000 / getTickFrequency();

		//对测试图片进行分类并写入文件
		double time1_Predict = getTickCount();
		int predictResult = svm->predict(featureVectorOfTestImage_);
		double time2_Predict = getTickCount();
		sum_Predict += (time2_Predict - time1_Predict) * 1000 / getTickFrequency();

		sprintf(line2, "%d %d\n", i, predictResult);
		fileOfPredictResult << line2;
	
		// 0
		if ((classOfSample.at<float>(i,0) == 0) && (predictResult == 0))
		{
			++numberOfRight_0;
		}
		if ((classOfSample.at<float>(i, 0) == 0) && (predictResult != 0))
		{
			++numberOfError_0;
		}

		// 1
		if ((classOfSample.at<float>(i, 0) == 1) && (predictResult == 1))
		{
			++numberOfRight_1;
		}
		if ((classOfSample.at<float>(i, 0) == 1) && (predictResult != 1))
		{
			++numberOfError_1;
		}
	}

	sprintf(line2, "extract feature time：%f\n", sum_ExtractFeature / featureVectorsOfSample.rows);
	fileOfPredictResult << line2;

	sprintf(line2, "predict time：%f\n", sum_Predict / featureVectorsOfSample.rows);
	fileOfPredictResult << line2;

	// 0
	double accuracy_0 = (100.0*(numberOfRight_0)) / (numberOfError_0 + numberOfRight_0);
	sprintf(line2, "0：%f\n", accuracy_0);
	fileOfPredictResult << line2;

	// 1
	double accuracy_1 = (100.0*numberOfRight_1) / (numberOfError_1 + numberOfRight_1);
	sprintf(line2, "1：%f\n", accuracy_1);
	fileOfPredictResult << line2;

	// accuracy
	double accuracy_All = (100.0*(numberOfRight_1 + numberOfRight_0)) / (numberOfError_0 + numberOfRight_0 + numberOfError_1 + numberOfRight_1);

	sprintf(line2, "accuracy:%f\n", accuracy_All);
	fileOfPredictResult << line2;

	fileOfPredictResult.close();
}