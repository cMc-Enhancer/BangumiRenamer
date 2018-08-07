// RenamerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Renamer.h"
#include "RenamerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRenamerDlg 对话框

CRenamerDlg::CRenamerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RENAMER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRenamerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CRenamerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Extension, &CRenamerDlg::OnBnClickedExtension)
	ON_BN_CLICKED(Open, &CRenamerDlg::OnBnClickedFileOpen)
	ON_BN_CLICKED(Reset, &CRenamerDlg::OnBnClickedReset)
	ON_BN_CLICKED(Rename, &CRenamerDlg::OnBnClickedRename)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


 //CRenamerDlg 消息处理程序

BOOL CRenamerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRenamerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRenamerDlg::OnPaint()
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
HCURSOR CRenamerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRenamerDlg::OnBnClickedExtension()
{
	CWnd *pWnd = GetDlgItem(IDC_EDIT3);
	pWnd->EnableWindow(IsDlgButtonChecked(Extension));
}

void CRenamerDlg::OnBnClickedFileOpen()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING,NULL,NULL);
	dlg.m_ofn.lpstrTitle = _T("请选择文件");
	//最多可以打开100个文件
	dlg.m_ofn.nMaxFile = 100 * MAX_PATH;
	dlg.m_ofn.lpstrFile = new TCHAR[dlg.m_ofn.nMaxFile];
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile);
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();//开始遍历用户选择文件列表
		if (clearFlag) {
			m_list.ResetContent();//清空列表框
			POSITION pt = pos;
			CString temp = dlg.GetNextPathName(pt);
			LPSTR filePath = temp.GetBuffer();
			temp.ReleaseBuffer();
			PathRemoveFileSpec(filePath);
			PathAddBackslash(filePath);
			m_list.AddString(filePath);//将路径添加到列表框
		}
		while (pos != NULL)
		{
			CString temp = dlg.GetNextPathName(pos);
			LPSTR fileName = temp.GetBuffer();
			temp.ReleaseBuffer();
			PathStripPath(fileName);
			m_list.AddString(fileName);//将文件名添加到列表框  
		}
	}
	clearFlag = false;
}

void CRenamerDlg::OnBnClickedReset()
{
	SetDlgItemText(IDC_EDIT1, _T(""));
	SetDlgItemText(IDC_EDIT2, _T(""));
	SetDlgItemText(IDC_EDIT3, _T(""));
	m_list.ResetContent();
	clearFlag = true;
}

void CRenamerDlg::OnBnClickedRename()
{
	CString name,num,ext,path,file,fileext,finalname;
	//获得编辑框中输入的信息
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(name);
	((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(num);
	if(IsDlgButtonChecked(Extension)==1)((CEdit *)GetDlgItem(IDC_EDIT3))->GetWindowText(ext);
	int number = atoi(num);
	for (int i = 1; i < m_list.GetCount(); i++) {
		m_list.GetText(i, file);
		//获得文件路径			
		m_list.GetText(0, path);
		//获得文件扩展名
		m_list.GetText(i, fileext);
		fileext = PathFindExtension(fileext);
		//判断输入序号
		if (num.GetLength() == 1)num.Format("%d", number);
		else if (num.GetLength() == 2)num.Format("%02d", number);
		//如果选择了更改扩展名
		if (IsDlgButtonChecked(Extension) == 1)finalname = path + name + " " + num + "." + ext;
		else finalname = path + name + " " + num + fileext;
		int renamecode = rename(path + file, finalname);
		//如果选择了包含字幕
		if (IsDlgButtonChecked(Sub) == 1 && !(i&1))number++;
		else number++;
		if (renamecode != 0) break;
		//更新列表
		LPSTR postname = finalname.GetBuffer();
		finalname.ReleaseBuffer();
		PathStripPath(postname);
		m_list.DeleteString(i);
		m_list.InsertString(i, postname);
	}
	clearFlag = true;
}

void CRenamerDlg::OnDropFiles(HDROP hDropInfo)
{
	char iFile[MAX_PATH];
	int iFileNumber;
	DragQueryFile(hDropInfo, 0, iFile, MAX_PATH);
	if (clearFlag) {
		m_list.ResetContent();
		PathRemoveFileSpec(iFile);
		PathAddBackslash(iFile);
		m_list.AddString(iFile);
	}
	// 得到拖拽操作中的文件个数
	iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (int i = 0; i < iFileNumber; i++)
	{
		// 得到每个文件名
		DragQueryFile(hDropInfo, i, iFile, MAX_PATH);
		if (m_list.FindString(-1, iFile) != LB_ERR)
			continue;
		// 把文件名添加到list中
		PathStripPath(iFile);
		m_list.AddString(iFile);
	}
	clearFlag = false;
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}