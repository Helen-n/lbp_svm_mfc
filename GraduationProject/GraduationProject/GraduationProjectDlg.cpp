// GraduationProjectDlg.cpp : 实现文件

#include "stdafx.h"
#include "GraduationProject.h"
#include "GraduationProjectDlg.h"
#include "afxdialogex.h"
#include "ImageProcess.h"
#include <fstream>
#include <thread>
#include <chrono>         // std::chrono::seconds

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CGraduationProjectDlg 对话框

CGraduationProjectDlg::CGraduationProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraduationProjectDlg::IDD, pParent)
	, m_editc(1.0)
	, m_editp(5e-3)
	, m_editnu(0.5)
	, m_deitcoef(0)
	, m_editgama(0.5)
	, m_editdeg(3)
	, m_radiotn(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraduationProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_POOL, m_picturePool);
	DDX_Control(pDX, IDC_PICTURE_STEAM, m_pictureSteam);
	DDX_Control(pDX, IDC_EDIT_PATH, m_ControlPath);
	DDX_Text(pDX, IDC_EDIT_C, m_editc);
	DDX_Text(pDX, IDC_EDIT_P, m_editp);
	DDX_Text(pDX, IDC_EDIT_NU, m_editnu);
	DDX_Text(pDX, IDC_EDIT_COEF, m_deitcoef);
	DDX_Text(pDX, IDC_EDIT5, m_editgama);
	DDX_Text(pDX, IDC_EDIT6, m_editdeg);
	DDX_Control(pDX, IDC_COMBOM_KERNEL, m_kernelType);
	DDX_Control(pDX, IDC_COMBOM_TYPE, m_svmType);
	DDX_Control(pDX, IDC_PROGRESS1, m_process);
}

BEGIN_MESSAGE_MAP(CGraduationProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_OPEN, &CGraduationProjectDlg::OnBnClickedMfcbuttonOpen)
	ON_BN_CLICKED(IDC_TUTTON_PROCESS, &CGraduationProjectDlg::OnBnClickedTuttonProcess)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO1, &CGraduationProjectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CGraduationProjectDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CGraduationProjectDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CGraduationProjectDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_MFCBUTTON_TRAIN, &CGraduationProjectDlg::OnBnClickedMfcbuttonTrain)
	ON_BN_CLICKED(IDC_MFCBUTTON_TRAINAUTO, &CGraduationProjectDlg::OnBnClickedMfcbuttonTrainauto)
	ON_BN_CLICKED(IDC_MFCBUTTONTest, &CGraduationProjectDlg::OnBnClickedMfcbuttontest)
END_MESSAGE_MAP()

// CGraduationProjectDlg 消息处理程序

