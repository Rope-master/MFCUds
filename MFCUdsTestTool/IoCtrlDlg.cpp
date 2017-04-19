// IoCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "IoCtrlDlg.h"
#include "afxdialogex.h"
#include "UdsUtil.h"

CString backlight_opt[] =
{
	_T("01:Fuel Gauge Dial Backligh Level"),
	_T("02:Coolant Gauge Dial Backlight Level"),
	_T("03:TFT Screen Backlight Level")
};

CString backlight_value[] =
{
	_T("0x00 (0%)"),
	_T("0x7F (50%)"),
	_T("0xFF (100%)")
};

CString buzzer_opt[] =
{
	_T("01:Turn buzzer warning")
};

CString buzzer_value[] =
{
	_T("00:Silence"),
	_T("01:Ring with Certain frequenc Always sounding")
};
CString gages_opt[] =
{
	_T("03:Coolant gauge"),
	_T("04:Fuel gauge")
};
CString gages_value[] =
{
	_T("00:表盘0刻度线位置"),
	_T("01:表盘最大刻度位置")
};

CString segment_opt[] =
{
	_T("01:TFT screen display red in full screen"),
	_T("02:TFT screen display green in full screen"),
	_T("03:TFT screen display blue in full screen"),
	_T("04:TFT screen display black and white square in full screen")
};

CString segment_value[] =
{
	_T("00:OFF"),
	_T("01:ON")
};

CString indicator_opt[] =
{
	_T("00:Turn left lamp"),
	_T("01:High beam lamp"),
	_T("02:Location lamp"),
	_T("03:Front fog lamp"),
	_T("04:Rear fog lamp"),
	_T("05:Turn right lamp"),
	_T("06:PEPS lamp"),
	_T("07:Door open lamp"),
	_T("08:Seat belt lamp"),
	_T("09:Passengers seat belt lamp"),
	_T("10:EPB lamp"),
	_T("11:Fuel warning lamp"),
	_T("12:Brake failed lamp"),
	_T("13:Altemator lamp"),
	_T("14:Oil Pressure lamp"),
	_T("15:Caralarm lamp"),
	_T("16:Reserve")
};

CString indicator_value[] =
{
	_T("00:OFF"),
	_T("01:ON")
};

BYTE backlight_level[2];
BYTE buzzer[2];
BYTE gages[2];
BYTE segment_disp[2];
BYTE indicator[6];

uds_ioctrl_t ioctrl_list[IOCTRL_CNT] =
{
	{ _T("Backlight Level"), 0xF092, backlight_level, 2, 0, 0, 0, backlight_opt, 3, 1, backlight_value, 3},
	{ _T("Buzzer"),          0xF020, buzzer,          2, 0, 0, 0, buzzer_opt,    1, 1, buzzer_value,    2},
	{ _T("Gages"),           0xF021, gages,           2, 0, 0, 0, gages_opt,     2, 3, gages_value,     2},
	{ _T("Segment Display"), 0xF022, segment_disp,    2, 0, 0, 0, segment_opt,   4, 1, segment_value,   2},
	{ _T("Indicators"),      0xF024, indicator,       6, 0, 0, 0, indicator_opt, 17,0, indicator_value, 2}
};


// CIoCtrlDlg 对话框

IMPLEMENT_DYNAMIC(CIoCtrlDlg, CDialogEx)

CIoCtrlDlg::CIoCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_IOCTRL, pParent)
{

}

CIoCtrlDlg::~CIoCtrlDlg()
{
}

void CIoCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_IOCTRL, m_List);
	DDX_Control(pDX, IDC_COMBO_IOCTRL, m_Combo);
}


BEGIN_MESSAGE_MAP(CIoCtrlDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IOCTRL, &CIoCtrlDlg::OnLvnItemchangedListIoctrl)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IOCTRL, &CIoCtrlDlg::OnNMClickListIoctrl)
	ON_CBN_SELCHANGE(IDC_COMBO_IOCTRL, &CIoCtrlDlg::OnCbnSelchangeComboIoctrl)
	ON_BN_CLICKED(IDC_BUTTON_IOCTRL, &CIoCtrlDlg::OnBnClickedButtonIoctrl)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_INC, &CIoCtrlDlg::OnBnClickedButtonInc)
	ON_BN_CLICKED(IDC_BUTTON_DEC, &CIoCtrlDlg::OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_IOCTRL_RT, &CIoCtrlDlg::OnBnClickedButtonIoctrlRt)
