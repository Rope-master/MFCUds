#pragma once
#include "LimitEdit.h"
#include "EditMsgDlg.h"
#include "ColoredListCtrl.h"
// CSendMsgDlg �Ի���


class CSendMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSendMsgDlg)

public:
	CSendMsgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSendMsgDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SENDMSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:


public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
#if 0
	afx_msg void OnNMRClickListXmit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuAddmsg();
	void XmitMsgAdd();
	// Convert a EditMsg to CanMsg
	CanMsg_t MsgConvert(EditMsg_t EditMsg);
	afx_msg void OnMenuEditmsg();
	afx_msg void OnMenuDelmsg();
	afx_msg void OnMenuClrmsg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static UINT XmitThread(void *param);
	afx_msg void OnBnClickedCheckXmit();
	afx_msg void OnBnClickedCheckSameid();
#endif
};