BOOL CGraduationProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//TODO: 在此添加额外的初始化代码
	// 设置初始化check box 
    ((CButton*)GetDlgItem(IDC_CHECK_PP))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PA2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PMAX))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_PMIN))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SPA))->SetCheck(TRUE);

	((CButton*)GetDlgItem(IDC_CHECK_SH))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SA2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_SD))->SetCheck(TRUE);


	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	m_kernelType.SetCurSel(2);
	m_svmType.SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGraduationProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGraduationProjectDlg::OnPaint()  
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGraduationProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* 显示图像，文字，选项设置等等*/
// mat转成Image用于显示
void CGraduationProjectDlg::MatToCImage(Mat &mat, CImage &cImage)
{
	//create new CImage
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	cImage.Destroy(); //clear
	cImage.Create(width, height, 8 * channels); //默认图像像素单通道占用1个字节

	//copy values
	uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //A pointer to the bitmap buffer
	int step = cImage.GetPitch();

	if (1 == channels)                             //对于单通道的图像需要初始化调色板  
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cImage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cImage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}

	for (int i = 0; i < height; ++i)
	{
		ps = (mat.ptr<uchar>(i));
		for (int j = 0; j < width; ++j)
		{
			if (channels == 1) //gray
			{
				*(pimg + i*step + j) = ps[j];
			}
			else if (channels == 3) //color
			{
				for (int k = 0; k < 3; ++k)
				{
					*(pimg + i*step + j * 3 + k) = ps[j * 3 + k];
				}
			}
		}
	}
}
// 用于控件显示图像
void CGraduationProjectDlg::SetPictureShow(int nID_PICTURE, Mat &mat)
{
	CImage cImg;
	MatToCImage(mat, cImg);
	CRect picRect;
	GetDlgItem(nID_PICTURE)->GetWindowRect(picRect);   //获取Picture Control控件的大小 
	ScreenToClient(picRect);                           //将客户区选中到控件表示的矩形区域内 
	GetDlgItem(nID_PICTURE)->MoveWindow(picRect.left,  //窗口移动到控件表示的区域  
	    picRect.top, picRect.Width(), picRect.Height(), TRUE);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(nID_PICTURE);                    //获取控件句柄  
	pWnd->GetClientRect(picRect);                      //获取句柄指向控件区域的大小  
	CDC *pDc = NULL;
	pDc = pWnd->GetDC();                               //获取picture的DC 
	cImg.Draw(pDc->m_hDC, picRect);                     //将图片绘制到picture表示的区域内  
	ReleaseDC(pDc);
}
// 用于控件显示文字
void CGraduationProjectDlg::SetTextShow(int nID_CHECK, int nID_TEXT, double value)
{
	CString tmpStr;
	CButton *pCheckbox;
	pCheckbox = (CButton*)GetDlgItem(nID_CHECK);
	if (pCheckbox->GetCheck())
	{
		tmpStr.Format(_T("%f"), value);
		GetDlgItem(nID_TEXT)->SetWindowTextW(tmpStr);
	}
}
/* 显示图像，文字，选项设置等等*/

/* 主要按钮的事件 */
// 打开按钮
void CGraduationProjectDlg::OnBnClickedMfcbuttonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	getFileList();
}
// 图像处理按钮
void CGraduationProjectDlg::OnBnClickedTuttonProcess()
{
	BeginImgProcess();
}
// 训练模型按钮
void CGraduationProjectDlg::OnBnClickedMfcbuttonTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	/* CUSTOM=-1,   LINEAR = 0, POLY = 1, RBF = 2, SIGMOID = 3, CHI2 = 4,  INTER = 5 */
	m_svm.c = m_editc;
	m_svm.coef = m_deitcoef;
	m_svm.degree = m_editdeg;
	m_svm.gamma = m_editgama;
	m_svm.kernel = SVM::KernelTypes(m_kernelType.GetCurSel());
	m_svm.nu = m_editnu;
	m_svm.p = m_editp;
	m_svm.svmType = SVM::Types::C_SVC;

	m_svm.Initialize();
	//m_svm.svmModelFilePath = "Liner-Kernel.xml";
	m_svm.Train(featureVectorsOfSample, classOfSample);

	// 训练完获得优化的参数
	m_editc = m_svm.svm->getC();
	m_deitcoef = m_svm.svm->getCoef0();
	m_editdeg = m_svm.svm->getDegree();
	m_editgama = m_svm.svm->getGamma();
	m_editnu = m_svm.svm->getNu();
	m_editp = m_svm.svm->getP();
	UpdateData(TRUE);
}
// 测试模型按钮
void CGraduationProjectDlg::OnBnClickedMfcbuttontest()
{
	// TODO: 在此添加控件通知处理程序代码
	m_svm.predictResultFilePath = "ExperienceTable\\result\\result.txt";
	getFileList();
	BeginImgProcess();
	m_svm.Predict(featureVectorsOfSample, classOfSample);
}
// 自动训练按钮
void CGraduationProjectDlg::OnBnClickedMfcbuttonTrainauto()
{
	// TODO: 在此添加控件通知处理程序代码
	// autoTrainParam();
	//collectLBPPic();
	 selectParam();
}
/* 主要按钮的事件 */

/* 选择LBP模式 */
void CGraduationProjectDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radiotn = 0;
}
void CGraduationProjectDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radiotn = 1;
}
void CGraduationProjectDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radiotn = 2;
}
void CGraduationProjectDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radiotn = 3;
}
/* 选择LBP模式 */

