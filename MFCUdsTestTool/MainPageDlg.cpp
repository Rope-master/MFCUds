// CanCommDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "MainPageDlg.h"
#include "afxdialogex.h"

#include "EditMsgDlg.h"
#include "UdsUtil.h"
#include "MFCUdsTestToolDlg.h"

INT nextrow = 0;
// CCanCommDlg �Ի���

IMPLEMENT_DYNAMIC(CMainPageDlg, CDialogEx)

CMainPageDlg::CMainPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{
	m_RcevEnable = FALSE;

	m_XmitRow = 0;
	m_SameId = FALSE;
	m_XmitEnable = TRUE;

	m_XmitModif = FALSE;
}

CMainPageDlg::~CMainPageDlg()
{
}

void CMainPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_XMIT, m_XmitList);
	DDX_Check(pDX, IDC_CHECK_XMIT, m_XmitEnable);
	DDX_Check(pDX, IDC_CHECK_SAMEID, m_SameId);

	DDX_Control(pDX, IDC_LIST_CANMSG, m_RecvList);
	DDX_Check(pDX, IDC_CHECK_RECV, m_RcevEnable);


	DDX_Text(pDX, IDC_EDIT_PRINT, m_Editprintstr);
	DDX_Control(pDX, IDC_EDIT_PRINT, m_Editprint);
}


BEGIN_MESSAGE_MAP(CMainPageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_RECV, &CMainPageDlg::OnBnClickedCheckRecv)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMainPageDlg::OnBnClickedButtonClear)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_XMIT, &CMainPageDlg::OnNMRClickListXmit)
	ON_COMMAND(ID_MENU_ADDMSG, &CMainPageDlg::OnMenuAddmsg)
	ON_COMMAND(ID_MENU_EDITMSG, &CMainPageDlg::OnMenuEditmsg)
	ON_COMMAND(ID_MENU_DELMSG, &CMainPageDlg::OnMenuDelmsg)
	ON_COMMAND(ID_MENU_CLRMSG, &CMainPageDlg::OnMenuClrmsg)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_XMIT, &CMainPageDlg::OnNMDblclkListXmit)
	ON_BN_CLICKED(IDC_CHECK_SAMEID, &CMainPageDlg::OnBnClickedCheckSameid)
	ON_BN_CLICKED(IDC_CHECK_XMIT, &CMainPageDlg::OnBnClickedCheckXmit)
END_MESSAGE_MAP()


// CCanCommDlg ��Ϣ��������

BOOL CMainPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ����Ӷ����ĳ�ʼ��
	//Transmit Lst
	m_XmitList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	m_XmitList.InsertColumn(0, _T("Cycle Time"));
	m_XmitList.SetColumnWidth(0, 75);
	m_XmitList.InsertColumn(1, _T("MSG ID"));
	m_XmitList.SetColumnWidth(1, 55);
	m_XmitList.InsertColumn(2, _T("DLC"));
	m_XmitList.SetColumnWidth(2, 40);
	m_XmitList.InsertColumn(3, _T("Data"));
	m_XmitList.SetColumnWidth(3, 175);
	m_XmitList.InsertColumn(4, _T("Count"));
	m_XmitList.SetColumnWidth(4, 55);
	m_XmitList.InsertColumn(5, _T("Num"));
	m_XmitList.SetColumnWidth(5, 55);
    
	//Recieve Lst
 	m_RecvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_RecvList.InsertColumn(0, _T("Seq"));
	m_RecvList.SetColumnWidth(0, 40);
	m_RecvList.InsertColumn(1, _T("TxRx"));
	m_RecvList.SetColumnWidth(1, 40);
	m_RecvList.InsertColumn(2, _T("MSG ID"));
	m_RecvList.SetColumnWidth(2, 55);
	m_RecvList.InsertColumn(3, _T("DLC"));
	m_RecvList.SetColumnWidth(3, 40);
	m_RecvList.InsertColumn(4, _T("Data"));
	m_RecvList.SetColumnWidth(4, 150);

