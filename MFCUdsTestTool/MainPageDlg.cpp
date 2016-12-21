// CanCommDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "MainPageDlg.h"
#include "afxdialogex.h"


INT nextrow = 0;
// CCanCommDlg 对话框

IMPLEMENT_DYNAMIC(CMainPageDlg, CDialogEx)

CMainPageDlg::CMainPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{
	m_CanRxEn = FALSE;
}

CMainPageDlg::~CMainPageDlg()
{
}

void CMainPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_CANMSG, m_List);
	DDX_Check(pDX, IDC_CHECK_RECV, m_CanRxEn);


	DDX_Text(pDX, IDC_EDIT_PRINT, m_Editprintstr);
	DDX_Control(pDX, IDC_EDIT_PRINT, m_Editprint);
}


BEGIN_MESSAGE_MAP(CMainPageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_RECV, &CMainPageDlg::OnBnClickedCheckRecv)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMainPageDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CCanCommDlg 消息处理程序

BOOL CMainPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_List.InsertColumn(0, _T("Seq"));
	m_List.SetColumnWidth(0, 40);
	m_List.InsertColumn(1, _T("TxRx"));
	m_List.SetColumnWidth(1, 40);
	m_List.InsertColumn(2, _T(" ID "));
	m_List.SetColumnWidth(2, 40);
	m_List.InsertColumn(3, _T("Dlc"));
	m_List.SetColumnWidth(3, 30);
	m_List.InsertColumn(4, _T("Data"));
	m_List.SetColumnWidth(4, 150);
	m_List.InsertColumn(5, _T("Time"));
	m_List.SetColumnWidth(5, 85);
	m_List.InsertColumn(6, _T("Chnl"));
	m_List.SetColumnWidth(6, 40);


	m_SendMsg.Create(IDD_DIALOG_SENDMSG, this);
	m_SendMsg.ShowWindow(SW_HIDE);
	m_Product.Create(IDD_DIALOG_PRODUCT, this);
	m_Product.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMainPageDlg::OnBnClickedCheckRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	/*
	if (m_CanRxEn)
	{
	StopRecv = 0;
	//开启接收线程
	AfxBeginThread(ReceiveThread, 0);
	}
	else
	StopRecv = 1;
	*/
}


void CMainPageDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Editprintstr.Empty();

	m_List.DeleteAllItems();
	UpdateData(FALSE);//更新数据
}

// Insert a Can msg to list
void CMainPageDlg::InsertItem(INT Dire, VCI_CAN_OBJ * pCanObj)
{

	UINT JustnowItem;

	//if (pCanObj->ID < theApp.m_Bgnid || pCanObj->ID > theApp.m_Endid) return;

	//发送信息列表显示
	CString str;
	CString str1;

	if (nextrow >= 59999)
	{
		m_List.DeleteAllItems();
		nextrow = 0;
	}

	str.Format(_T("%d"), nextrow);

	if (Dire == 1)
	    m_List.ItemColorFlag[nextrow] = 1;
	else
		m_List.ItemColorFlag[nextrow] = 0;

	JustnowItem = m_List.InsertItem(nextrow, str);
	nextrow++;

	if (Dire == 1)
	    m_List.SetItemText(JustnowItem, 1, _T("Send"));
	else
		m_List.SetItemText(JustnowItem, 1, _T("Recv"));


	if ((pCanObj->ExternFlag) == 1)
	{
		str1.Format(_T("%08X"), pCanObj->ID);
	}
	else
	{
		str1.Format(_T("%04X"), pCanObj->ID);
	}

	m_List.SetItemText(JustnowItem, 2, str1);	//ID信息

	str = _T("");
	str.Format(_T("%d"), pCanObj->DataLen);
	m_List.SetItemText(JustnowItem, 3, str);

	INT i;
	str = _T("");
	for (i = 0; i<pCanObj->DataLen; i++)
	{
		str1.Format(_T("%02X"), pCanObj->Data[i]);
		str = (str + str1 + _T(" "));
	}
	m_List.SetItemText(JustnowItem, 4, str);

	str.Format(_T("%d"), theApp.m_CanChnl);
	m_List.SetItemText(JustnowItem, 6, str);

	CString strTime;
	SYSTEMTIME   systime;
	GetLocalTime(&systime);
	strTime.Format(_T("%02d:%02d:%02d:%03d"), systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	m_List.SetItemText(JustnowItem, 5, strTime);


	CSize size;
	size.cx = 0;
	size.cy = 50;
	m_List.Scroll(size);
	//发送信息列表显示完毕
}


void CMainPageDlg::PrintLog(UINT LogColor, CString LogContent)
{
	m_Editprintstr += LogContent;
	m_Editprintstr += _T("\r\n");
	UpdateData(FALSE);//更新数据

	INT Index = m_Editprint.GetLineCount();
	m_Editprint.LineScroll(Index, 0);
}


void CMainPageDlg::SetOverlayDlg(UINT DlgIndex)
{
	if (DlgIndex == DlgNone)
	{
		m_SendMsg.ShowWindow(SW_HIDE);
		m_Product.ShowWindow(SW_HIDE);
	}
	else
	{
		if (DlgIndex == DlgTxMsg)
		{
			m_SendMsg.ShowWindow(SW_SHOW);
			m_Product.ShowWindow(SW_HIDE);
		}
		else
		{
			m_SendMsg.ShowWindow(SW_HIDE);
			m_Product.ShowWindow(SW_SHOW);
		}
	}
}