// 进行图像处理
void CGraduationProjectDlg::BeginImgProcess()
{
	// SVM训练的Vector
	Mat featureSVector;          // 蒸汽的LBP图像
	Mat featurePVector;          // 熔池的LBP图像
	Mat featureDVector;          // 参数的特征

	if (m_imagePPaths.size())
	{
		if (!featureVectorsOfSample.empty())
		    featureVectorsOfSample.release();

		if (!classOfSample.empty())
			classOfSample.release();
		int i;
		ImageProcess doPImprocess;
		ImageProcess doSImprocess;

		LoadROI roi;
		roi.readFormTxt("ExperienceTable\\SelectParam-roiParam.txt");
		roi.myRoi; 

		m_process.SetRange(0, m_imagePPaths.size());
		m_process.SetStep(1);
		/*ofstream outfile("kernel-imgprocess-param.xls");
		outfile << "positionX" << "\t" << "positionY" << "\t" << "area"  << "\t" << "lmax" << "\t" << "lmin" << "\t"
			    << "heigth"   << "\t" << "area1" << "\t" << "derection" << "\t" << endl;*/
		for (i = 0; i < m_imageSPaths.size(); ++i)
		{   
			Mat srcPImg = imread(m_imagePPaths[i].c_str());
			Mat srcSImg = imread(m_imageSPaths[i].c_str());
			if (!srcPImg.empty() && !srcSImg.empty())
			{
				// 将图像传入处理器
				cvtColor(srcPImg, doPImprocess.grayImage, CV_RGB2GRAY);
				cvtColor(srcSImg, doSImprocess.grayImage, CV_RGB2GRAY);
				/******* 得到处理的图像，设置在界面*****************/

				double position(0), area(0), lmax(0), lmin(0), area2(0);
				double width(0), heigth(0), area1(0), derection(0);

				int roiIndex = m_imagePROIs[i]-1;
				doPImprocess.setPoolFROI(roi.myRoi[roiIndex].poolFROI);
				doPImprocess.setPoolSROI(roi.myRoi[roiIndex].poolSROI);
				doPImprocess.setPoolPROI(roi.myRoi[roiIndex].poolPROI);
				doSImprocess.setSteamROI(roi.myRoi[roiIndex].steamROI);

				

				// 熔池图像，熔池pool图像，熔池蒸汽图像，熔池LBP图像
				Mat poolImage, poolPImage, poolSImage, poolBImage;
				doPImprocess.getPool(poolImage);
				doPImprocess.getPoolP(poolPImage);
				doPImprocess.getPoolS(poolSImage);
				SetPictureShow(IDC_PICTURE_POOL, srcPImg);
				SetPictureShow(IDC_PICTURE_PPOOL1, poolImage);
				SetPictureShow(IDC_PICTURE_PPOOL2, poolPImage);
				

				// 显示各个参数
				doPImprocess.calPoolAll();
				Point2f point = doPImprocess.poolCenter;
				area  = doPImprocess.poolPArea;
				lmax  = doPImprocess.poolMaxDist;
				lmin = doPImprocess.poolMinDist;
				area2 = doPImprocess.poolSArea;

				SetTextShow(IDC_CHECK_PP, IDC_POOL_POSITION, point.x);
				SetTextShow(IDC_CHECK_PA2,IDC_POOL_AREA ,area);
				SetTextShow(IDC_CHECK_PMAX, IDC_POOL_MAX,lmax);
				SetTextShow(IDC_CHECK_PMIN, IDC_POOL_MIN,lmin);
				SetTextShow(IDC_CHECK_SPA, IDC_POOL_SA,area2);

				// 蒸汽图像...
				Mat steamImage, steamPImage, steamSImage, steamBImage;
				doSImprocess.getSteam(steamImage);
				doSImprocess.getSteamP(steamPImage);
				doSImprocess.getSteamS(steamSImage);
				SetPictureShow(IDC_PICTURE_STEAM, srcSImg);
				SetPictureShow(IDC_PICTURE_PSTEAM1, steamImage);
				SetPictureShow(IDC_PICTURE_PSTEAM2, steamPImage);
				
				
				// 显示各个参数
				doSImprocess.calSteamAll();
				heigth = doSImprocess.steamHeight;
				area1 = doSImprocess.steamArea;
				derection = doSImprocess.steamDerc;

				SetTextShow(IDC_CHECK_SH, IDC_STEAM_HEIGHT,heigth);
				SetTextShow(IDC_CHECK_SA2, IDC_STEAM_AREA,area1);
				SetTextShow(IDC_CHECK_SD, IDC_STEAM_DEREC,derection);

				Mat tmpPoolImage, tmpSteamImage;
				resize(poolImage, tmpPoolImage, Size(CELL_SIZE*4, CELL_SIZE*4));
				resize(steamImage, tmpSteamImage, Size(CELL_SIZE*4, CELL_SIZE*4));
				
				switch (m_radiotn)
				{
				  case 0:
					  m_lbp.ComputeLBPFeatureVector_Circle(tmpPoolImage, Size(CELL_SIZE, CELL_SIZE), featureSVector);
					  m_lbp.ComputeLBPFeatureVector_Circle(tmpSteamImage, Size(CELL_SIZE, CELL_SIZE), featurePVector);

					  m_lbp.ComputeLBPImage_Circle(tmpPoolImage,poolSImage);
					  m_lbp.ComputeLBPImage_Circle(tmpSteamImage,steamSImage);
					  break;
				  case 1:
					  m_lbp.ComputeLBPFeatureVector_256(tmpPoolImage, Size(CELL_SIZE, CELL_SIZE), featureSVector);
					  m_lbp.ComputeLBPFeatureVector_256(tmpSteamImage, Size(CELL_SIZE, CELL_SIZE), featurePVector);

					  m_lbp.ComputeLBPImage_256(tmpPoolImage, poolSImage);
					  m_lbp.ComputeLBPImage_256(tmpSteamImage, steamSImage);
					  break;
				  case 2:
					  m_lbp.ComputeLBPFeatureVector_Uniform(tmpPoolImage, Size(CELL_SIZE, CELL_SIZE), featureSVector);
					  m_lbp.ComputeLBPFeatureVector_Uniform(tmpSteamImage, Size(CELL_SIZE, CELL_SIZE), featurePVector);

					  m_lbp.ComputeLBPImage_Uniform(tmpPoolImage, poolSImage);
					  m_lbp.ComputeLBPImage_Uniform(tmpSteamImage, steamSImage);
					  break;
				  default:
					  m_lbp.ComputeLBPFeatureVector_Rotation_Uniform(tmpPoolImage, Size(CELL_SIZE, CELL_SIZE), featureSVector);
					  m_lbp.ComputeLBPFeatureVector_Rotation_Uniform(tmpSteamImage, Size(CELL_SIZE, CELL_SIZE), featurePVector);

					  m_lbp.ComputeLBPImage_Rotation_Uniform(tmpPoolImage, poolSImage);
					  m_lbp.ComputeLBPImage_Rotation_Uniform(tmpSteamImage, steamSImage);
					  break;
				}

				SetPictureShow(IDC_PICTURE_PSTEAM3, steamSImage);
				SetPictureShow(IDC_PICTURE_PPOOL3, poolSImage);

				char text[100];
				sprintf(text,"%d张图像已经处理，共%d张图片",i+1, m_imageSPaths.size());
				CString ctext(text);
				GetDlgItem(IDC_STATIC_TEXT)->SetWindowTextW(ctext);
				
				m_process.StepIt();
				
				featureDVector = (Mat_<float>(1, CELL_SIZE/2) <<  position, area, lmax,lmin, width, heigth, area1, derection);
				Mat nfeatureDVector;
				Mat tempVectorsOfSample(1,(featureSVector.cols + featurePVector.cols + featureDVector.cols),CV_32FC1,Scalar(0));

				featureSVector.copyTo(tempVectorsOfSample(Rect(0, 0, featureSVector.cols, 1)));
				featurePVector.copyTo(tempVectorsOfSample(Rect(featureSVector.cols, 0, featurePVector.cols, 1)));
				featureDVector.copyTo(tempVectorsOfSample(Rect(featureSVector.cols + featurePVector.cols, 0,
					                                      featureDVector.cols, 1)));

				featureVectorsOfSample.push_back(tempVectorsOfSample);
 				classOfSample.push_back((float)m_imagePClasses[i]);
	
				//outfile << point.x << "\t" << point.y << "\t" << area << "\t" << lmax << "\t" << lmin << "\t"
				//	<< heigth << "\t" << area1 << "\t" << derection << "\t" << endl;
			}
		}
		//outfile.close();

		// 归一化
		for (int col = featureSVector.cols + featurePVector.cols; col < featureSVector.cols + featurePVector.cols + featureDVector.cols; ++col)
		{
			Mat mask(featureVectorsOfSample.size(), CV_8UC1, Scalar(0));
			mask.col(col) = Scalar(255);
			normalize(featureVectorsOfSample, featureVectorsOfSample, 1.0, 0.0, NORM_MINMAX,-1,mask);
			featureVectorsOfSample.col(col) /= 100;
		}
	}
	else
	{
		AfxMessageBox(_T("没有找到图像"));
	}
	UpdateData(true);
}

