#pragma once

#define HAVE_OPENCV_ML
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

class LBP
{
public:

	// ���������256άLBP��������
	void ComputeLBPFeatureVector_256(const Mat &srcImage, Size cellSize, Mat &featureVector);
	void ComputeLBPImage_256(const Mat &srcImage, Mat &LBPImage);// ����256άLBP����ͼ

	// ����ҶȲ���+�ȼ�ģʽLBP��������(58��ģʽ)
	void ComputeLBPFeatureVector_Uniform(const Mat &srcImage, Size cellSize, Mat &featureVector);
	void ComputeLBPImage_Uniform(const Mat &srcImage, Mat &LBPImage);// ����ȼ�ģʽLBP����ͼ

	// ����ҶȲ���+��ת����+�ȼ�ģʽLBP��������(9��ģʽ)
	void ComputeLBPFeatureVector_Rotation_Uniform(const Mat &srcImage, Size cellSize, Mat &featureVector);
	void ComputeLBPImage_Rotation_Uniform(const Mat &srcImage, Mat &LBPImage); 
	// ����ҶȲ���+��ת����+�ȼ�ģʽLBP����ͼ,ʹ�ò��ұ�

	// ����Բ��LBP
	void ComputeLBPImage_Circle(const Mat & srcImage, Mat &LBPImage);
	void ComputeLBPFeatureVector_Circle(const Mat &srcImage , Size cellSize,  Mat &featureVector);

	// Test
	void Test();// ���ԻҶȲ���+��ת����+�ȼ�ģʽLBP
	void TestGetMinBinaryLUT();

private:
	void BuildUniformPatternTable(int *table); // ����ȼ�ģʽ���ұ�
	int GetHopCount(int i);// ��ȡi��0,1���������

	void ComputeLBPImage_Rotation_Uniform_2(const Mat &srcImage, Mat &LBPImage);
	// ����ҶȲ���+��ת����+�ȼ�ģʽLBP����ͼ,��ʹ�ò��ұ�
	int ComputeValue9(int value58);// ����9�ֵȼ�ģʽ
	int GetMinBinary(int binary);// ͨ��LUT������С������
	uchar GetMinBinary(uchar *binary); // ����õ���С������
};




