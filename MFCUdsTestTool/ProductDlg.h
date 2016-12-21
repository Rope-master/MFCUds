#pragma once


// CProductDlg 对话框

class CProductDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProductDlg)

public:
	CProductDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProductDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRODUCT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_EditDate;
	CString m_EditSnSuffix;
	CComboBox m_CombICU;

	INT m_SnSuffix;
	CSpinButtonCtrl m_SnSpin;
	CMonthCalCtrl m_MonthCal;

	CArray<UdsCmd> m_CmdList;
	UdsCmd m_CmdNow;
	BOOL m_GetRsp;
	BYTE m_GetRspCnt;
	BYTE m_RspBuf[4];
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetdate();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonWrsn();
	afx_msg void OnBnClickedButtonWrmanudate();
	afx_msg void OnBnClickedButtonRdsn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