#if 0
	m_SendMsg.Create(IDD_DIALOG_SENDMSG, this);
	m_SendMsg.ShowWindow(SW_HIDE);
	m_Product.Create(IDD_DIALOG_PRODUCT, this);
	m_Product.ShowWindow(SW_SHOW);
#endif
	AfxBeginThread(XmitThread, this);

#ifdef USE_TIMER
	SetTimer(1, TIMOUT_TIM, NULL);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CMainPageDlg::OnBnClickedCheckRecv()
{
	// TODO: �ڴ����ӿؼ�֪ͨ������������
	UpdateData(TRUE);
	/*
	if (m_RcevEnable)
	{
	StopRecv = 0;
	//���������߳�
	AfxBeginThread(ReceiveThread, 0);
	}
	else
	StopRecv = 1;
	*/
}


void CMainPageDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ����ӿؼ�֪ͨ������������
	m_CriticalSection.Lock();
	m_Editprintstr.Empty();
	m_CriticalSection.Unlock();

	m_RecvList.DeleteAllItems();
	UpdateData(FALSE);//��������
}

// Insert a Can msg to list
void CMainPageDlg::InsertItem(INT Dire, VCI_CAN_OBJ * pCanObj)
{

	UINT JustnowItem;

	if ((theApp.m_FilterEn == TRUE) && (pCanObj->ID < theApp.m_Bgnid || pCanObj->ID > theApp.m_Endid)) return;

	//������Ϣ�б���ʾ
	CString str;
	CString str1;

	if (nextrow >= 59999)
	{
		m_RecvList.DeleteAllItems();
		nextrow = 0;
	}

	str.Format(_T("%d"), nextrow);

	if (Dire == 1)
	    m_RecvList.ItemColorFlag[nextrow] = 1;
	else
		m_RecvList.ItemColorFlag[nextrow] = 0;

	JustnowItem = m_RecvList.InsertItem(nextrow, str);
	nextrow++;

	if (Dire == 1)
	    m_RecvList.SetItemText(JustnowItem, 1, _T("Send"));
	else
		m_RecvList.SetItemText(JustnowItem, 1, _T("Recv"));


	if ((pCanObj->ExternFlag) == 1)
	{
		str1.Format(_T("%08X"), pCanObj->ID);
	}
	else
	{
		str1.Format(_T("%04X"), pCanObj->ID);
	}

	m_RecvList.SetItemText(JustnowItem, 2, str1);	//ID��Ϣ

	str = _T("");
	str.Format(_T("%d"), pCanObj->DataLen);
	m_RecvList.SetItemText(JustnowItem, 3, str);

	INT i;
	str = _T("");
	for (i = 0; i<pCanObj->DataLen; i++)
	{
		str1.Format(_T("%02X"), pCanObj->Data[i]);
		str = (str + str1 + _T(" "));
	}
	m_RecvList.SetItemText(JustnowItem, 4, str);

#ifdef DISP_TIME
	CString strTime;
	SYSTEMTIME   systime;
	GetLocalTime(&systime);
	strTime.Format(_T("%02d:%02d:%02d:%03d"), systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	m_RecvList.SetItemText(JustnowItem, 5, strTime);
#endif

	CSize size;
	size.cx = 0;
	size.cy = 50;
	m_RecvList.Scroll(size);
	//������Ϣ�б���ʾ����
}


void CMainPageDlg::PrintLog(UINT LogColor, CString LogContent)
{
	m_CriticalSection.Lock();
	m_Editprintstr += LogContent;
	m_Editprintstr += _T("\r\n");
	UpdateData(FALSE);//��������

	INT Index = m_Editprint.GetLineCount();
	m_Editprint.LineScroll(Index, 0);

	if (Index > 10000)
	{
		m_Editprintstr.Empty();
	}
	m_CriticalSection.Unlock();
}

#if 0
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
#endif

void CMainPageDlg::OnNMRClickListXmit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ������������
	CRect rect;
	GetWindowRect(&rect);
	CPoint p;
	GetCursorPos(&p);//����������Ļ����  
	if (rect.PtInRect(p))
	{
		CMenu RMenu;
		RMenu.LoadMenu(IDR_MENU_XMIT);//�༭�õĲ˵���Դ  
		CMenu *m_SubMenu = RMenu.GetSubMenu(0);

		m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
	}

	*pResult = 0;
}


