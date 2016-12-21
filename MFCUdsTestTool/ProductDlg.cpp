// ProductDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "ProductDlg.h"
#include "afxdialogex.h"

#include "UdsUtil.h"

// CProductDlg 对话框

IMPLEMENT_DYNAMIC(CProductDlg, CDialogEx)

CProductDlg::CProductDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PRODUCT, pParent)
{

}

CProductDlg::~CProductDlg()
{
}

void CProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SPIN_SNSUFFIX, m_SnSpin);
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_MonthCal);
	DDX_Control(pDX, IDC_COMBO_ICUTYPE, m_CombICU);

	DDX_Text(pDX, IDC_EDIT_DATE, m_EditDate);
	DDX_Text(pDX, IDC_EDIT_SNSUFFIX, m_EditSnSuffix);
}


BEGIN_MESSAGE_MAP(CProductDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETDATE, &CProductDlg::OnBnClickedButtonSetdate)
	ON_BN_CLICKED(IDC_BUTTON_WRSN, &CProductDlg::OnBnClickedButtonWrsn)
	ON_BN_CLICKED(IDC_BUTTON_WRMANUDATE, &CProductDlg::OnBnClickedButtonWrmanudate)
	ON_BN_CLICKED(IDC_BUTTON_RDSN, &CProductDlg::OnBnClickedButtonRdsn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProductDlg 消息处理程序


BOOL CProductDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SnSpin.SetRange(0, 9999);
	m_SnSpin.SetBuddy(GetDlgItem(IDC_EDIT_SNSUFFIX));

	CEdit*pEdt = (CEdit*)GetDlgItem(IDC_EDIT_SNSUFFIX);
	pEdt->SetLimitText(4);

	m_CombICU.SetCurSel(0);//预置M12

	SetTimer(1, TIMOUT_MS, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProductDlg::OnBnClickedButtonSetdate()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime CtrlTime;

	m_MonthCal.GetCurSel(CtrlTime);

	m_EditDate.Format(_T("%d%d%d"), CtrlTime.GetYear(), CtrlTime.GetMonth(),CtrlTime.GetDay());

	UpdateData(FALSE);//更新数据
}


void CProductDlg::OnBnClickedButtonWrsn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);//更新控件数据

	UdsCmd CmdNew;
	//Push Session request cmd
	CmdNew.SID = SID_10;
	CmdNew.CmdBuf[0] = 0x03;
	CmdNew.CmdLen = 1;
	m_CmdList.Add(CmdNew);

	//Push request seed cmd
	CmdNew.SID = SID_27;
	CmdNew.CmdBuf[0] = 0x01;
	CmdNew.CmdLen = 1;
	m_CmdList.Add(CmdNew);

	//Push send key cmd
	CmdNew.SID = SID_27;
	CmdNew.CmdBuf[0] = 0x02;
	CmdNew.CmdLen = 5;
	m_CmdList.Add(CmdNew);

	//Write data cmd
	CmdNew.SID = SID_2E;
	CmdNew.CmdBuf[0] = 0xF1;
	CmdNew.CmdBuf[1] = 0x8C;
	CmdNew.CmdLen = 2;


	//Get sn from Edit Control
	BYTE hex_str[32];
	BYTE hex_buf[50];

	BYTE temp_char;
	BYTE temp_buf[50];
	LONG temp_len;

	INT newflag = 1, i;

	temp_len = UdsUtil::str2char(m_EditDate, temp_buf) - 1;
	newflag = 1;
	for (i = 0; i<temp_len; i++)
	{
		temp_char = temp_buf[i];
		if (temp_char == ' ')
		{
			newflag = 1;
		}
		else
		{
			if (newflag == 1)
			{
				newflag = 0;
				hex_str[0] = temp_char;
				hex_str[1] = 0;
				hex_str[2] = 0;
			}
			else
			{
				newflag = 1;
				hex_str[1] = temp_char;
				hex_str[2] = 0;
			}

			if (newflag == 1 || temp_buf[i + 1] == ' ')
			{
				UdsUtil::str2HEX(hex_str, hex_buf);
				CmdNew.CmdBuf[CmdNew.CmdLen++] = hex_buf[0];
				if (CmdNew.CmdLen >= 6)
					break;
			}
		}
	}
	CmdNew.CmdBuf[6] = 0xA0;
	if (m_CombICU.GetCurSel() == 0)
		CmdNew.CmdBuf[7] = 0x01;
	else
		CmdNew.CmdBuf[7] = 0x02;

	CmdNew.CmdBuf[8] = 0x00;
	CmdNew.CmdBuf[9] = 0x00;
	CmdNew.CmdLen = 10;

	temp_len = UdsUtil::str2char(m_EditSnSuffix, temp_buf) - 1;
	newflag = 1;
	for (i = 0; i<temp_len; i++)
	{
		temp_char = temp_buf[i];
		if (temp_char == ' ')
		{
			newflag = 1;
		}
		else
		{
			if (newflag == 1)
			{
				newflag = 0;
				hex_str[0] = temp_char;
				hex_str[1] = 0;
				hex_str[2] = 0;
			}
			else
			{
				newflag = 1;
				hex_str[1] = temp_char;
				hex_str[2] = 0;
			}

			if (newflag == 1 || temp_buf[i + 1] == ' ')
			{
				UdsUtil::str2HEX(hex_str, hex_buf);
				CmdNew.CmdBuf[CmdNew.CmdLen++] = hex_buf[0];
				if (CmdNew.CmdLen >= 12)
					break;
			}
		}
	}
	m_CmdList.Add(CmdNew);

}


void CProductDlg::OnBnClickedButtonWrmanudate()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CProductDlg::OnBnClickedButtonRdsn()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CProductDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
	//CDialogEx::OnOK();
}


void CProductDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
	//CDialogEx::OnCancel();
}

void CProductDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	BYTE DataBuf[BUF_LEN];
	UINT readlen;
	UINT remnpos;

	WORD curr_did;
	UINT listrow = 0;
	UINT did_n;

	CString str;
	CString str1;

	if (m_GetRsp == TRUE)
	{
		readlen = theApp.UdsClient.get_rsp(DataBuf, BUF_LEN);

		if (readlen > 0)
		{
			m_GetRsp = FALSE;

			switch (m_CmdNow.SID)
			{
				case SID_27:
				{
					if (readlen == 6 && DataBuf[1] == 0x01)
					{
						m_RspBuf[0] = DataBuf[2];
						m_RspBuf[1] = DataBuf[3];
						m_RspBuf[2] = DataBuf[4];
						m_RspBuf[3] = DataBuf[5];
					}
				}
				break;
				case SID_10:
				case SID_2E:
				default:
					break;
			}

		}
		else
		{
			m_GetRspCnt++;
			if (m_GetRspCnt >= GETRSP_CNT)
			{
				m_GetRsp = FALSE;
			}
		}
	}
	else
	{
		INT_PTR CmdSize = m_CmdList.GetSize();
		if (CmdSize <= 0) return;
		m_CmdNow = m_CmdList[0];
		m_CmdList.RemoveAt(0, 1);
		if (m_CmdNow.SID == SID_27 && m_CmdNow.CmdBuf[0] == 0x02)
		{
			UdsUtil::KeyCalcu(m_RspBuf, &m_CmdNow.CmdBuf[1]);
		}

		theApp.UdsClient.request(m_CmdNow.SID, m_CmdNow.CmdBuf, m_CmdNow.CmdLen);
		m_GetRsp = TRUE;
		m_GetRspCnt = 0;
	}

	CDialogEx::OnTimer(nIDEvent);
}
