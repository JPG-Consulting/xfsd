
// UIDlg.h : ͷ�ļ�
//

#pragma once


// CUIDlg �Ի���
class CUIDlg : public CDialogEx
{
// ����
public:
	CUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnBnClickedMfcmenubutton2();
	afx_msg void OnBnClickedMfcmenubutton4();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnTvnSelchangedMfcshelltree1(NMHDR *pNMHDR, LRESULT *pResult);
};
