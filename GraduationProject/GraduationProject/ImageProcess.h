#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;


class ImageProcess
{

#define PI 3.14
#define ELEMENT(x,y)(getStructuringElement(MORPH_RECT, Size((x), (y))))
private:
	Rect poolPRoi;  // pool center roi
	Rect poolSRoi;  // pool steam roi
	Rect poolFRoi;  // pool full roi
	Rect steamRoi;  // steam roi


private:
	void calHeight(const Mat &srcImage, double &height);
	void calArea(const Mat &image, double &area);
	void getTwoLineWidth(const Mat &srcImage, double width);
	void calArea2(const Mat &srcImage, double &area);
	void denosing(const Mat &srcImage, Mat &dstImage, Size size);
	void calDirection(const Mat &srcImage, double &direction);
	void calCenter(const Mat &src, Point2f &center);

public:
	ImageProcess();
	ImageProcess(Mat &mat);
	ImageProcess(Mat *mat);
	virtual~ImageProcess();
	Mat  grayImage;
	Mat  srcImage;
	Point2f preCenter; // �����һ��û�м���õ����ģ������ϴε����
	double  prePArea;

	// �۳ز��ּ��㷵�ر���
	Point poolCenter;
	double poolPArea;
	double poolMaxDist;
	double poolMinDist;
	double poolSArea;

	// �������ּ��㷵�ر���
	double steamHeight;
	double steamArea;
	double steamDerc;
	void* steamColor;

public:
	//* Set param: ROI of pool and steam
	void setSteamROI(Rect &roi);
	void setPoolPROI(Rect &roi);
	void setPoolSROI(Rect &roi);
	void setPoolFROI(Rect &roi);

	//*  Get steam process Image
	void getSteam(Mat &dstImage);   // �õ�����ͼ��+�۳�ͼ��
	void getSteamS(Mat &dstImage);  // �õ�����ͼ��
	void getSteamP(Mat &dstImage);  // �õ��۳�ͼ��


	//*  Get pool process Image
	void getPool(Mat &dstImage);    // �õ��۳�ͼ��+����ͼ��
	void getPoolS(Mat &dstImage);   // �õ��۳�����ͼ��
	void getPoolP(Mat &dstImage);   // �õ��۳�ͼ��

	bool calPoolAll();
	bool calSteamAll();  
};