END_MESSAGE_MAP()


// CIoCtrlDlg 消息处理程序


BOOL CIoCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//SetTimer(1, TIMOUT_MS, NULL);

	::ShowWindow(::GetDlgItem(m_hWnd, IDC_COMBO_IOCTRL), SW_HIDE);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	m_List.InsertColumn(0, _T("Name"));
	m_List.SetColumnWidth(0, 60);
	m_List.InsertColumn(1, _T(" DID "));
	m_List.SetColumnWidth(1, 50);
	m_List.InsertColumn(2, _T("DLC"));
	m_List.SetColumnWidth(2, 50);
	m_List.InsertColumn(3, _T("Data"));
	m_List.SetColumnWidth(3, 160);
	m_List.InsertColumn(4, _T("Value"));
	m_List.SetColumnWidth(4, 160);


	UINT listrow = 0;
	UINT nowItem;

	CString str;
	CString str1;

	for (listrow = 0; listrow < IOCTRL_CNT; listrow++)
	{
		m_List.ItemTextColor[listrow] = 0;

		m_List.ItemColorFlag[listrow] = 0;

		nowItem = m_List.InsertItem(listrow, ioctrl_list[listrow].name);

		str.Format(_T("0X%04X"), ioctrl_list[listrow].did);

		m_List.SetItemText(nowItem, 1, str);	//DID

		str.Format(_T("%d"), ioctrl_list[listrow].dlc);

		m_List.SetItemText(nowItem, 2, str);	//dlc
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CIoCtrlDlg::OnLvnItemchangedListIoctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (m_List.GetCheck(pNMLV->iItem))
	{
		INT item;
		for (item = 0; item < m_List.GetItemCount(); item++)
		{
			if (item == pNMLV->iItem)
				continue;
			m_List.SetCheck(item, FALSE);
		}
	}
	*pResult = 0;
}


void CIoCtrlDlg::OnNMClickListIoctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	Invalidate();
	HWND hWnd1 = ::GetDlgItem(m_hWnd, IDC_LIST_IOCTRL);
	RECT rect;
	//get the row number
	nItem = pNMItemActivate->iItem;
	//get the column number
	nSubItem = pNMItemActivate->iSubItem;
	if ((nSubItem != 3 && nSubItem != 4) || nItem == -1)
		return;

	//Retrieve the text of the selected subItem 
	//from the list
	CString str = GetItemText(hWnd1, nItem,
		nSubItem);

	RECT rect1, rect2;
	// this macro is used to retrieve the Rectanle 
	// of the selected SubItem
	ListView_GetSubItemRect(hWnd1, nItem,
		nSubItem, LVIR_BOUNDS, &rect);
	//Get the Rectange of the listControl
	::GetWindowRect(pNMItemActivate->hdr.hwndFrom, &rect1);
	//Get the Rectange of the Dialog
	::GetWindowRect(m_hWnd, &rect2);

	int x = rect1.left - rect2.left;
	int y = rect1.top - rect2.top;


	if (nItem != -1)
		::SetWindowPos(::GetDlgItem(m_hWnd, IDC_COMBO_IOCTRL),
			HWND_TOP, rect.left + x - 7, rect.top + 9,
			rect.right - rect.left,
			rect.bottom - rect.top, NULL);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_COMBO_IOCTRL), SW_SHOW);
	::SetFocus(::GetDlgItem(m_hWnd, IDC_COMBO_IOCTRL));
	//Draw a Rectangle around the SubItem
	//::Rectangle(::GetDC(pNMItemActivate->hdr.hwndFrom),
	//rect.left, rect.top - 1, rect.right, rect.bottom);

	m_Combo.ResetContent();

	m_Combo.ModifyStyleEx(0, WS_EX_CLIENTEDGE);//can we tell at all

	if (nSubItem == 3)
	{
		for (int i = 0; i < ioctrl_list[nItem].opt_num; i++)
			m_Combo.AddString(ioctrl_list[nItem].p_opt[i]);
	}
	else
	{
		for (int i = 0; i < ioctrl_list[nItem].value_num; i++)
			m_Combo.AddString(ioctrl_list[nItem].p_value[i]);
	}

	m_Combo.ShowDropDown();

	*pResult = 0;
}


// Generate a combo in a List item
CComboBox * CIoCtrlDlg::ComboItem(HWND hWndList,INT nItem, INT nSubItem)
{
	return nullptr;
}

