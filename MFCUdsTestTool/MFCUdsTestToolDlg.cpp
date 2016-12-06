
// MFCUdsTestToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "MFCUdsTestToolDlg.h"
#include "afxdialogex.h"
#include "ControlCAN.h"
#include "UdsUtil.h"
#include "UdsClient.h"

#include "OpenDevDlg.h"
#include "RdWrDidDlg.h"
#include "UdsDiagDlg.h"
#include "CanCommDlg.h"
#include "UdsConfDlg.h"
#include "IoCtrlDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCUdsTestToolDlg �Ի���



CMFCUdsTestToolDlg::CMFCUdsTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCUDSTESTTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCUdsTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_Table);

	//DDX_Radio(pDX, IDC_RADIO_EXT, m_CanExt);
	//DDX_Radio(pDX, IDC_RADIO_RMT, m_CanRmt);
}

BEGIN_MESSAGE_MAP(CMFCUdsTestToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_BT_OPENDEV, &CMFCUdsTestToolDlg::OnBnClickedBtOpendev)
	ON_COMMAND(ID_MENU_OPENDEV, &CMFCUdsTestToolDlg::OnMenuOpendev)
	ON_COMMAND(ID_MENU_CLOSEDEV, &CMFCUdsTestToolDlg::OnMenuClosedev)
	ON_COMMAND(ID_MENU_RDID, &CMFCUdsTestToolDlg::OnMenuRdid)
	ON_COMMAND(ID_MENU_WDID, &CMFCUdsTestToolDlg::OnMenuWdid)
	ON_COMMAND(ID_MENU_RESET, &CMFCUdsTestToolDlg::OnMenuReset)
	ON_COMMAND(ID_MENU_DTCON, &CMFCUdsTestToolDlg::OnMenuDtcon)
	ON_COMMAND(ID_MENU_DTCOFF, &CMFCUdsTestToolDlg::OnMenuDtcoff)
	ON_COMMAND(ID_MENU_RDDTC, &CMFCUdsTestToolDlg::OnMenuRddtc)
	ON_COMMAND(ID_MENU_CRDTC, &CMFCUdsTestToolDlg::OnMenuCrdtc)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CMFCUdsTestToolDlg::OnTcnSelchangeTabMain)
	ON_COMMAND(ID_MENU_CC, &CMFCUdsTestToolDlg::OnMenuCc)
	ON_COMMAND(ID_MENU_UDSCONF, &CMFCUdsTestToolDlg::OnMenuUdsconf)
	ON_COMMAND(ID_MENU_IOCTRL, &CMFCUdsTestToolDlg::OnMenuIoctrl)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCUdsTestToolDlg ��Ϣ�������

BOOL CMFCUdsTestToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//���Menu
	m_Menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_Menu);

	//ΪTable control ����ҳ��
	m_Table.InsertItem(0, _T("ͨѶ"));
	m_Table.InsertItem(1, _T("���"));

	//�����Ի���
	m_CanComm.Create(IDD_DIALOG_COM, &m_Table);
	m_UdsDiag.Create(IDD_DIALOG_DIAG, &m_Table);


	//�趨��Table����ʾ�ķ�Χ
	CRect rc;
	m_Table.GetClientRect(rc);
	rc.top += 23;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_CanComm.MoveWindow(&rc);
	m_UdsDiag.MoveWindow(&rc);

	//�ѶԻ������ָ�뱣������
	pDialog[0] = &m_CanComm;
	pDialog[1] = &m_UdsDiag;
	//��ʾ��ʼҳ��
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	//���浱ǰѡ��
	m_CurSelTab = 0;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���������߳�
	//AfxBeginThread(ReceiveThread, 0);
	//Replace Recv Thread With a Timer
	SetTimer(1, TIMOUT_RECV_MS, NULL);
	//����UDS�߳�
	AfxBeginThread(UdsMainThread, &theApp.UdsClient);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCUdsTestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCUdsTestToolDlg::OnPaint()
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
HCURSOR CMFCUdsTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCUdsTestToolDlg::OnBnClickedBtOpendev()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpenDevDlg  Dlg;
	INT DlgRet;

	Dlg.p_DlgRet = &DlgRet;
	m_CanComm.PrintLog(0, _T(">>Open Can Device"));
	Dlg.DoModal();

	if (DlgRet == 0)
		m_CanComm.PrintLog(0, _T("      Done"));
	else
		m_CanComm.PrintLog(0, _T("      Fail"));
}

