// UdsDiagDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "UdsDiagDlg.h"
#include "afxdialogex.h"

#include "UdsClient.h"



static obd_dtc_t obd_dtc_list[OBD_DTC_CNT] =
{
	{_T("燃油传感器过低"), _T("P04621A"), 0x04621A, 0, 0 },
	{_T("燃油传感器过高"), _T("P04631B"), 0x04631B, 0, 0 },
	{_T("电池电压过低"),   _T("B111716"), 0x911716, 0, 0 },
	{_T("电池电压过高"),   _T("B111717"), 0x911717, 0, 0 },
	{_T("ECU总线关闭"),    _T("U007300"), 0xC07300, 0, 0 },
	{_T("ABS通信丢失"),    _T("U012100"), 0xC12100, 0, 0 },
	{_T("EPB通信丢失"),    _T("U012800"), 0xC12800, 0, 0 },
	{_T("EPS通信丢失"),    _T("U013100"), 0xC13100, 0, 0 },
	{_T("BCM通信丢失"),    _T("U014100"), 0xC14100, 0, 0 },
	{_T("SRS通信丢失"),    _T("U120000"), 0xD20000, 0, 0 },
	{_T("PEPS通信丢失"),   _T("U021400"), 0xC21400, 0, 0 },
	{_T("VCU通信丢失"),    _T("U010200"), 0xC10200, 0, 0 },
	{_T("MCU通信丢失"),    _T("U010300"), 0xC10300, 0, 0 },
	{_T("BMS通信丢失"),    _T("U011200"), 0xC11200, 0, 0 },
	{_T("MP5通信丢失"),    _T("U016300"), 0xC16300, 0, 0 },
	{_T("刹车指示灯出错"), _T("B153D01"), 0x953D01, 0, 0 },
	{_T("EPC指示灯出错"),  _T("B153E01"), 0x953E01, 0, 0 },
	{_T("TCU指示灯出错"),  _T("B153F01"), 0x953F01, 0, 0 },
	{_T("AIRBAG指示灯出错"),  _T("B154001"), 0x954001, 0, 0},
	{_T("ESC指示灯出错"),  _T("B154101"), 0x954101, 0, 0 },
	{_T("ABS指示灯出错"),  _T("B154401"), 0x954401, 0, 0 },
	{_T("MIL指示灯出错"),  _T("B154501"), 0x954501, 0, 0 },
	{_T("EPB故障灯出错"),  _T("B154601"), 0x954601, 0, 0 },
	{_T("ESC关闭灯出错"),  _T("B154701"), 0x954701, 0, 0 }
};

static CString DtcStBit[DTCST_NUM] = {
	_T("TestFailed"),
	_T("TestFailedThisOperationCycle"),
	_T("Pending DTC"),
	_T("Confirmed DTC"),
	_T("TestNotCompletedSinceLastClear"),
	_T("TestFailedSinceLastClear "),
	_T("TestNotCompletedThisOperationCycle"),
	_T("WarningIndicatorRequested")
};
// CUdsDiagDlg 对话框

IMPLEMENT_DYNAMIC(CUdsDiagDlg, CDialogEx)

CUdsDiagDlg::CUdsDiagDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DIAG, pParent)
{

}

CUdsDiagDlg::~CUdsDiagDlg()
{
}

void CUdsDiagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_DTC, m_List);
	DDX_Control(pDX, IDC_LIST_DTCST, m_ListSt);
}


BEGIN_MESSAGE_MAP(CUdsDiagDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SPTDTC, &CUdsDiagDlg::OnBnClickedButtonSptdtc)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLRDTC, &CUdsDiagDlg::OnBnClickedButtonClrdtc)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DTC, &CUdsDiagDlg::OnNMClickListDtc)
	ON_BN_CLICKED(IDC_BUTTON_FTTOC, &CUdsDiagDlg::OnBnClickedButtonFttoc)
END_MESSAGE_MAP()


// CUdsDiagDlg 消息处理程序


