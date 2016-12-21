// SendMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "SendMsgDlg.h"
#include "afxdialogex.h"

#include "ControlCAN.h"
#include "UdsUtil.h"
#include "MFCUdsTestToolDlg.h"
// CSendMsgDlg 对话框

IMPLEMENT_DYNAMIC(CSendMsgDlg, CDialogEx)

CSendMsgDlg::CSendMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SENDMSG, pParent)
{
	m_CanExt = FALSE;
	m_CanRmt = FALSE;
}

CSendMsgDlg::~CSendMsgDlg()
{
}

void CSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CANTX, m_Editcantx);
	DDX_Text(pDX, IDC_EDIT_CANID, m_EditCanid);

	DDX_Control(pDX, IDC_EDIT_CANTX, m_EditData);
}


BEGIN_MESSAGE_MAP(CSendMsgDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TX, &CSendMsgDlg::OnBnClickedButtonTx)
END_MESSAGE_MAP()


// CSendMsgDlg 消息处理程序


BOOL CSendMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditCanid = _T("766");
	GetDlgItem(IDC_EDIT_CANID)->SetWindowText(m_EditCanid);

	m_EditData.SetLimitLen(23);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSendMsgDlg::OnBnClickedButtonTx()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);//更新控件数据

					 //从界面获取发送信息
	VCI_CAN_OBJ SendObj;
	BYTE hex_str[32];
	BYTE hex_buf[50];
	BYTE data_buf[50];
	BYTE can_id[10] = { 0 };

	BYTE temp_char;
	BYTE temp_buf[50];
	LONG temp_len;

	int datanum = 0, IDnum = 0, newflag = 1, i;
	int FrameFormat, FrameType;

	if (m_CanExt == 0)
		FrameFormat = FRMFMT_STD;
	else
		FrameFormat = FRMFMT_EXT;

	if (m_CanRmt == 0)
		FrameType = FRMTYP_DAT;
	else
		FrameType = FRMTYP_RMT;

	temp_len = UdsUtil::str2char(m_EditCanid, temp_buf) - 1;
	UdsUtil::str2HEX(temp_buf, can_id);

	temp_len = UdsUtil::str2char(m_Editcantx, temp_buf) - 1;
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
				data_buf[datanum++] = hex_buf[0];
				if (datanum >= 8)
					break;
			}
		}
	}
	SendObj.ExternFlag = FrameType;
	SendObj.DataLen = datanum;
	SendObj.RemoteFlag = FrameFormat;
	if (FrameFormat == 1)//if remote frame, data area is invalid
		for (i = 0; i<datanum; i++)
			data_buf[i] = 0;

	SendObj.ID = 0;
	SendObj.ID |= (UINT)can_id[0] << 8;
	SendObj.ID |= (UINT)can_id[1] << 0;


	for (i = 0; i<datanum; i++)
		SendObj.Data[i] = data_buf[i];
	/****************************************************************************/
	/******************************从界面获取发送信息完毕***********************/
	/****************************************************************************/

	//调用动态链接库发送函数
	INT flag = CMFCUdsTestToolDlg::TransmitCanmsg(&SendObj);//CAN message send
	if (flag < 1)
	{
		if (flag == -1)
			MessageBox(_T("failed- device not open\n"));
		else if (flag == 0)
			MessageBox(_T("send error\n"));
		return;

	}

}


void CSendMsgDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
	//CDialogEx::OnOK();
}


void CSendMsgDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
	//CDialogEx::OnCancel();
}
