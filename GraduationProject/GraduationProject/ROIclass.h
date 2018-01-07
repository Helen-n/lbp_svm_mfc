#pragma once
#include <opencv2\highgui\highgui.hpp>
#include <vector>
#include <fstream>
using namespace std;

struct MyROI
{
	cv::Rect poolFROI;
	cv::Rect poolSROI;
	cv::Rect poolPROI;
	cv::Rect steamROI;
};

class LoadROI
{

public:
	LoadROI() {};
	~LoadROI() {};

public:
	std::vector<MyROI> myRoi;
	void readFormTxt(string strFilePath)
	{
		string line;
		MyROI tmpRoi;
		float x1, x2, x3, x4, y1, y2, y3, y4, w1, w2, w3, w4, h1, h2, h3, h4;
		std::ifstream trainingData(strFilePath, ios::out);
		while (getline(trainingData, line))
		{
			if (line.empty())
				continue;
			stringstream stream;
			stream << line;

			stream >> x1 >> y1 >> w1 >> h1;
			stream >> x2 >> y2 >> w2 >> h2;
			stream >> x3 >> y3 >> w3 >> h3;
			stream >> x4 >> y4 >> w4 >> h4;

			tmpRoi.poolFROI = cv::Rect(x1 ,y1, w1, h1);
			tmpRoi.poolSROI = cv::Rect(x2, y2, w2, h2);
			tmpRoi.poolPROI = cv::Rect(x3, y3, w3, h3);
			tmpRoi.steamROI = cv::Rect(x4, y4, w4, h4);

			myRoi.push_back(tmpRoi);
		}
		trainingData.close();
	}
};








