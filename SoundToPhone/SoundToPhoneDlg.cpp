
// SoundToPhoneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SoundToPhone.h"
#include "SoundToPhoneDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSoundToPhoneDlg 对话框




CSoundToPhoneDlg::CSoundToPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoundToPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_AudioRec = NULL;
	m_bufferSize = RECBUFFER; //默认值
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


// CSoundToPhoneDlg 消息处理程序

BOOL CSoundToPhoneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	UpdateStatic(L"停止状态。");
	// TODO: 在此添加额外的初始化代码
	CListCtrl* pLstCtrl = (CListCtrl*) GetDlgItem(IDC_LISTCLIENT);
	//改式样：参考 http://hi.baidu.com/daihaipengdhp/item/751a843af50daf5981f1a79a
	LONG lStyle;
	lStyle = GetWindowLong(pLstCtrl->m_hWnd,GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(pLstCtrl->m_hWnd,GWL_STYLE,lStyle);

	DWORD dwStyle;
	dwStyle = pLstCtrl->GetExStyle();
	//dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;//有checkbox ,不要
	dwStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	pLstCtrl->SetExtendedStyle(dwStyle);

	//test
	pLstCtrl->InsertColumn(1,L"IP地址", LVCFMT_CENTER,150);
	pLstCtrl->InsertColumn(2,L"port"  , LVCFMT_CENTER,50 );
	pLstCtrl->InsertColumn(3,L"名称"  , LVCFMT_CENTER,60 );


	GetLocalIp();

	m_csocket.CreateSocket(SOCK_PORT_CONTROL,this);
	m_csocket.SetParent(this);
	m_asocket.CreateSocket(SOCK_PORT_AUDIO,this);
	m_asocket.SetParent(this);
	//m_AudioRec = new CAudioRec()
	//m_AudioRec.CreateThreadEx(this);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSoundToPhoneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSoundToPhoneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSoundToPhoneDlg::OnBnClickedBtnstart()
{
	// TODO: Add your control notification handler code here
	if( m_mapClient.size()==0 && FALSE == GetBroadCastCheck() )
	{
		MessageBox(L"没有客户端",L"提示",0);
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
	UpdateStatic(L"停止状态。");
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
	//std::map<char*,int>::iterator iter;  //犯了极大的错误，std::map<char*,int>第一个不能为指针,因为是键 参考;
	//http://blog.csdn.net/xieqidong/article/details/3504469

	pLstCtrl->DeleteAllItems();

	std::map<std::string,int>::iterator iter;
	for(iter= m_mapClient.begin();iter != m_mapClient.end();iter++)
	{
		const char* addr = (*iter).first.c_str();  //相当于改变被指针指向，可能会被修改，于是加const限制
		int port = (*iter).second;
		//((CSoundToPhoneDlg*)m_pDlg)->m_asocket.SendDataTo(lpHdr->lpData,lpHdr->dwBytesRecorded,addr,port);
		int nRow = pLstCtrl->InsertItem(1,L"无用字符串",0);
		
		CStringW addrW = (CStringW)addr;  //强制转了对了ljy
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



	////10022 错误：IP地址转换后乱码造成，如下这样已经OK
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
		//printf("%s\n",hostname);//计算机名字
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
