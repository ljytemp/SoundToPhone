#pragma once
#include <mmsystem.h>

#define WM_RECORDSOUND_STARTRECORDING	WM_USER+500
#define WM_RECORDSOUND_STOPRECORDING	WM_USER+501
#define WM_RECORDSOUND_ENDTHREAD		WM_USER+502

#define MAXRECBUFFER	8
#define RECBUFFER		1 //8192    来自手机播放缓存 3072
#define SAMPLEPSEC      11025

//参考 ： http://yaycici.blog.163.com/blog/static/173759063201110444857597/
// CAudioRec

class CAudioRec : public CWinThread
{
	DECLARE_DYNCREATE(CAudioRec)

	CDialogEx* m_pDlg;

	HWAVEIN m_hRecord;
	WAVEFORMATEX m_WaveFormatEx; 
	LPWAVEHDR rechead[MAXRECBUFFER];
	//int m_RECBUFFER;

	BOOL recording;//是否在正常录制

	CAudioRec();           // protected constructor used by dynamic creation
	CAudioRec(int RecordBuffSize);
	virtual ~CAudioRec();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void CreateThreadEx(CDialogEx* dlg);
	LPWAVEHDR CreateWaveHeader();
	void PreCreateHeader();

	afx_msg void OnStartRecording(WPARAM wp,LPARAM lp);
	afx_msg void OnStopRecording(WPARAM wp,LPARAM lp);
	afx_msg void OnEndThread(WPARAM wp,LPARAM lp);
	afx_msg void OnSoundData(WPARAM wParam, LPARAM lParam);

	int m_RECBUFFER;
	void ResetThread(int bufSize);
protected:
	DECLARE_MESSAGE_MAP()
};