void CGraduationProjectDlg::BeginTrain()
{}

void CGraduationProjectDlg::BaginTest()
{}

// 原本为了节省时间，将训练的参数的保存了起来，下次使用，直接加载，未完成
void CGraduationProjectDlg::loadVectorParam(string path)
{
	Mat mat;
	string line;
	int first = 1; // 截断第一个[

	float num;
	string mark;
	std::ifstream trainingData(path, ios::out);
	while (getline(trainingData, line))
	{
		if (line.empty())
			continue;
		if (first)
		{
			line.replace(line.find("["), 1, ""); // 去除头部的
			first = 0;
		}
		stringstream stream;
		stream << line;

		Mat matrow;
		while (stream)
		{
			stream >> num >> mark;
			matrow.push_back(num);
			if (mark == ";" || mark == "]")
				break;
		}
		mat.push_back(matrow.reshape(0, 1));
	}
	trainingData.close();
}

// 自动化训练，得到所有数据,未完成，请自行准备数据集
void CGraduationProjectDlg::autoTrainParam()
{
	// 1.不同的内核
	// 2.不同的LBP模式
	// 3.不容参数下的模型以及测试数据集

	// 同一LBP模式下的不同内核
	
	for (int k = 0; k < 4; ++k)
	{
		for (int m = 0; m < 4; m++)
		{
			getPath("");
			m_radiotn = k;
			BeginImgProcess();
			m_kernelType.SetCurSel(m);
			char modelPath[50], resultPath[50];
			sprintf(modelPath, "Model-LbpType-%dKernelType-%d.xml",k,m);
			sprintf(resultPath, "Result-LbpType-%dKernelType-%d.txt", k, m);
			m_svm.svmModelFilePath = modelPath;
			OnBnClickedMfcbuttonTrain();
			m_svm.predictResultFilePath = resultPath;
			getPath("");
			BeginImgProcess();
			m_svm.Predict(featureVectorsOfSample, classOfSample);
		}
	}
}