BOOL CUdsDiagDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1, 200, NULL);
	SetTim = FALSE;
	SetCnt = 0;
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_List.InsertColumn(0, _T("序号"));
	m_List.SetColumnWidth(0, 40);
	m_List.InsertColumn(1, _T("故障名"));
	m_List.SetColumnWidth(1, 100);
	m_List.InsertColumn(2, _T("显示码"));
	m_List.SetColumnWidth(2, 60);
	m_List.InsertColumn(3, _T("DTC"));
	m_List.SetColumnWidth(3, 65);
	m_List.InsertColumn(4, _T("子故障类型"));
	m_List.SetColumnWidth(4, 100);
	m_List.InsertColumn(5, _T("故障状态"));
	m_List.SetColumnWidth(5, 70);
	m_List.InsertColumn(6, _T("状态位"));
	m_List.SetColumnWidth(6, 50);

	UINT dtcst_n;
	UINT nowItem;

	//List Dtc state
	m_ListSt.SetExtendedStyle(LVS_EX_GRIDLINES);

	m_ListSt.InsertColumn(0, _T("状态位"));
	m_ListSt.SetColumnWidth(0, 200);
	m_ListSt.InsertColumn(1, _T("位值"));
	m_ListSt.SetColumnWidth(1, 40);

	for (dtcst_n = 0; dtcst_n < DTCST_NUM; dtcst_n++)
	{
		nowItem = m_ListSt.InsertItem(dtcst_n, DtcStBit[dtcst_n]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUdsDiagDlg::OnBnClickedButtonSptdtc()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 1;

	//ReportSupportedDTC
	CmdBuf[0] = 0x0A;

	theApp.UdsClient.request(SID_19, CmdBuf, CmdLen);

	SetTim = TRUE;
	SetCnt = 2;

	m_List.DeleteAllItems();
	UpdateData(FALSE);//更新数据
}

void CUdsDiagDlg::OnBnClickedButtonFttoc()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 2;

	//ReportDTCByStatusMask
	CmdBuf[0] = 0x02;
	CmdBuf[1] = 0x02; //Test failed this operation cycle

	theApp.UdsClient.request(SID_19, CmdBuf, CmdLen);

	SetTim = TRUE;
	SetCnt = 2;

	m_List.DeleteAllItems();
	UpdateData(FALSE);//更新数据
}

void CUdsDiagDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BYTE DataBuf[BUF_LEN];
	UINT readlen;
	UINT remnpos;

	BYTE AvMask;
	UINT DtcCode;
	BYTE DtcSt;

	UINT dtc_n;
	UINT nowItem;
	BOOL find_dtc;
	CString str;

	if (SetCnt != 0)
	{
		SetCnt--;
		goto TimerEnd;
	}
	if (SetTim == TRUE)
	{
		SetTim = FALSE;
		readlen = theApp.UdsClient.get_rsp(DataBuf, BUF_LEN);
		
		AvMask = DataBuf[3];
		remnpos = 3;

		while (remnpos <= readlen)
		{
			DtcCode = 0;
			DtcCode |= (((UINT)DataBuf[remnpos]) << 16);
			DtcCode |= (((UINT)DataBuf[remnpos+1]) << 8);
			DtcCode |= (((UINT)DataBuf[remnpos+2]) << 0);

			DtcSt = DataBuf[remnpos+3];
			remnpos += 4;

			for (dtc_n = 0; dtc_n < OBD_DTC_CNT; dtc_n++)
			{
				if (obd_dtc_list[dtc_n].DtcCode == DtcCode)
				{
					str.Format(_T("%d"), dtc_n);
					nowItem = m_List.InsertItem(dtc_n, str);

					m_List.SetItemText(nowItem, 1, obd_dtc_list[dtc_n].Name);	    //Dtc Name
					m_List.SetItemText(nowItem, 2, obd_dtc_list[dtc_n].DispCode);	//Display code

					str.Format(_T("0X%06X"), obd_dtc_list[dtc_n].DtcCode);
					m_List.SetItemText(nowItem, 3, str);	                        //Dtc

					str.Format(_T("0X%02X"), DtcSt);
					m_List.SetItemText(nowItem, 5, str);             //Dtc status

					obd_dtc_list[dtc_n].DtcSt = DtcSt;
					obd_dtc_list[dtc_n].ListRow = nowItem;

					break;
				}
			}

		}

	}

TimerEnd:
	CDialogEx::OnTimer(nIDEvent);
}



void CUdsDiagDlg::OnBnClickedButtonClrdtc()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE CmdBuf[BUF_LEN];
	UINT CmdLen = 3;

	//All Group
	CmdBuf[0] = 0xFF;
	CmdBuf[1] = 0xFF;
	CmdBuf[2] = 0xFF;

	theApp.UdsClient.request(SID_14, CmdBuf, CmdLen);
}


void CUdsDiagDlg::OnNMClickListDtc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	INT mItem, mSubItem;

	//get the row number
	mItem = pNMItemActivate->iItem;
	//get the column number
	mSubItem = pNMItemActivate->iSubItem;

	if (mItem == -1) return;


	UINT dtc_n;
	BYTE DtcSt;
	UINT dtcst_n;

	for (dtc_n = 0; dtc_n < OBD_DTC_CNT; dtc_n++)
	{
		if (obd_dtc_list[dtc_n].ListRow == mItem)
		{
			DtcSt = obd_dtc_list[dtc_n].DtcSt;

			for (dtcst_n = 0; dtcst_n < DTCST_NUM; dtcst_n++)
			{
				if (DtcSt & (0x01 << dtcst_n))
				{
					m_ListSt.SetItemText(dtcst_n, 1, _T("1"));	    //DtcSt Bit
				}
				else
				{
					m_ListSt.SetItemText(dtcst_n, 1, _T("0"));	    //DtcSt Bit
				}
			}
			break;
		}
	}

	*pResult = 0;
}
