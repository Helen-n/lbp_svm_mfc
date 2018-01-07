#include "stdafx.h"
#include "ImageProcess.h"
#include  <assert.h>

ImageProcess::ImageProcess():
	preCenter(0,0),
	prePArea(10)
{}

ImageProcess::ImageProcess(Mat *mat)
	:srcImage(*mat)
{
	if (!srcImage.empty())
	{
		if (srcImage.channels() > 1)
			cvtColor(srcImage, grayImage, CV_RGB2GRAY);
		else
			grayImage = srcImage;
	}
	else
		assert(srcImage.empty());
}

ImageProcess::ImageProcess(Mat &mat)
	:srcImage(mat)
{
	if (!srcImage.empty())
	{
		if (srcImage.channels() > 1)
			cvtColor(srcImage, grayImage, CV_RGB2GRAY);
		else
			grayImage = srcImage;
	}
	else
		assert(srcImage.empty());
}

ImageProcess::~ImageProcess()
{}

// 开运算
void ImageProcess::denosing(const Mat &srcImage, Mat &dstImage, Size size)
{
	// 进行去噪处理
	Mat element = getStructuringElement(MORPH_RECT, size);
	// 进行开运算
	morphologyEx(srcImage, dstImage, MORPH_OPEN, element);
	// 进行腐蚀
	morphologyEx(dstImage, dstImage, MORPH_ERODE, element);
	// 进行膨胀
	morphologyEx(dstImage, dstImage, MORPH_DILATE, element);
}

// 计算图像面积方法一
void ImageProcess::calArea(const Mat &image, double &area)
{
	double conArea = 0;
	// 计算图像面积
	Mat cannyImage;
	// 对图像进行边缘检测
	Canny(image, cannyImage, 160, 255, 3);
	// 在得到的二值图像中寻找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 绘制轮廓
	for (int i = 0; i < (int)contours.size(); i++)
	{
		drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
	}

	// 计算轮廓的面积
	for (int i = 0; i < (int)contours.size(); i++)
	{
		conArea += contourArea(contours[i], true);
	}
	area = conArea;
}

// 计算图像面积方法二
void ImageProcess::calArea2(const Mat &srcImage, double &area)
{
	area = 0;
	int r, c;
	for (r = 0; r < srcImage.rows; r++)
	{
		for (c = 0; c < srcImage.cols; c++)
		{
			if (srcImage.at<char>(r, c) != 0)
			{
				area++;
			}
		}
	}
}

// 计算蒸汽方向
void ImageProcess::calDirection(const Mat &srcImage, double &direction)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Point> maxContour;
	vector<Vec4i> hierarchy;
	//Canny(srcImage, canny_output, 50, 255);
	Laplacian(srcImage, canny_output, srcImage.depth());
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (int i = 0; i < (int)contours.size(); i++)
	{
		drawContours(canny_output, contours, i, Scalar(255), 1, 8);
	}

	// 找到最大面积
	float maxArea = 0;
	int   maxIndex = -1;
	for (int i = 0; i < contours.size(); i++)
	{
		if (maxArea < contourArea(contours[i], true))
		{
			maxArea  = contourArea(contours[i], true);
			maxIndex = i;
		}
	}
	Point2d p2(0, 0); //距离最远的点
	Point2f	mc(0, 0);
	//计算轮廓矩   
	if (maxIndex != -1)
	{
		
		Moments mu;
		mu = moments(contours[maxIndex], false);
		//计算轮廓的质心   
		mc = Point2d(mu.m10 / mu.m00, mu.m01 / mu.m00);

		// 找到欧氏距离最远的点，并计算方向
		double maxDistance = 0.0, tmpDistance;
		
		maxContour = contours[maxIndex];
		for (int k = 0; k < maxContour.size(); ++k)
		{
			if (maxContour[k].y < mc.y)
			{
				Point p = maxContour[k];
				tmpDistance = (mc.x - p.x)*(mc.x - p.x) + (mc.y - p.y)*(mc.y - p.y);
				if (maxDistance < tmpDistance)
				{
					maxDistance = tmpDistance;
					p2.x = p.x; p2.y = p.y;
				}
			}
		}
		// 计算角度
		double tanXY = (mc.y - p2.y) / (p2.x - mc.x);
		direction = atan(tanXY);
		line(canny_output,mc,p2,Scalar(255));
		//imshow("【Derection,】",canny_output);
	}
}

