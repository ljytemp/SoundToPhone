
// SoundToPhoneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SoundToPhone.h"
#include "SoundToPhoneDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSoundToPhoneDlg �Ի���




CSoundToPhoneDlg::CSoundToPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoundToPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_AudioRec = NULL;
	m_bufferSize = RECBUFFER; //Ĭ��ֵ
}

void CSoundToPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSoundToPhoneDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNSTART, &CSoundToPhoneDlg::OnBnClickedBtnstart)
	ON_BN_CLICKED(IDC_BTNSTOP, &CSoundToPhoneDlg::OnBnClickedBtnstop)
	ON_BN_CLICKED(IDOK, &CSoundToPhoneDlg::OnBnClickedOk)
	ON_MESSAGE(WM_REFRESH,&CSoundToPhoneDlg::OnRefreshListCtrl)
	ON_BN_CLICKED(IDC_BTNTEST, &CSoundToPhoneDlg::OnBnClickedBtntest)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSoundToPhoneDlg ��Ϣ�������

BOOL CSoundToPhoneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	UpdateStatic(L"ֹͣ״̬��");
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CListCtrl* pLstCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCLIENT);
	//��ʽ�����ο� http://hi.baidu.com/daihaipengdhp/item/751a843af50daf5981f1a79a
	LONG lStyle;
	lStyle = GetWindowLong(pLstCtrl->m_hWnd,GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(pLstCtrl->m_hWnd,GWL_STYLE,lStyle);

	DWORD dwStyle;
	dwStyle = pLstCtrl->GetExStyle();
	//dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;//��checkbox ,��Ҫ
	dwStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	pLstCtrl->SetExtendedStyle(dwStyle);

	//test
	pLstCtrl->InsertColumn(1,L"IP��ַ", LVCFMT_CENTER,150);
	pLstCtrl->InsertColumn(2,L"port"  , LVCFMT_CENTER,50 );
	pLstCtrl->InsertColumn(3,L"����"  , LVCFMT_CENTER,60 );


	GetLocalIp();

	m_csocket.CreateSocket(SOCK_PORT_CONTROL,this);
	m_csocket.SetParent(this);
	m_asocket.CreateSocket(SOCK_PORT_AUDIO,this);
	m_asocket.SetParent(this);
	//m_AudioRec = new CAudioRec()
	//m_AudioRec.CreateThreadEx(this);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSoundToPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSoundToPhoneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSoundToPhoneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSoundToPhoneDlg::OnBnClickedBtnstart()
{
	// TODO: Add your control notification handler code here
	if( m_mapClient.size()==0 && FALSE == GetBroadCastCheck() )
	{
		MessageBox(L"û�пͻ���",L"��ʾ",0);
		return ;
	}
	if (m_AudioRec)
	{
		m_AudioRec->PostThreadMessage(WM_RECORDSOUND_STARTRECORDING,0,0);
	}
	
}

void CSoundToPhoneDlg::OnBnClickedBtnstop()
{
	// TODO: Add your control notification handler code here
	UpdateStatic(L"ֹͣ״̬��");
//	m_mapClient.clear();
	CListCtrl* pLstCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCLIENT);
//	pLstCtrl->DeleteAllItems();
	//OnRefreshListCtrl(0,0);
	if (m_AudioRec) m_AudioRec->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING,0,0);
}


void CSoundToPhoneDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//m_AudioRec.PostThreadMessage(WM_RECORDSOUND_ENDTHREAD,0,0);
	CDialogEx::OnOK();
}

