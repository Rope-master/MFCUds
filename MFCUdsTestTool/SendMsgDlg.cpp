// SendMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "SendMsgDlg.h"
#include "afxdialogex.h"

#include "ControlCAN.h"
#include "UdsUtil.h"
#include "MFCUdsTestToolDlg.h"
#include "EditMsgDlg.h"
// CSendMsgDlg 对话框

IMPLEMENT_DYNAMIC(CSendMsgDlg, CDialogEx)

CSendMsgDlg::CSendMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SENDMSG, pParent)
{

}

CSendMsgDlg::~CSendMsgDlg()
{
}

void CSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CSendMsgDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSendMsgDlg 消息处理程序


BOOL CSendMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
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

#if 0
void CSendMsgDlg::OnNMRClickListXmit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CRect rect;
	GetWindowRect(&rect);
	CPoint p;
	GetCursorPos(&p);//鼠标点的屏幕坐标  
	if (rect.PtInRect(p))
	{
		CMenu RMenu;
		RMenu.LoadMenu(IDR_MENU_XMIT);//编辑好的菜单资源  
		CMenu *m_SubMenu = RMenu.GetSubMenu(0);

		m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
	}
	*pResult = 0;
}


void CSendMsgDlg::OnMenuAddmsg()
{
	// TODO: 在此添加命令处理程序代码
	XmitMsgAdd();
}


void CSendMsgDlg::XmitMsgAdd()
{
	EditMsg_t EditMsg;
	CEditMsgDlg Dlg;
	EditMsg.Dlc = 8;
	EditMsg.Cycle = _T("100");
	EditMsg.Data = _T("00 00 00 00 00 00 00 00");
	EditMsg.FrameNum = _T("0");
	Dlg.pEditMsg = &EditMsg;
	Dlg.DoModal();

	CanMsg_t CanMsg;
	CanMsg = MsgConvert(EditMsg);

	CanMsg_t nCanMsg;
	BOOL IdExist;
	INT_PTR nItem;
	UINT nowItem;
	CString str;
	INT_PTR ListSize = m_XmitArray.GetSize();
	IdExist = FALSE;
	for (nItem = 0; nItem < ListSize; nItem++)
	{
		nCanMsg = m_XmitArray.GetAt(nItem);
		if (CanMsg.Id == nCanMsg.Id)
		{
			IdExist = TRUE;
			break;
		}
	}

	m_CriticalSection.Lock();

	if (IdExist == FALSE || m_SameId == TRUE)
	{
		m_XmitModif = TRUE;
		nowItem = m_XmitList.InsertItem(m_XmitRow, _T(""));
		m_XmitRow++;
		m_XmitArray.Add(CanMsg);
	}
	else
	{
		nowItem = nItem;
		m_XmitArray.SetAt(nItem, CanMsg);
	}

	m_XmitList.SetItemText(nowItem, 0, EditMsg.Cycle);	//Cycle Time
	m_XmitList.SetItemText(nowItem, 1, EditMsg.Id);	    //Can id
	str.Format(_T("%d"), EditMsg.Dlc);
	m_XmitList.SetItemText(nowItem, 2, str);	        //Can Data Length
	m_XmitList.SetItemText(nowItem, 3, EditMsg.Data);	    //Can Data
	m_XmitList.SetItemText(nowItem, 5, EditMsg.FrameNum);	////Frame Num

	m_CriticalSection.Unlock();
}