// 计算熔池高度
void ImageProcess::calHeight(const Mat &srcImage, double &height)
{
	Mat cannyImage;
	double g_dConLength = 0;
	Canny(srcImage, cannyImage, 128, 255, 3);
	////imshow("【cannyImage】", cannyImage);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//绘制轮廓
	for (int i = 0; i < (int)contours.size(); i++)
	{
		drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
	}
	//计算轮廓的长度
	for (int i = 0; i < (int)contours.size(); i++)
	{
		 g_dConLength += arcLength(contours[i], true);
	}
	height = g_dConLength;
}

// 计算中心
void ImageProcess::calCenter(const Mat &src, Point2f &center)
{
	Mat cannyImage;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Laplacian(src, cannyImage, src.depth());
	findContours(cannyImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// 绘制每个的轮廓      
	for (int i = 0; i < (int)contours.size(); i++)
	{
		drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
	}

	// 找到最大面积
	float maxArea = 0;
	int   maxIndex = -1;
	for (int i = 0; i < contours.size(); i++)
	{
		if (maxArea < contourArea(contours[i], true))
		{
			maxArea = contourArea(contours[i], true);
			maxIndex = i;
		}
	}

	//计算轮廓的质心     
	if (maxIndex != -1)
	{
		Moments mu = moments(contours[maxIndex], false);
		center = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	}
}

// 处理得到熔池所有的数据
bool ImageProcess::calPoolAll()
{
	bool retFlag = false;
	Mat prosImage;
    getPoolP(prosImage);
	//imshow("【Pool Image】", prosImage);
	Mat element = ELEMENT(3,3);
	erode(prosImage, prosImage, element, Point(-1, -1), 3);
	//imshow("【Erode Image】", prosImage);
	morphologyEx(prosImage, prosImage, MORPH_OPEN, element, Point(-1, -1), 3);
	//imshow("【Open Image】", prosImage);
	threshold(prosImage, prosImage, 40, 255, 0);
	////imshow("【Threshold Image】", prosImage);
	// 寻找质心位置
	Point2f mc(-1, -1);
	calCenter(prosImage, mc);
	if (mc.x != -1)
	{
		preCenter = mc;
	}
	else
	{
		if (preCenter.x == 0)
		{
			preCenter.x = prosImage.cols / 2;
			preCenter.y = prosImage.rows / 2;
		}
		mc = preCenter;
	}
	circle(prosImage, mc, 2,Scalar(0));
	//imshow("【Threshold Image】", prosImage);
	// 最近距离与最远距离的差值和
	Mat prosImage2, laplaImage;
	getPoolP(prosImage2);
	morphologyEx(prosImage2, prosImage2, MORPH_GRADIENT, element, Point(-1, -1), 3);
	//imshow("【Grandient Image】", prosImage2);
	threshold(prosImage2, prosImage2, 80, 255, 0);
	//imshow("【Threshold2 Image】", prosImage2);
	boxFilter(prosImage2, prosImage2, -1, Size(8, 8));
	//imshow("【BoxFil Image】", prosImage2);
	morphologyEx(prosImage2, prosImage2, MORPH_CLOSE, element, Point(-1, -1), 3);
	morphologyEx(prosImage2, prosImage2, MORPH_ERODE, element, Point(-1, -1), 3);
	//imshow("【Open_Close Image】", prosImage2);
	threshold(prosImage2, prosImage2, 40, 255, 0);
	Laplacian(prosImage2, laplaImage, prosImage2.depth());
	//imshow("【Laplacian Image】", prosImage2);
	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;
	vector<Point> maxLContour;
	Point2d p2(0, 0), p3(0, 0); //距离最远的点
	float maxLength = 0;
	int   index = -1;

	Laplacian(prosImage2, laplaImage, prosImage2.depth());
	findContours(laplaImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < contours.size(); i++)
	{
		if (maxLength < arcLength(contours[i], true))
		{
			maxLength = arcLength(contours[i], true);
			index = i;
		}
	}

	float tmpDistance;
	float maxDistance = 0;
	float minDistance = 100000;
	if (index != -1)
	{
		maxLContour = contours[index];
		for (int k = 0; k < maxLContour.size(); ++k)
		{
			if (maxLContour[k].y < mc.y)
				continue;

			Point p = maxLContour[k];
			tmpDistance = (mc.x - p.x)*(mc.x - p.x) + (mc.y - p.y)*(mc.y - p.y);
			if (maxDistance < tmpDistance)
			{
				maxDistance = tmpDistance;
				p2.x = p.x; p2.y = p.y;
			}
			if (minDistance > tmpDistance)
			{
				minDistance = tmpDistance;
				p3.x = p.x; p3.y = p.y;
			}
		}
	}
	line(prosImage2,mc,p2,Scalar(255));
	line(prosImage2, mc, p3, Scalar(255));
	//imshow("【Line Image】",prosImage2);
	// 计算得到pool所有的参数
	poolCenter = mc;
    calArea2(prosImage,poolPArea);
	if (poolPArea)
	{
		prePArea = poolPArea;
	}
	else
	{
		poolPArea = prePArea;
	}

	poolMaxDist = sqrt(maxDistance);
	poolMinDist = sqrt(minDistance);

	Mat poolSteam;
	getPoolS(poolSteam);
	threshold(poolSteam, poolSteam,100,255,THRESH_BINARY);
	//imshow("【PSteam Image】",poolSteam);
	calArea2(poolSteam, poolSArea);
	return retFlag;
}

// 处理得到蒸汽所有的数据
bool ImageProcess::calSteamAll()
{
	Mat tmpMat;
	getSteamS(tmpMat);
	calArea2(tmpMat, steamArea);

	Mat tmpMat2;
	getSteamS(tmpMat2);
	calHeight(tmpMat2, steamHeight);
	calDirection(tmpMat2, steamDerc);
	return true;
}


// 得到蒸汽处理的图像
void ImageProcess::getSteam(Mat &dstImage)
{
	dstImage = grayImage(steamRoi).clone();
}
void ImageProcess::getSteamS(Mat &dstImage)
{
	Mat tmpImage = grayImage(steamRoi).clone();
	threshold(tmpImage, tmpImage, 100, 255, 3);
	denosing(tmpImage, dstImage, Size(4, 4));
	//imshow("【Threshold steam】", tmpImage);
}
void ImageProcess::getSteamP(Mat &dstImage)
{
	Mat tmpImage = grayImage(steamRoi).clone();
	//threshold(tmpImage, tmpImage, 255, 255, 4);
	threshold(tmpImage, tmpImage, 80, 255, 0);
	denosing(tmpImage, dstImage, Size(8, 8));
	Mat element = getStructuringElement(MORPH_RECT, Size(5,5));
	dilate(dstImage, dstImage, element);
}

//  得到熔池处理的图像
void ImageProcess::getPool(Mat &dstImage)
{
	dstImage = grayImage(poolFRoi).clone();
}
void ImageProcess::getPoolS(Mat &dstImage)
{
	Mat tmpMat;
	tmpMat = grayImage(poolSRoi);
	threshold(tmpMat, dstImage, 100, 255, 0);
}
void ImageProcess::getPoolP(Mat &dstImage)
{
	dstImage = grayImage(poolPRoi).clone();
	////imshow("【dstImage】", dstImage);
}

// 设置熔池和蒸汽的ROI区域
void ImageProcess::setSteamROI(Rect &roi)
{
	steamRoi = roi;
}
void ImageProcess::setPoolPROI(Rect &roi)
{
	poolPRoi = roi;
}
void ImageProcess::setPoolSROI(Rect &roi)
{
	poolSRoi = roi;
}
void ImageProcess::setPoolFROI(Rect &roi)
{
	poolFRoi = roi;
}
/* 测试特征提取的方式 */
// for the test
//int main()
//{
//	ImageProcess* doPImprocess = new ImageProcess();
//	ImageProcess* doSImprocess = new ImageProcess();
//
//	Mat srcPImg = imread("C:\\Users\\ASUS\\Desktop\\img.jpg");
//	Mat srcSImg = imread("C:\\Users\\ASUS\\Desktop\\img1.jpg");
//
//	if (!srcPImg.empty() && !srcSImg.empty())
//	{
//		// 将图像传入处理器
//		cvtColor(srcPImg, doPImprocess->grayImage, CV_RGB2GRAY);
//		cvtColor(srcSImg, doSImprocess->grayImage, CV_RGB2GRAY);
//		/******* 得到处理的图像，设置在界面*****************/
//
//		// 熔池图像，熔池pool图像，熔池蒸汽图像，熔池LBP图像
//		Mat poolImage, poolPImage, poolSImage, poolBImage;
//		doPImprocess->getPool(poolImage);
//		doPImprocess->getPoolP(poolPImage);
//		doPImprocess->getPoolS(poolSImage);
//		//imshow("【熔池原图】", srcPImg);
//		//imshow("【熔池全图图像】", poolImage);
//		//imshow("【熔池前端蒸汽图像】", poolPImage);
//		//imshow("【熔池中心图像】", poolSImage);
//
//		cout << "*************************************************" << endl;
//		// 显示各个参数
//		double position, area, radiu, areaS;
//		Point2f point;
//		std::cout << "--------------------------------------" << endl;
//		doPImprocess->getPoolPP(point);
//		std::cout << "熔池中心位置： " << point << endl;
//		doPImprocess->getPoolPA(area);
//		std::cout << "熔池面积 ： " << area << endl;
//		doPImprocess->getPoolPR(radiu);
//		std::cout << "熔池半径 ： " << radiu << endl;
//		doPImprocess->getPoolSA(areaS);
//		std::cout << "熔池面积 ： " << areaS << endl;
//		std::cout << "-------------------------------------" << endl;
//		// 蒸汽图像...
//		Mat steamImage, steamPImage, steamSImage, steamBImage;
//		doSImprocess->getSteam(steamImage);
//		doSImprocess->getSteamP(steamPImage);
//		doSImprocess->getSteamS(steamSImage);
//		//imshow("【蒸汽原图】", srcSImg);
//		//imshow("【蒸汽全图图像】", steamImage);
//		//imshow("【蒸汽熔池图像】", steamPImage);
//		//imshow("【蒸汽图像】", steamSImage);
//		//SetPictureShow(IDC_PICTURE_PSTEAM4, poolBImage);
//		// 显示各个参数
//
//		double width, heigth, area1, derection;
//
//		doSImprocess->getSteamPA(width);
//		std::cout << "蒸汽面积： " << width << endl;
//		doSImprocess->getSteamPH(heigth);
//		std::cout << "熔池高度： " << heigth << endl;
//		doSImprocess->getSteamPA(area1);
//		std::cout << "蒸汽面积： " << area1 << endl;
//		doSImprocess->getSteamSD(derection);
//		std::cout << "蒸汽方向： " << derection << endl;
//		std::cout << "****************************************" << endl;
//		
//		system("pause");
//		return 0;
//	}
//}