// 得到图像的路径
void CGraduationProjectDlg::getPath(string strFilePath)
{
	m_imagePPaths.clear();
	m_imagePClasses.clear();
	m_imagePROIs.clear();
	m_imageSPaths.clear();
	m_imageSClasses.clear();
	m_imageSROIs.clear();
	string line;
	std::ifstream trainingData(strFilePath, ios::out);
	while (getline(trainingData, line))
	{
		if (line.empty())
			continue;
		stringstream stream;
		stream << line;
		string imagePath, imageClass, roiIndex;
		stream >> imagePath;
		stream >> imageClass;
		stream >> roiIndex;
		if (imagePath.find("BW") != imagePath.npos)
		{
			m_imagePPaths.push_back(imagePath);
			m_imagePClasses.push_back(atoi(imageClass.c_str()));
			m_imagePROIs.push_back(atoi(roiIndex.c_str()));
		}
		else
		{
			m_imageSPaths.push_back(imagePath);
			m_imageSClasses.push_back(atoi(imageClass.c_str()));
			m_imageSROIs.push_back(atoi(roiIndex.c_str()));
		}

	}
	trainingData.close();
}

// 保存LBP处理后的图像
void CGraduationProjectDlg::collectLBPPic()
{
	char path[100];
	Rect poolRoi(210, 20, 150, 200);
	Rect stemRoi(80, 20, 280, 300);
	for (int i = 1; i < 7; ++i)
	{
		sprintf(path, "C:\\Users\\ASUS\\Desktop\\new file\\%d.jpg", i);

		Mat src1 = imread(path);
		cvtColor(src1,src1,CV_RGB2GRAY);

		Mat src;

			if (i<4)
			{
				src = src1(poolRoi);
			}
			else
			{
				src = src1(stemRoi);;
			}

			Mat circle;
			m_lbp.ComputeLBPImage_Circle(src, circle);
			imshow("【Circle】", circle);

			Mat lbp256;
			m_lbp.ComputeLBPImage_256(src, lbp256);
			imshow("【lbp256】", lbp256);

			Mat uniform;
			m_lbp.ComputeLBPImage_Uniform(src, uniform);
			imshow("【uniform】", uniform);

			Mat rotation;
			m_lbp.ComputeLBPImage_Rotation_Uniform(src, rotation);
			imshow("【Rotation】", rotation);

			imshow("【SRC】",src);
			waitKey(0);

		
	}
	
}

