#pragma once
#include "ColoredListCtrl.h"
#include "MainPageDlg.h"
// CIoCtrlDlg 对话框

class CIoCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIoCtrlDlg)

public:
	CIoCtrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIoCtrlDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IOCTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CColoredListCtrl m_List;
	CComboBox m_Combo;

public:
	CMainPageDlg *p_CanComm;
	INT nItem, nSubItem;

	CArray<UdsCmd> m_CmdList;
	UdsCmd m_CmdNow;
	BOOL m_GetRsp;
	BYTE m_GetRspCnt;
	BYTE m_RspBuf[4];

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListIoctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListIoctrl(NMHDR *pNMHDR, LRESULT *pResult);

	// Generate a combo in a List item
	CComboBox * ComboItem(HWND hWndList, INT nItem, INT nSubItem);
	// this function will returns the item text depending on the item and SubItem Index
	CString GetItemText(HWND hWnd, int nItem, int nSubItem) const;
	afx_msg void OnCbnSelchangeComboIoctrl();
	afx_msg void OnBnClickedButtonIoctrl();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonInc();
	afx_msg void OnBnClickedButtonDec();
	afx_msg void OnBnClickedButtonIoctrlRt();
};


/* uds io control type */
typedef struct __UDS_IOCTRL_T__
{
	CString name;
	WORD did;
	BYTE *p_data;
	BYTE dlc;
	BYTE default_value;
	BYTE step;
	BOOL  enable;
	CString *p_opt;
	BYTE opt_num;
	BYTE opt_offset;
	CString *p_value;
	BYTE value_num;
}uds_ioctrl_t;

#define IOCTRL_CNT  5