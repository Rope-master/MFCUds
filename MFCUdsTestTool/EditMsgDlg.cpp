// EditMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUdsTestTool.h"
#include "EditMsgDlg.h"
#include "afxdialogex.h"

#include "UdsUtil.h"
// CEditMsgDlg 对话框

IMPLEMENT_DYNAMIC(CEditMsgDlg, CDialogEx)

CEditMsgDlg::CEditMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_EDITMSG, pParent)
{

}

CEditMsgDlg::~CEditMsgDlg()
{
}

void CEditMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_DLC, m_CombDlc);
	DDX_Text(pDX, IDC_EDIT_CANID, str_EditId);
	DDX_Text(pDX, IDC_EDIT_DATA, str_EditData);
	DDX_Text(pDX, IDC_EDIT_CYCLE, str_EditCycle);
	DDX_Text(pDX, IDC_EDIT_FRAMENUM, str_EditFrameNum);
}


BEGIN_MESSAGE_MAP(CEditMsgDlg, CDialogEx)
END_MESSAGE_MAP()


// CEditMsgDlg 消息处理程序


BOOL CEditMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_CombDlc.SetCurSel(pEditMsg->Dlc);//初始化DLC

	GetDlgItem(IDC_EDIT_CANID)->SetWindowText(pEditMsg->Id);
	GetDlgItem(IDC_EDIT_DATA)->SetWindowText(pEditMsg->Data);
	GetDlgItem(IDC_EDIT_CYCLE)->SetWindowText(pEditMsg->Cycle);
	GetDlgItem(IDC_EDIT_FRAMENUM)->SetWindowText(pEditMsg->FrameNum);

	m_EditId.SubclassWindow(GetDlgItem(IDC_EDIT_CANID)->m_hWnd);
	m_EditData.SubclassWindow(GetDlgItem(IDC_EDIT_DATA)->m_hWnd);
	m_EditCycle.SubclassWindow(GetDlgItem(IDC_EDIT_CYCLE)->m_hWnd);
	m_EditFrameNum.SubclassWindow(GetDlgItem(IDC_EDIT_FRAMENUM)->m_hWnd);

	m_EditId.SetLimitText(3);
	m_EditData.SetLimitText(23);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEditMsgDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);//更新控件数据

	pEditMsg->Dlc = m_CombDlc.GetCurSel();
	pEditMsg->Id = str_EditId;
	pEditMsg->Data = str_EditData;
	pEditMsg->Cycle = str_EditCycle;
	pEditMsg->FrameNum = str_EditFrameNum;
	CDialogEx::OnOK();
}
