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
	Point2f preCenter; // 如果下一次没有计算得到质心，就用上次的替代
	double  prePArea;

	// 熔池部分计算返回变量
	Point poolCenter;
	double poolPArea;
	double poolMaxDist;
	double poolMinDist;
	double poolSArea;

	// 蒸汽部分计算返回变量
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
	void getSteam(Mat &dstImage);   // 得到蒸汽图像+熔池图像
	void getSteamS(Mat &dstImage);  // 得到蒸汽图像
	void getSteamP(Mat &dstImage);  // 得到熔池图像


	//*  Get pool process Image
	void getPool(Mat &dstImage);    // 得到熔池图像+蒸汽图像
	void getPoolS(Mat &dstImage);   // 得到熔池蒸汽图像
	void getPoolP(Mat &dstImage);   // 得到熔池图像

	bool calPoolAll();
	bool calSteamAll();  
};

