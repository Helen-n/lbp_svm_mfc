
// GraduationProjectDlg.h : ͷ�ļ�
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


// CGraduationProjectDlg �Ի���
class CGraduationProjectDlg : public CDialogEx
{
// ����
public:
	CGraduationProjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������

	enum { IDD = IDD_GRADUATIONPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	
	CString        m_trainDataPath;   // ѵ���ļ�·��-txt
	vector<string> m_imagePPaths;     // �۳�·��
	vector<string> m_imageSPaths;     // ����·��
	vector<int>    m_imagePROIs;      // ROI����
	vector<int>    m_imageSROIs;      // ROI����
	vector<int>    m_imagePClasses;   // �۳ر�ǩ
	vector<int>    m_imageSClasses;   // ������ǩ

	Mat featureVectorsOfSample;  // ����ÿ��ͼ���LBP�����Ͳ�������
	Mat classOfSample;           // ����ÿ��ͼ��ķ���

	LBP            m_lbp;
	SVMTest        m_svm;

	// SVM ��������
	float m_editc;
	float m_editp;
	float m_editnu;
	float m_deitcoef;
	float m_editgama;
	float m_editdeg;
	int   m_radiotn;

public:
	CEdit          m_ControlPath;	  // ��ʾѡ�еĵ�ַ
	CStatic        m_picturePool;     // ��ʾ�۳�ͼ��
	CStatic        m_pictureSteam;    // ��ʾ������ͼ��
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