// this function will returns the item text depending on the item and SubItem Index
CString CIoCtrlDlg::GetItemText(HWND hWnd, int nItem, int nSubItem) const
{
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iSubItem = nSubItem;
	CString str;
	int nLen = 128;
	int nRes;
	do
	{
		nLen *= 2;
		lvi.cchTextMax = nLen;
		lvi.pszText = str.GetBufferSetLength(nLen);
		nRes = (int)::SendMessage(hWnd,
			LVM_GETITEMTEXT, (WPARAM)nItem,
			(LPARAM)&lvi);
	} while (nRes == nLen - 1);
	str.ReleaseBuffer();
	return str;
}

void CIoCtrlDlg::OnCbnSelchangeComboIoctrl()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ComboStr;
	INT nIndex = m_Combo.GetCurSel();
	m_Combo.GetLBText(nIndex, ComboStr);

	m_Combo.ShowWindow(FALSE);

	if (nItem >= 0 && nItem < IOCTRL_CNT)
	{
		if (nItem == 0)
		{
			if (nSubItem == 3)
			{
				m_List.SetItemText(0, 3, ComboStr);
				ioctrl_list[0].p_data[0] = nIndex + ioctrl_list[0].opt_offset;
			}
			else
			{
				m_List.SetItemText(0, 4, ComboStr);
				if (nIndex == 0)
				    ioctrl_list[0].p_data[1] = 0;
				else if (nIndex == 1)
					ioctrl_list[0].p_data[1] = 0x7F;
				else
					ioctrl_list[0].p_data[1] = 0xFF;
			}
		}
		else
		{
			if (nSubItem == 3)
			{
				m_List.SetItemText(nItem, 3, ComboStr);
				ioctrl_list[nItem].p_data[0] = nIndex + ioctrl_list[nItem].opt_offset;
			}
			else
			{
				m_List.SetItemText(nItem, 4, ComboStr);
				ioctrl_list[nItem].p_data[1] = nIndex;
			}
		}
	}
}


void CIoCtrlDlg::OnBnClickedButtonIoctrl()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nItem >= 0 && nItem < IOCTRL_CNT)
	{
		UdsCmd CmdNew;
		UINT listrow = 0;

		//Push Session request cmd
		CmdNew.SID = SID_10;
		CmdNew.CmdBuf[0] = 0x03;
		CmdNew.CmdLen = 1;
		//m_CmdList.Add(CmdNew);
		theApp.UdsClient.push_cmd(CmdNew);


		//Push request seed cmd
		CmdNew.SID = SID_27;
		CmdNew.CmdBuf[0] = 0x01;
		CmdNew.CmdLen = 1;
		//m_CmdList.Add(CmdNew);
		theApp.UdsClient.push_cmd(CmdNew);

		//Push send key cmd
		CmdNew.SID = SID_27;
		CmdNew.CmdBuf[0] = 0x02;
		CmdNew.CmdLen = 5;
		//m_CmdList.Add(CmdNew);
		theApp.UdsClient.push_cmd(CmdNew);

		//Push Io Control cmd
		for (listrow = 0; listrow < IOCTRL_CNT; listrow++)
		{
			CmdNew.SID = SID_2F;
			if (m_List.GetCheck(listrow))
			{
				CmdNew.CmdBuf[0] = (BYTE)(ioctrl_list[listrow].did >> 8);
				CmdNew.CmdBuf[1] = (BYTE)(ioctrl_list[listrow].did >> 0);
				CmdNew.CmdBuf[2] = 0x03;

				CmdNew.CmdBuf[3] = 0;
				if (nItem == (IOCTRL_CNT - 1))
				{
					if (ioctrl_list[nItem].p_data[1] == 0) //OFF
					{
						BYTE LampPos = ioctrl_list[nItem].p_data[0];
						if (LampPos < 8)
						{
							CmdNew.CmdBuf[3] = 0;
							CmdNew.CmdBuf[3] &= (~(0x01 << LampPos));
							CmdNew.CmdBuf[4] = 0;
							CmdNew.CmdBuf[5] = 0;
							CmdNew.CmdBuf[6] = 0;
							CmdNew.CmdBuf[7] = 0;
							CmdNew.CmdBuf[8] = 0;
						}
						else
						{
							LampPos -= 8;
							CmdNew.CmdBuf[3] = 0;
							CmdNew.CmdBuf[4] = 0;
							CmdNew.CmdBuf[4] &= (~(0x01 << LampPos));
							CmdNew.CmdBuf[5] = 0;
							CmdNew.CmdBuf[6] = 0;
							CmdNew.CmdBuf[7] = 0;
							CmdNew.CmdBuf[8] = 0;
						}
					}
					else
					{
						BYTE LampPos = ioctrl_list[nItem].p_data[0];
						if (LampPos < 8)
						{
							CmdNew.CmdBuf[3] = 0;
							CmdNew.CmdBuf[3] |= (0x01 << LampPos);
							CmdNew.CmdBuf[4] = 0;
							CmdNew.CmdBuf[5] = 0;
							CmdNew.CmdBuf[6] = 0;
							CmdNew.CmdBuf[7] = 0;
							CmdNew.CmdBuf[8] = 0;
						}
						else
						{
							LampPos -= 8;
							CmdNew.CmdBuf[3] = 0;
							CmdNew.CmdBuf[4] = 0;
							CmdNew.CmdBuf[4] |= (0x01 << LampPos);
							CmdNew.CmdBuf[5] = 0;
							CmdNew.CmdBuf[6] = 0;
							CmdNew.CmdBuf[7] = 0;
							CmdNew.CmdBuf[8] = 0;
						}
					}

					CmdNew.CmdLen = 9;
				}
				else
				{
					CmdNew.CmdBuf[3] = ioctrl_list[nItem].p_data[0];
					CmdNew.CmdBuf[4] = ioctrl_list[nItem].p_data[1];
					CmdNew.CmdLen = 5;
				}
				//m_CmdList.Add(CmdNew);
				theApp.UdsClient.push_cmd(CmdNew);
				break;
			}
		}

		p_CanComm->PrintLog(0, _T(">>Io Control Short Adjustment"));
	}
}


