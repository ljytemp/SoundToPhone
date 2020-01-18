// AudioRec.cpp : implementation file
//

#include "stdafx.h"
#include "SoundToPhone.h"
#include "AudioRec.h"
#include "SoundToPhoneDlg.h"



// CAudioRec

IMPLEMENT_DYNCREATE(CAudioRec, CWinThread)

CAudioRec::CAudioRec()
{
	recording=FALSE;
	//isallocated=0;

	PreCreateHeader();
	//isallocated=1;

	//Setting WAVEFORMATEX  structure for the audio input
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wFormatTag		=	WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels		=	1;
	m_WaveFormatEx.wBitsPerSample	=	16;
	m_WaveFormatEx.cbSize			=	0;
	m_WaveFormatEx.nSamplesPerSec	=	SAMPLEPSEC;  //11025 Hz
	m_WaveFormatEx.nBlockAlign		=	m_WaveFormatEx.wBitsPerSample/8;
	m_WaveFormatEx.nAvgBytesPerSec	=	m_WaveFormatEx.nBlockAlign * m_WaveFormatEx.nSamplesPerSec;
}

CAudioRec::CAudioRec( int RecordBuffSize )
{
	m_RECBUFFER = RecordBuffSize;
	recording=FALSE;
	//isallocated=0;

	PreCreateHeader();

	//isallocated=1;

	//Setting WAVEFORMATEX  structure for the audio input
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wFormatTag		=	WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels		=	1;
	m_WaveFormatEx.wBitsPerSample	=	16;
	m_WaveFormatEx.cbSize			=	0;
	m_WaveFormatEx.nSamplesPerSec	=	SAMPLEPSEC;  //11025 Hz
	m_WaveFormatEx.nBlockAlign		=	m_WaveFormatEx.wBitsPerSample/8;
	m_WaveFormatEx.nAvgBytesPerSec	=	m_WaveFormatEx.nBlockAlign * m_WaveFormatEx.nSamplesPerSec;
}

void CAudioRec::PreCreateHeader()
{
	for(int i=0;i<MAXRECBUFFER;i++)
		rechead[i]=CreateWaveHeader();
}

CAudioRec::~CAudioRec()
{
	//if(!isallocated)	return;
	OnEndThread(0,0);
	for(int i=0;i<MAXRECBUFFER;i++)
	{
		if(rechead[i])
		{
			if(rechead[i]->lpData )
				delete [](rechead[i]->lpData);
			delete rechead[i];
		}
	}
}

LPWAVEHDR CAudioRec::CreateWaveHeader()
{
	LPWAVEHDR lpHdr = new WAVEHDR;
	if(lpHdr==NULL)		return NULL;

	ZeroMemory(lpHdr, sizeof(WAVEHDR));
	//char* lpByte = new char[RECBUFFER];
	char* lpByte = new char[m_RECBUFFER];

	if(lpByte==NULL)	return NULL;

	lpHdr->lpData =  lpByte;
	//lpHdr->dwBufferLength =RECBUFFER;
	lpHdr->dwBufferLength =m_RECBUFFER;
	return lpHdr;
}

BOOL CAudioRec::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CAudioRec::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAudioRec, CWinThread)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_STARTRECORDING,OnStartRecording)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_STOPRECORDING,OnStopRecording)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_ENDTHREAD,OnEndThread)
	ON_THREAD_MESSAGE(MM_WIM_DATA,OnSoundData)
END_MESSAGE_MAP()