LRESULT CSoundToPhoneDlg::OnRefreshListCtrl( WPARAM wParam, LPARAM lParam )
{
	CListCtrl* pLstCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCLIENT);
	//std::map<char*,int>::iterator iter;  //���˼���Ĵ���std::map<char*,int>��һ������Ϊָ��,��Ϊ�Ǽ� �ο�;
	//http://blog.csdn.net/xieqidong/article/details/3504469

	pLstCtrl->DeleteAllItems();

	std::map<std::string,int>::iterator iter;
	for(iter= m_mapClient.begin();iter != m_mapClient.end();iter++)
	{
		const char* addr = (*iter).first.c_str();  //�൱�ڸı䱻ָ��ָ�򣬿��ܻᱻ�޸ģ����Ǽ�const����
		int port = (*iter).second;
		//((CSoundToPhoneDlg*)m_pDlg)->m_asocket.SendDataTo(lpHdr->lpData,lpHdr->dwBytesRecorded,addr,port);
		int nRow = pLstCtrl->InsertItem(1,L"�����ַ���",0);
		
		CStringW addrW = (CStringW)addr;  //ǿ��ת�˶���ljy
		pLstCtrl->SetItemText(nRow,0,(LPCTSTR)addrW);
		CString str;
		str.Format(L"%d",port);
		pLstCtrl->SetItemText(nRow,1,str);

		str.Format(L"%d",m_bufferSize);
		pLstCtrl->SetItemText(nRow,2,str);
	}

	/*
	if (m_AudioRec)
	{
		m_AudioRec->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING,0,0);
		delete m_AudioRec;
		m_AudioRec = NULL;
	}
	if (m_bufferSize>0)
	{
		m_AudioRec = new CAudioRec(m_bufferSize);
		m_AudioRec->CreateThreadEx(this);
	}
	*/
	if (m_AudioRec)
	{
		m_AudioRec->m_RECBUFFER = m_bufferSize;
		m_AudioRec->PreCreateHeader();
	}
	else
	{
		m_AudioRec = new CAudioRec(m_bufferSize);
		m_AudioRec->CreateThreadEx(this);
	}

	return 0L;
}


void CSoundToPhoneDlg::OnBnClickedBtntest()
{
	if (m_AudioRec) m_AudioRec->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING,0,0);
	if (m_AudioRec) m_AudioRec->PostThreadMessage(WM_RECORDSOUND_ENDTHREAD,0,0);

	delete m_AudioRec;
	m_AudioRec = NULL;
	m_AudioRec = new CAudioRec(m_bufferSize);
	m_AudioRec->CreateThreadEx(this);

	// TODO: Add your control notification handler code here
	//CAudioUDPSocket skt;
	//skt.CreateSocket(SOCK_PORT_CONTROL,this);
	//skt.SetParent(this);
	//skt.Bind(12103);
	//int ret = skt.SendDataTo("ljygood",7,"192.168.0.104",12100);
	//int err =  GetLastError();

	//int ret = m_csocket.SendDataTo("ljygood",7,"192.168.0.104",12100);



	////10022 ����IP��ַת����������ɣ����������Ѿ�OK
	////m_csocket.SetSockOpt()
	//CString addr("192.168.0.104");
	//int ret = m_csocket.SendTo("ljygood",7,12100,(LPCTSTR)addr);
	//int err =  GetLastError();
}

void CSoundToPhoneDlg::UpdateStatic(CString txt)
{
	
	CStatic* pStatic = (CStatic*) GetDlgItem(IDC_STC_STATUS);
	pStatic->SetWindowText(txt);
}

BOOL CSoundToPhoneDlg::GetBroadCastCheck()
{
	CButton* pCheck = (CButton*) GetDlgItem(IDC_CHECKBOX);
	return pCheck->GetCheck();
}


void CSoundToPhoneDlg::OnDestroy()
{
	
	m_AudioRec->PostThreadMessage(WM_RECORDSOUND_ENDTHREAD,0,0);
	if (m_AudioRec)
	{
		delete m_AudioRec;
		m_AudioRec = NULL;
	}
	CDialogEx::OnDestroy();
	// TODO: Add your message handler code here
}

CString CSoundToPhoneDlg::GetLocalIp()
{
	CString strIP = L"";
	char hostname[128]; 
	if(::gethostname(hostname,128)==0) 
	{ 
		//printf("%s\n",hostname);//���������
		//TRACE(L"%s\n",hostname);
		::OutputDebugStringA(hostname);
	} 
	struct hostent *pHost = gethostbyname(hostname); 
	for (int i = 0; pHost != NULL && pHost->h_addr_list[i] != NULL; i++)
	{ 
		//printf("%s\n",inet_ntoa(*(struct in_addr*)pHost->h_addr_list[i]));		
		strIP = inet_ntoa(*(struct in_addr*)pHost->h_addr_list[i]);
		::OutputDebugString(strIP);
	} 
	
	CStatic* pStatic = (CStatic*) GetDlgItem(IDC_STC_IP);
	pStatic->SetWindowText(strIP);
	return L"";
}

int CSoundToPhoneDlg::MinGongBS( int a,int b )
{
	int max = (a >= b?a:b),min = (a < b?a:b),i;
	for(i = 1;;++i)
	{
		if((max * i) % min == 0)
		{
			return (max * i);
		}
	}

}

int CSoundToPhoneDlg::MaxGongYS( int a,int b )
{
	int r;
	while(b!=0)
	{
		r = a%b;
		a = b;
		b = r;
	}
	return a;
}