void CIoCtrlDlg::OnBnClickedButtonIoctrlRt()
{
	// TODO: 在此添加控件通知处理程序代码
	if (nItem >= 0 && nItem < IOCTRL_CNT)
	{
		UdsCmd CmdNew;
		UINT listrow = 0;

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

		//Push Io Control cmd
		for (listrow = 0; listrow < IOCTRL_CNT; listrow++)
		{
			CmdNew.SID = SID_2F;
			if (m_List.GetCheck(listrow))
			{
				CmdNew.CmdBuf[0] = (BYTE)(ioctrl_list[listrow].did >> 8);
				CmdNew.CmdBuf[1] = (BYTE)(ioctrl_list[listrow].did >> 0);
				CmdNew.CmdBuf[2] = 0x00;
				CmdNew.CmdLen = 3;
				m_CmdList.Add(CmdNew);
				break;
			}
		}

		p_CanComm->PrintLog(0, _T(">>Io Control Short Return To Mcu"));
	}
}

void CIoCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BYTE DataBuf[BUF_LEN];
	UINT readlen;

	if (m_GetRsp == TRUE)
	{
		readlen = theApp.UdsClient.get_rsp(DataBuf, BUF_LEN);

		if (readlen > 0)
		{
			m_GetRsp = FALSE;

			switch (m_CmdNow.SID)
			{
			case SID_2F:
				p_CanComm->PrintLog(0, _T("      Done"));
				break;
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
				p_CanComm->PrintLog(0, _T("      Fail"));
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


void CIoCtrlDlg::OnBnClickedButtonInc()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	FLOAT per;
	if (ioctrl_list[0].p_data[1] < (0xFF - 0x19))
	{
		ioctrl_list[0].p_data[1] += 0x19;
	}
	per = ((FLOAT)ioctrl_list[0].p_data[1] * 100 / 0xFF);
	str.Format(_T("%02X (%2.2f%%)"), ioctrl_list[0].p_data[1], per);
	m_List.SetItemText(0, 4, str);

}


void CIoCtrlDlg::OnBnClickedButtonDec()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	FLOAT per;
	if (ioctrl_list[0].p_data[1] > 0x19)
	{
		ioctrl_list[0].p_data[1] -= 0x19;
	}
	per = ((FLOAT)ioctrl_list[0].p_data[1] * 100 / 0xFF);
	str.Format(_T("%02X (%2.0f%%)"), ioctrl_list[0].p_data[1], per);
	m_List.SetItemText(0, 4, str);
}