// 选择训练的参数
void CGraduationProjectDlg::selectParam()
{
	// 选取不同的C和gamma来
	for (int k = 2; k < 180; k *= 2)
	{
		for (double m = 1; m > 0.003; m/=2)
		{
			getPath("D:\\Experience\\ExperienceTable\\3selectParam\\SelectParam-053-01-06-500-train.txt");
			BeginImgProcess();
			m_radiotn = 1;
			m_kernelType.SetCurSel(2);

			char modelPath[50], resultPath[50];
			sprintf(modelPath, "C-%dGama-%f.xml", k, m);
			sprintf(resultPath, "C-%dGama-%f.txt", k, m);
			m_svm.svmModelFilePath = modelPath;
			m_editc = k;
			m_editgama = m;
			OnBnClickedMfcbuttonTrain(); 
			m_svm.predictResultFilePath = resultPath;
			getPath("D:\\Experience\\ExperienceTable\\3selectParam\\SelectParam-test-o.txt");
			BeginImgProcess();
			m_svm.Predict(featureVectorsOfSample, classOfSample);
		}
	}
}

// 获取文件列表
void CGraduationProjectDlg::getFileList()
{
	m_imageSPaths.clear();
	m_imageSClasses.clear();
	m_imageSROIs.clear();

	m_imagePPaths.clear();
	m_imagePClasses.clear();
	m_imagePROIs.clear();

	// 设置过滤器   
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		m_trainDataPath = strFilePath;
		m_ControlPath.SetWindowTextW(strFilePath);
	}

	string line;
	std::ifstream trainingData(strFilePath, ios::out);
	while (getline(trainingData, line))
	{
		if (line.empty())
			continue;
		stringstream stream;
		stream << line;
		string imagePath, imageClass, roiIndex;
		stream >> imagePath;
		stream >> imageClass;
		stream >> roiIndex;
		if (imagePath.find("BW") != imagePath.npos)
		{
			m_imagePPaths.push_back(imagePath);
			m_imagePClasses.push_back(atoi(imageClass.c_str()));
			m_imagePROIs.push_back(atoi(roiIndex.c_str()));
		}
		else
		{
			m_imageSPaths.push_back(imagePath);
			m_imageSClasses.push_back(atoi(imageClass.c_str()));
			m_imageSROIs.push_back(atoi(roiIndex.c_str()));
		}
	}
	trainingData.close();
}


