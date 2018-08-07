// RenamerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Renamer.h"
#include "RenamerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRenamerDlg �Ի���

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


 //CRenamerDlg ��Ϣ�������

BOOL CRenamerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRenamerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRenamerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	dlg.m_ofn.lpstrTitle = _T("��ѡ���ļ�");
	//�����Դ�100���ļ�
	dlg.m_ofn.nMaxFile = 100 * MAX_PATH;
	dlg.m_ofn.lpstrFile = new TCHAR[dlg.m_ofn.nMaxFile];
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile);
	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();//��ʼ�����û�ѡ���ļ��б�
		if (clearFlag) {
			m_list.ResetContent();//����б��
			POSITION pt = pos;
			CString temp = dlg.GetNextPathName(pt);
			LPSTR filePath = temp.GetBuffer();
			temp.ReleaseBuffer();
			PathRemoveFileSpec(filePath);
			PathAddBackslash(filePath);
			m_list.AddString(filePath);//��·����ӵ��б��
		}
		while (pos != NULL)
		{
			CString temp = dlg.GetNextPathName(pos);
			LPSTR fileName = temp.GetBuffer();
			temp.ReleaseBuffer();
			PathStripPath(fileName);
			m_list.AddString(fileName);//���ļ�����ӵ��б��  
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
	//��ñ༭�����������Ϣ
	((CEdit *)GetDlgItem(IDC_EDIT1))->GetWindowText(name);
	((CEdit *)GetDlgItem(IDC_EDIT2))->GetWindowText(num);
	if(IsDlgButtonChecked(Extension)==1)((CEdit *)GetDlgItem(IDC_EDIT3))->GetWindowText(ext);
	int number = atoi(num);
	for (int i = 1; i < m_list.GetCount(); i++) {
		m_list.GetText(i, file);
		//����ļ�·��			
		m_list.GetText(0, path);
		//����ļ���չ��
		m_list.GetText(i, fileext);
		fileext = PathFindExtension(fileext);
		//�ж��������
		if (num.GetLength() == 1)num.Format("%d", number);
		else if (num.GetLength() == 2)num.Format("%02d", number);
		//���ѡ���˸�����չ��
		if (IsDlgButtonChecked(Extension) == 1)finalname = path + name + " " + num + "." + ext;
		else finalname = path + name + " " + num + fileext;
		int renamecode = rename(path + file, finalname);
		//���ѡ���˰�����Ļ
		if (IsDlgButtonChecked(Sub) == 1 && !(i&1))number++;
		else number++;
		if (renamecode != 0) break;
		//�����б�
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
	// �õ���ק�����е��ļ�����
	iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (int i = 0; i < iFileNumber; i++)
	{
		// �õ�ÿ���ļ���
		DragQueryFile(hDropInfo, i, iFile, MAX_PATH);
		if (m_list.FindString(-1, iFile) != LB_ERR)
			continue;
		// ���ļ�����ӵ�list��
		PathStripPath(iFile);
		m_list.AddString(iFile);
	}
	clearFlag = false;
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}