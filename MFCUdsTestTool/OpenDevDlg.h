#pragma once
#include "LimitEdit.h"

// COpenDevDlg 对话框

class COpenDevDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenDevDlg)

public:
	COpenDevDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenDevDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OPENDEV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CLimitDecEdit m_EditExBgnid;
	CLimitDecEdit m_EditExEndid;

	CString m_EditBgnid;
	CString m_EditEndid;

	CComboBox m_combchnl;
	CComboBox m_combbaud;

public:
	PINT p_DlgRet;
public:

//	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedBtOpendev();
};
