#pragma once

#include <opencv2/ml.hpp>
#include <fstream>
#include "LBP.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv::ml;

#define LOG_WARN_SVM_TEST(...)                  //LOG4CPLUS_MACRO_FMT_BODY ("SVMTest", WARN_LOG_LEVEL, __VA_ARGS__)
//#define CONFIG_FILE                                              "./Resource/Configuration.xml"
#define CELL_SIZE   16

class SVMTest
{
public:

	SVMTest();
	bool Initialize();
	virtual ~SVMTest();

	void Train(Mat &featureVectorsOfSample, Mat &classOfSample);
	void Predict(Mat &featureVectorsOfSample, Mat &classOfSample);

public:
	// ²ÎÊý
	CString     testDataFileList;
	string      svmModelFilePath;
	string      predictResultFilePath;
	SVM::Types       svmType;          // See SVM::Types. Default value is SVM::C_SVC.
	SVM::KernelTypes kernel;      
	double c;                          // For SVM::C_SVC, SVM::EPS_SVR or SVM::NU_SVR. Default value is 0.
	double coef;                       // For SVM::POLY or SVM::SIGMOID. Default value is 0.
	double degree;                     // For SVM::POLY. Default value is 0.
	double gamma;                      // For SVM::POLY, SVM::RBF, SVM::SIGMOID or SVM::CHI2. Default value is 1.
	double nu;                         // For SVM::NU_SVC, SVM::ONE_CLASS or SVM::NU_SVR. Default value is 0.
	double p;                          // For SVM::EPS_SVR. Default value is 0.
    
	Ptr<SVM> svm;
	// feature extracting(HOG,LBP,Haar,etc)
	LBP lbp;
};