void CMFCUdsTestToolDlg::OnMenuOpendev()
{
	// TODO: �ڴ���������������
	COpenDevDlg  Dlg;
	INT DlgRet;

	Dlg.p_DlgRet = &DlgRet;
	m_CanComm.PrintLog(0, _T(">>Open Can Device"));
	Dlg.DoModal();

	if (DlgRet == 0)
	    m_CanComm.PrintLog(0, _T("      Done"));
	else
		m_CanComm.PrintLog(0, _T("      Fail"));
}

void CMFCUdsTestToolDlg::OnMenuClosedev()
{
	// TODO: �ڴ���������������
	m_CanComm.PrintLog(0, _T(">>Close Can Device"));

	if (VCI_CloseDevice(VCI_USBCAN2, CAN_DEVINDEX) != 1)
	{
		m_CanComm.PrintLog(0, _T("      Fail"));
	}
	else
	{
		m_CanComm.PrintLog(0, _T("      Done"));
	}
}

void CMFCUdsTestToolDlg::OnMenuRdid()
{
	// TODO: �ڴ���������������
	m_CanComm.PrintLog(0, _T(">>Start Read Data By Identifier"));

	CRdWrDidDlg  Dlg;
	Dlg.RdWr = Dlg.EmRd;
	Dlg.DoModal();

	m_CanComm.PrintLog(0, _T("      Read End"));
}



void CMFCUdsTestToolDlg::OnMenuWdid()
{
	// TODO: �ڴ���������������
	m_CanComm.PrintLog(0, _T(">>Start Write Data By Identifier"));

	CRdWrDidDlg  Dlg;
	Dlg.RdWr = Dlg.EmWr;
	Dlg.DoModal();

	m_CanComm.PrintLog(0, _T("      Write End"));
}


void CMFCUdsTestToolDlg::OnMenuReset()
{
	// TODO: �ڴ���������������
	BYTE RstBuf[BUF_LEN];
	UINT RstLen = 1;

    // Request Extened Session
	RstBuf[0] = 0x03;
	theApp.UdsClient.request(SID_10, RstBuf, RstLen);

	RstBuf[0] = 0x01;
	theApp.UdsClient.request(SID_11, RstBuf, RstLen);
	m_CanComm.PrintLog(0, _T(">>Reset Mcu"));
	m_CanComm.PrintLog(0, _T("      End"));
}



void CMFCUdsTestToolDlg::OnMenuDtcon()
{
	// TODO: �ڴ���������������
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 1;

	CmdBuf[0] = 0x01;

	theApp.UdsClient.request(SID_85, CmdBuf, CmdLen);

	m_CanComm.PrintLog(0, _T(">>Dtc Control On"));
	m_CanComm.PrintLog(0, _T("      End"));
}


void CMFCUdsTestToolDlg::OnMenuDtcoff()
{
	// TODO: �ڴ���������������
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 1;

	CmdBuf[0] = 0x02;

	theApp.UdsClient.request(SID_85, CmdBuf, CmdLen);

	m_CanComm.PrintLog(0, _T(">>Dtc Control Off"));
	m_CanComm.PrintLog(0, _T("      End"));
}


void CMFCUdsTestToolDlg::OnMenuRddtc()
{
	// TODO: �ڴ���������������
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 1;

	//ReportSupportedDTC
	CmdBuf[0] = 0x0A;

	theApp.UdsClient.request(SID_19, CmdBuf, CmdLen);

	m_CanComm.PrintLog(0, _T(">>Read Dtc By Group"));

	m_CanComm.PrintLog(0, _T("      End"));
}


void CMFCUdsTestToolDlg::OnMenuCrdtc()
{
	// TODO: �ڴ���������������
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 3;

	//All Group
	CmdBuf[0] = 0xFF;
	CmdBuf[1] = 0xFF;
	CmdBuf[2] = 0xFF;

	theApp.UdsClient.request(SID_14, CmdBuf, CmdLen);

	m_CanComm.PrintLog(0, _T(">>Clear Dtc By Group"));
	m_CanComm.PrintLog(0, _T("      End"));
}

void CMFCUdsTestToolDlg::OnMenuIoctrl()
{
	// TODO: �ڴ���������������
	m_CanComm.PrintLog(0, _T(">>Start Io Control"));

	CIoCtrlDlg  Dlg;
	Dlg.p_CanComm = &m_CanComm;
	Dlg.DoModal();
	m_CanComm.PrintLog(0, _T("      Io Control End"));
}