void CAudioRec::OnStartRecording( WPARAM wp,LPARAM lp )
{
	//如果没有客户端，就没有必要录制发送 发送广播？
	//if ( 0 ==((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.size() ) return;


	//参考 : http://wenku.baidu.com/link?url=bq-bu5wOmYCnB65rqrlIzL64iSmWpxloWjbVzO4tcD3GSuQ2ij8z2G8MEQ3uwhJPVxApGDlCiddBbTu2fakBfPeLyOyn0-tMCzpZl_YAzom

	MMRESULT mmReturn = ::waveInOpen( &m_hRecord, WAVE_MAPPER, &m_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
	if(mmReturn!=MMSYSERR_NOERROR )		
		return ;

	if(mmReturn==MMSYSERR_NOERROR )
	{
		for(int i=0; i < MAXRECBUFFER ; i++)
		{
			mmReturn = ::waveInPrepareHeader(m_hRecord,rechead[i], sizeof(WAVEHDR));
			mmReturn = ::waveInAddBuffer(m_hRecord, rechead[i], sizeof(WAVEHDR));
		}
		mmReturn = ::waveInStart(m_hRecord);
		if(mmReturn==MMSYSERR_NOERROR )
			recording=TRUE;
	}
	return ;
}

void CAudioRec::OnStopRecording( WPARAM wp,LPARAM lp )
{
	MMRESULT mmReturn = 0;
	if(!recording)		
		return ;
	mmReturn = ::waveInStop(m_hRecord);
	recording = FALSE;

	mmReturn = ::waveInReset(m_hRecord);  
	Sleep(500); 
	
	mmReturn = ::waveInClose(m_hRecord);
}

void CAudioRec::OnEndThread( WPARAM wp,LPARAM lp )
{
	if(recording)	
		OnStopRecording(0,0);
	::PostQuitMessage(0);
}

void CAudioRec::OnSoundData( WPARAM wParam, LPARAM lParam )
{
	//int clen=1;
	if(FALSE == recording )	
	{
		//放一段空白数据，否则会杂音残留，麻烦
		//char* lpByte = new char[m_RECBUFFER];
		//((CSoundToPhoneDlg*)m_pDlg)->m_asocket.SendDataTo(lpByte,m_RECBUFFER,addrB,portB);
		return ;
	}
	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
	if(lpHdr->dwBytesRecorded==0 || lpHdr==NULL)	return ;
	::waveInUnprepareHeader(m_hRecord, lpHdr, sizeof(WAVEHDR));

	//if(lpHdr->lpData!=NULL )
	//{
	//	( (CGraghDialogDlg*) pDlg )->m_G729aCompress .Compress  (lpHdr->lpData,lpHdr->dwBytesRecorded,((CGraghDialogDlg*) pDlg )->m_AComped,&clen);
	//	if(clen>0)
	//	{
	//		((CGraghDialogDlg*) pDlg )->daudio.SendAudioData((unsigned char *)(((CGraghDialogDlg*) pDlg )->m_AComped),clen);
	//		((CGraghDialogDlg*) pDlg )->m_ABytesSent +=clen;
	//		((CGraghDialogDlg*) pDlg )->m_AFrames  ++;
	//		//			((CGraghDialogDlg*) pDlg )->daudio.SendAudioData((unsigned char *)lpHdr->lpData,lpHdr->dwBytesRecorded);
	//	}
	//}

	//if ( 0 ==((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.size() )
	if (TRUE == ((CSoundToPhoneDlg*)m_pDlg)->GetBroadCastCheck() )
	{
		unsigned int portB = 32100;
		//CString addrB("255.255.255.255");
		char* addrB = "255.255.255.255";
		((CSoundToPhoneDlg*)m_pDlg)->m_asocket.SendDataTo(lpHdr->lpData,lpHdr->dwBytesRecorded,addrB,portB);
	}
	else
	{
		std::map<std::string,int>::iterator iter;
		for(iter=((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.begin();iter != ((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.end();iter++)
		{
			//char* addr = (*iter).first;
			//参考 ： http://www.cnblogs.com/chenyuming507950417/archive/2012/06/27/2564603.html

			char *addr = new char[(*iter).first.length() + 1];		
			strcpy(addr,(*iter).first.c_str());

			int port = (*iter).second;


			//CStringA strT("255.255.255.255");
			//char *addr=strT.GetBuffer(0);
			//int port = 32100;

			((CSoundToPhoneDlg*)m_pDlg)->m_asocket.SendDataTo(lpHdr->lpData,lpHdr->dwBytesRecorded,addr,port);
			delete[] addr;
		}
	}
	
	
	if(recording)
	{	//Reuse the old buffer
		::waveInPrepareHeader(m_hRecord,lpHdr, sizeof(WAVEHDR));
		::waveInAddBuffer(m_hRecord, lpHdr, sizeof(WAVEHDR));
	}
}

void CAudioRec::CreateThreadEx( CDialogEx* dlg )
{
	m_pDlg = dlg;
	CreateThread();
}

void CAudioRec::ResetThread( int bufSize )
{
	m_RECBUFFER = bufSize;
	for(int i=0;i<MAXRECBUFFER;i++)
	{
		if(rechead[i])
		{
			if(rechead[i]->lpData )
				delete [](rechead[i]->lpData);
			delete rechead[i];
		}
	}
	PreCreateHeader();
}


// CAudioRec message handlers
