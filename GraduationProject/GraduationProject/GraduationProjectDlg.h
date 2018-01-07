
// GraduationProjectDlg.h : 头文件
//
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "afxwin.h"
#include "afxcmn.h"

#include "ROIclass.h"
#include "LBP.h"
#include "SVM.h"


// CGraduationProjectDlg 对话框
class CGraduationProjectDlg : public CDialogEx
{
// 构造
public:
	CGraduationProjectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据

	enum { IDD = IDD_GRADUATIONPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedMfcbuttonOpen();
	afx_msg void OnBnClickedTuttonProcess();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedMfcbuttonTrain();

public:
	
	CString        m_trainDataPath;   // 训练文件路径-txt
	vector<string> m_imagePPaths;     // 熔池路径
	vector<string> m_imageSPaths;     // 蒸汽路径
	vector<int>    m_imagePROIs;      // ROI区域
	vector<int>    m_imageSROIs;      // ROI区域
	vector<int>    m_imagePClasses;   // 熔池标签
	vector<int>    m_imageSClasses;   // 蒸汽标签

	Mat featureVectorsOfSample;  // 保存每个图像的LBP特征和参数特征
	Mat classOfSample;           // 保存每个图像的分类

	LBP            m_lbp;
	SVMTest        m_svm;

	// SVM 参数部分
	float m_editc;
	float m_editp;
	float m_editnu;
	float m_deitcoef;
	float m_editgama;
	float m_editdeg;
	int   m_radiotn;

public:
	CEdit          m_ControlPath;	  // 显示选中的地址
	CStatic        m_picturePool;     // 显示熔池图像
	CStatic        m_pictureSteam;    // 显示蒸汽的图像
	CMFCTabCtrl      m_tab;
	CComboBox        m_kernelType;
	CComboBox        m_svmType;

public:
	void SetPictureShow(int nID_PICTURE, Mat &mat);
	void SetTextShow(int nID_CHECK, int nID_TEXT, double value);
	void MatToCImage(Mat &mat, CImage &cImage);
	void getFileList();
	void BeginImgProcess();
	void BeginTrain();
	void BaginTest();

	void loadVectorParam(string path);
	void autoTrainParam();
	void getPath(string strPath);
	void collectLBPPic();
	void selectParam();
	afx_msg void OnBnClickedMfcbuttonTrainauto();
	CProgressCtrl m_process;
	afx_msg void OnBnClickedMfcbuttontest();
};
