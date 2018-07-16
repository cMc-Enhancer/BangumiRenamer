
// RenamerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CRenamerDlg �Ի���
class CRenamerDlg : public CDialogEx
{
// ����
public:
	CRenamerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RENAMER_DIALOG };
#endif

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
