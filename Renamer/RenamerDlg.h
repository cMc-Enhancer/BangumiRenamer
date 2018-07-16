
// RenamerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CRenamerDlg 对话框
class CRenamerDlg : public CDialogEx
{
// 构造
public:
	CRenamerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RENAMER_DIALOG };
#endif

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
	afx_msg void OnBnClickedExtension();
	afx_msg void OnBnClickedFileOpen();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedRename();
	CListBox m_list;
private:
	boolean clearFlag=true;

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
