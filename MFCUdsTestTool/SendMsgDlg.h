#pragma once
#include "LimitEdit.h"

// CSendMsgDlg 对话框

class CSendMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSendMsgDlg)

public:
	CSendMsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendMsgDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SENDMSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CLimitHexEdit m_EditData;

	CString m_Editcantx;
	CString m_EditCanid;

	BOOL m_CanExt;
	BOOL m_CanRmt;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTx();
	virtual void OnOK();
	virtual void OnCancel();
};
