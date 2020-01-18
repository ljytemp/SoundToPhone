
// SoundToPhoneDlg.h : ͷ�ļ�
//

#pragma once
#include "AudioUDPSocket.h"
#include <map>
#include <string>
#include "AudioRec.h"


#define WM_REFRESH WM_USER+109

using namespace std;
// CSoundToPhoneDlg �Ի���
class CSoundToPhoneDlg : public CDialogEx
{
// ����
public:
	CSoundToPhoneDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOUNDTOPHONE_DIALOG };

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
	CAudioUDPSocket m_csocket;
	CAudioUDPSocket m_asocket;
	CAudioRec* m_AudioRec; //�ĳ�ָ�룬��ΪҪ���ݲ�����̬����
	std::map<std::string,int> m_mapClient;
	int m_bufferSize;

	afx_msg void OnBnClickedBtnstart();
	afx_msg void OnBnClickedBtnstop();		
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnRefreshListCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtntest();

	void UpdateStatic(CString txt);
	BOOL GetBroadCastCheck();
	afx_msg void OnDestroy();

	CString GetLocalIp();
	int MinGongBS(int a,int b);
	int MaxGongYS(int a,int b);
};