void CMainPageDlg::OnMenuAddmsg()
{
	// TODO: �ڴ������������������
	XmitMsgAdd();
}


void CMainPageDlg::OnMenuEditmsg()
{
	// TODO: �ڴ������������������
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


void CMainPageDlg::OnMenuDelmsg()
{
	// TODO: �ڴ������������������
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
	UpdateData(FALSE);//��������
}


void CMainPageDlg::OnMenuClrmsg()
{
	// TODO: �ڴ������������������
	m_XmitModif = TRUE;
	m_XmitList.DeleteAllItems();
	m_XmitArray.RemoveAll();
	UpdateData(FALSE);//��������
}


void CMainPageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ��������������/������Ĭ��ֵ
	m_CriticalSection.Lock();
	if (m_XmitArray.GetSize() != m_XmitList.GetItemCount())
	{
		m_XmitList.DeleteAllItems();
		m_XmitArray.RemoveAll();
	}
	m_CriticalSection.Unlock();

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CMainPageDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ�����ר�ô�����/�����û���
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

				//���ö�̬���ӿⷢ�ͺ���
				CMFCUdsTestToolDlg::TransmitCanmsg(SendObj);

				//Refresh the RecvArray
				m_XmitArray.SetAt(nIndex, CanMsg);
			}

			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainPageDlg::XmitMsgAdd()
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
CanMsg_t CMainPageDlg::MsgConvert(EditMsg_t EditMsg)
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

UINT CMainPageDlg::XmitThread(LPVOID v)
{
	CMainPageDlg *dlg = (CMainPageDlg *)v;
	VCI_CAN_OBJ SendObj[1];
	CString str, str1;
	INT nItem;
	CanMsg_t CanMsg;

	while (1)
	{
		//����������Ϣ
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

						//���ö�̬���ӿⷢ�ͺ���
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

void CMainPageDlg::OnNMDblclkListXmit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ������������

	if (pNMItemActivate->iItem == -1)
	{
		XmitMsgAdd();
		return;
	}

	INT nItem, nSubItem;
	//get the row number
	nItem = pNMItemActivate->iItem;
	//get the column number
	nSubItem = pNMItemActivate->iSubItem;

	EditMsg_t EditMsg;
	CString str;
	CEditMsgDlg Dlg;

	EditMsg.Cycle = m_XmitList.GetItemText(nItem, 0);
	EditMsg.Data = m_XmitList.GetItemText(nItem, 3);
	str = m_XmitList.GetItemText(nItem, 2);
	EditMsg.Dlc = 8;
	EditMsg.Id = m_XmitList.GetItemText(nItem, 1);
	EditMsg.FrameNum = m_XmitList.GetItemText(nItem, 5);

	Dlg.pEditMsg = &EditMsg;
	Dlg.DoModal();

	CanMsg_t CanMsg;
	CanMsg = MsgConvert(EditMsg);
	m_XmitArray.SetAt(nItem, CanMsg);

	m_XmitList.SetItemText(nItem, 0, EditMsg.Cycle);	//Cycle Time
	m_XmitList.SetItemText(nItem, 1, EditMsg.Id);	    //Can id
	str.Format(_T("%d"), EditMsg.Dlc);
	m_XmitList.SetItemText(nItem, 2, str);	            //Can Data Length
	m_XmitList.SetItemText(nItem, 3, EditMsg.Data);	    //Can Data
	m_XmitList.SetItemText(nItem, 5, EditMsg.FrameNum);	//Frame Num

	*pResult = 0;
}


void CMainPageDlg::OnBnClickedCheckSameid()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CMainPageDlg::OnBnClickedCheckXmit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}