// Convert a EditMsg to CanMsg
CanMsg_t CSendMsgDlg::MsgConvert(EditMsg_t EditMsg)
{
	CanMsg_t CanMsg;

	BYTE temp_buf[50];
	LONG temp_len;
	BYTE HexBuf[10] = { 0 };
	BYTE DecBuf[10] = { 0 };

	temp_len = UdsUtil::str2char(EditMsg.Id, temp_buf) - 1;
	UdsUtil::str2HEX(temp_buf, HexBuf);
	CanMsg.Id = (UINT)HexBuf[0] << 8;
	CanMsg.Id |= (UINT)HexBuf[1] << 0;

	temp_len = UdsUtil::str2char(EditMsg.Cycle, temp_buf) - 1;
	temp_len = UdsUtil::str2DEC(temp_buf, DecBuf);
	CanMsg.Cycle = 0;
	for (int i = 0; i < temp_len; i++)
	{
		CanMsg.Cycle = CanMsg.Cycle * 10 + DecBuf[i];
	}

	temp_len = UdsUtil::str2char(EditMsg.FrameNum, temp_buf) - 1;
	temp_len = UdsUtil::str2DEC(temp_buf, DecBuf);
	CanMsg.FrameNum = 0;
	for (int i = 0; i < temp_len; i++)
	{
		CanMsg.FrameNum = CanMsg.FrameNum * 10 + DecBuf[i];
	}

	CanMsg.Dlc = EditMsg.Dlc;
	if (CanMsg.Dlc >= MSG_LEN)
		CanMsg.Dlc = MSG_LEN;

	CanMsg.Count = 0;

	BYTE ttmp_buf[32];
	BYTE TmpChar;
	INT newflag = 1;
	INT DataNum = 0;
	temp_len = UdsUtil::str2char(EditMsg.Data, temp_buf) - 1;
	for (INT i = 0; i<temp_len; i++)
	{
		TmpChar = temp_buf[i];
		if (TmpChar == ' ')
		{
			newflag = 1;
		}
		else
		{
			if (newflag == 1)
			{
				newflag = 0;
				ttmp_buf[0] = TmpChar;
				ttmp_buf[1] = 0;
				ttmp_buf[2] = 0;
			}
			else
			{
				newflag = 1;
				ttmp_buf[1] = TmpChar;
				ttmp_buf[2] = 0;
			}

			if (newflag == 1 || temp_buf[i + 1] == ' ')
			{
				UdsUtil::str2HEX(ttmp_buf, HexBuf);
				CanMsg.Data[DataNum++] = HexBuf[0];
				if (DataNum >= CanMsg.Dlc)
					break;
			}
		}
	}

	return CanMsg;
}

void CSendMsgDlg::OnMenuEditmsg()
{
	// TODO: 在此添加命令处理程序代码
	POSITION ps;
	INT nIndex;

	ps = m_XmitList.GetFirstSelectedItemPosition();
	nIndex = m_XmitList.GetNextSelectedItem(ps);

	if (nIndex == -1) return;

	EditMsg_t EditMsg;
	CString str;
	CEditMsgDlg Dlg;

	EditMsg.Cycle = m_XmitList.GetItemText(nIndex, 0);
	EditMsg.Data = m_XmitList.GetItemText(nIndex, 3);
	str = m_XmitList.GetItemText(nIndex, 2);
	EditMsg.Dlc = 8;
	EditMsg.Id = m_XmitList.GetItemText(nIndex, 1);
	EditMsg.FrameNum = m_XmitList.GetItemText(nIndex, 5);

	Dlg.pEditMsg = &EditMsg;
	Dlg.DoModal();

	CanMsg_t CanMsg;
	CanMsg = MsgConvert(EditMsg);
	m_XmitArray.SetAt(nIndex, CanMsg);

	m_XmitList.SetItemText(nIndex, 0, EditMsg.Cycle);	//Cycle Time
	m_XmitList.SetItemText(nIndex, 1, EditMsg.Id);	    //Can id
	str.Format(_T("%d"), EditMsg.Dlc);
	m_XmitList.SetItemText(nIndex, 2, str);	            //Can Data Length
	m_XmitList.SetItemText(nIndex, 3, EditMsg.Data);	    //Can Data
	m_XmitList.SetItemText(nIndex, 5, EditMsg.FrameNum);	//Frame Num
}


void CSendMsgDlg::OnMenuDelmsg()
{
	// TODO: 在此添加命令处理程序代码
	POSITION ps;
	INT nIndex;

	ps = m_XmitList.GetFirstSelectedItemPosition();
	nIndex = m_XmitList.GetNextSelectedItem(ps);

	if (nIndex == -1) return;

	INT ListSize = m_XmitList.GetItemCount();
	for (INT i = nIndex; i < ListSize; i++)
		m_XmitList.ItemColorFlag[i] = m_XmitList.ItemColorFlag[i + 1];

	m_XmitModif = TRUE;
	m_XmitList.DeleteItem(nIndex);
	m_XmitArray.RemoveAt(nIndex);
	UpdateData(FALSE);//更新数据
}