void CMFCUdsTestToolDlg::OnMenuCc()
{
	// TODO: �ڴ���������������
	m_CanComm.PrintLog(0, _T(">>Start Communication Control"));

	CUdsCcDlg  Dlg;
	Dlg.DoModal();

	m_CanComm.PrintLog(0, _T("      CC End"));
}


void CMFCUdsTestToolDlg::OnMenuUdsconf()
{
	// TODO: �ڴ���������������
	m_CanComm.PrintLog(0, _T(">>Start Uds Conf"));

	CUdsConfDlg  Dlg;
	Dlg.DoModal();

	m_CanComm.PrintLog(0, _T("      Uds Conf End"));
}

void CMFCUdsTestToolDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ѵ�ǰ��ҳ����������
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//�õ��µ�ҳ������
	m_CurSelTab = m_Table.GetCurSel();
	//���µ�ҳ����ʾ����
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
}


INT CMFCUdsTestToolDlg::TransmitCanmsg(VCI_CAN_OBJ *SendObj)
{
	CMFCUdsTestToolDlg *dlg = (CMFCUdsTestToolDlg*)AfxGetApp()->GetMainWnd();

	INT flag;
	//���ö�̬���ӿⷢ�ͺ���
	flag = VCI_Transmit(VCI_USBCAN2, CAN_DEVINDEX, theApp.m_CanChnl, SendObj, 1);//CAN message send

	if (flag < 1)
	{
		return flag;
	}

	dlg->m_CanComm.InsertItem(1, SendObj);
	return flag;
}

UINT CMFCUdsTestToolDlg::UdsMainThread(LPVOID v)
{
	CUdsClient *pObj = (CUdsClient *)v;
	while (1)
	{
		pObj->main_loop();
		Sleep(1);
	}
}

UINT CMFCUdsTestToolDlg::ReceiveThread(LPVOID v)
{
	CMFCUdsTestToolDlg *dlg = (CMFCUdsTestToolDlg*)AfxGetApp()->GetMainWnd();

	INT NumValue;
	INT num = 0;
	VCI_CAN_OBJ pCanObj[200];

	CString str, str1;

	DWORD ReceivedID;

	while (1)
	{

		//���ö�̬���ӿ���պ���
		NumValue = VCI_Receive(VCI_USBCAN2, CAN_DEVINDEX, theApp.m_CanChnl, pCanObj, 200, 0);

		//������Ϣ�б���ʾ
		CString strTime;
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		strTime.Format(_T("%02d:%02d:%02d:%03d"), systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

		for (num = 0; num<NumValue; num++)
		{

			ReceivedID = pCanObj[num].ID;

			if (ReceivedID == theApp.m_Rspid)
				theApp.UdsClient.netowrk_recv_frame(0, pCanObj[num].Data, pCanObj[num].DataLen);
			dlg->m_CanComm.InsertItem(0, pCanObj);
		}
	
		if (theApp.UdsClient.n_ResultErr == TRUE)
		{
			theApp.UdsClient.n_ResultErr = FALSE;
			str.Format(_T(">>NetWork Err %u"), theApp.UdsClient.n_Result);
			dlg->m_CanComm.PrintLog(0, _T(">>NetWork Err"));
		}

		Sleep(5);
	}

	return 1;
}


void CMFCUdsTestToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	INT NumValue;
	INT num = 0;
	VCI_CAN_OBJ pCanObj[200];

	CString str, str1;

	DWORD ReceivedID;

	//���ö�̬���ӿ���պ���
	NumValue = VCI_Receive(VCI_USBCAN2, CAN_DEVINDEX, theApp.m_CanChnl, pCanObj, 200, 0);

	for (num = 0; num<NumValue; num++)
	{

		ReceivedID = pCanObj[num].ID;

		if (ReceivedID == theApp.m_Rspid)
			theApp.UdsClient.netowrk_recv_frame(0, pCanObj[num].Data, pCanObj[num].DataLen);
		m_CanComm.InsertItem(0, pCanObj);
	}

	if (theApp.UdsClient.n_ResultErr == TRUE)
	{
		theApp.UdsClient.n_ResultErr = FALSE;
		str.Format(_T(">>NetWork Err %u"), theApp.UdsClient.n_Result);
		m_CanComm.PrintLog(0, _T(">>NetWork Err"));
	}

	CDialogEx::OnTimer(nIDEvent);

}
