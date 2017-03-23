#pragma once
#include "LimitEdit.h"

typedef struct _EDIT_MSG_T_
{
	CString Id;
	BYTE Dlc;
	CString Cycle;
	CString FrameNum;
	CString Data;
	CString PreData;
}EditMsg_t, *EditMsg_ptr;

// CEditMsgDlg 对话框

class CEditMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditMsgDlg)

public:
	CEditMsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditMsgDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDITMSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_CombDlc;
	CLimitHexEdit m_EditId;
	CLimitHexEdit m_EditData;
	CLimitDecEdit m_EditCycle;
	CLimitDecEdit m_EditFrameNum;

	CString str_EditId;
	CString str_EditCycle;
	CString str_EditFrameNum;
	CString str_EditData;
public:
	EditMsg_ptr pEditMsg;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