void CSendMsgDlg::OnMenuClrmsg()
{
	// TODO: 在此添加命令处理程序代码
	m_XmitModif = TRUE;
	m_XmitList.DeleteAllItems();
	m_XmitArray.RemoveAll();
	UpdateData(FALSE);//更新数据
}


BOOL CSendMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_SPACE)
		{
			CanMsg_t CanMsg;
			VCI_CAN_OBJ SendObj[1];

			POSITION ps;
			INT nIndex;
			INT nSubItem = 3;
			ps = m_XmitList.GetFirstSelectedItemPosition();
			nIndex = m_XmitList.GetNextSelectedItem(ps);

			if (nIndex == -1) return TRUE;

			if (!m_XmitList.GetCheck(nIndex))
			{
				CanMsg = m_XmitArray.GetAt(nIndex);
				CanMsg.Count++;

				int FrameFormat, FrameType;
				FrameFormat = FRMFMT_STD;
				FrameType = FRMTYP_DAT;

				SendObj->ExternFlag = FrameType;
				SendObj->DataLen = CanMsg.Dlc;
				SendObj->RemoteFlag = FrameFormat;
				SendObj->ID = CanMsg.Id;

				for (INT i = 0; i < CanMsg.Dlc; i++)
					SendObj->Data[i] = CanMsg.Data[i];

				//调用动态链接库发送函数
				CMFCUdsTestToolDlg::TransmitCanmsg(SendObj);

				//Refresh the RecvArray
				m_XmitArray.SetAt(nIndex, CanMsg);
			}

			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSendMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_CriticalSection.Lock();
	if (m_XmitArray.GetSize() != m_XmitList.GetItemCount())
	{
		m_XmitList.DeleteAllItems();
		m_XmitArray.RemoveAll();
	}
	m_CriticalSection.Unlock();

	CDialogEx::OnTimer(nIDEvent);
}

UINT CSendMsgDlg::XmitThread(LPVOID v)
{
	CSendMsgDlg *dlg = (CSendMsgDlg *)v;
	VCI_CAN_OBJ SendObj[1];
	CString str, str1;
	INT nItem;
	CanMsg_t CanMsg;

	while (1)
	{
		//处理发送消息
		if (dlg->m_XmitEnable == TRUE)
		{

			INT ListSize = dlg->m_XmitList.GetItemCount();
			for (nItem = 0; nItem < ListSize; nItem++)
			{
				if (dlg->m_XmitList.GetCheck(nItem))
				{
					if (dlg->m_XmitModif == TRUE)
					{
						dlg->m_XmitModif = FALSE;
						break;
					}
					CanMsg = dlg->m_XmitArray.GetAt(nItem);
					if (CanMsg.Cycle == 0) continue;
					CanMsg.TimeCnt += TIMOUT_XMIT;
					if (CanMsg.TimeCnt >= CanMsg.Cycle)
					{
						CanMsg.TimeCnt = 0;
						CanMsg.Count++;

						int FrameFormat, FrameType;
						FrameFormat = FRMFMT_STD;
						FrameType = FRMTYP_DAT;

						SendObj->SendType = 0;
						SendObj->ExternFlag = FrameType;
						SendObj->DataLen = CanMsg.Dlc;
						SendObj->RemoteFlag = FrameFormat;
						SendObj->ID = CanMsg.Id;

						for (INT i = 0; i < CanMsg.Dlc; i++)
							SendObj->Data[i] = CanMsg.Data[i];

						//调用动态链接库发送函数
						CMFCUdsTestToolDlg::TransmitCanmsg(SendObj);

						str.Format(_T("%d"), CanMsg.Count);
						dlg->m_XmitList.SetItemText(nItem, 4, str);	         //Count
					}

					if (CanMsg.FrameNum > 0 && CanMsg.Count > CanMsg.FrameNum)
					{
						CanMsg.Count = 0;
						dlg->m_XmitList.SetCheck(nItem, FALSE);
					}
					//Refresh the RecvArray
					dlg->m_XmitArray.SetAt(nItem, CanMsg);
				}
			}
		}
		Sleep(TIMOUT_XMIT);

	}
	return 1;
}


void CSendMsgDlg::OnBnClickedCheckXmit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);//更新数据
}


void CSendMsgDlg::OnBnClickedCheckSameid()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);//更新数据
}

#endif