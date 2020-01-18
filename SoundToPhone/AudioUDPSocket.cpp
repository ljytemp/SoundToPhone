// AudioUDPSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SoundToPhone.h"
#include "AudioUDPSocket.h"
#include "SoundToPhoneDlg.h"


// CAudioUDPSocket

CAudioUDPSocket::CAudioUDPSocket()
{
	m_data = new char[8192];
	m_dlen = 8192;
}

CAudioUDPSocket::~CAudioUDPSocket()
{
}

void CAudioUDPSocket::CreateSocket(int portType,CDialogEx* dlg)
{
	m_type = portType;
	m_pDlg = dlg;
	//char localname[512];
	//memset(localname,0,512);
	//gethostname(localname,512);	
	if (FALSE == Create(portType,SOCK_DGRAM) )
	{
		TRACE(L"NO---LJY");
	}
	//AsyncSelect( FD_READ );
	// 
}

int CAudioUDPSocket::SendDataTo( char* data,int datalen,char* addr,unsigned int port )
{
	//char* tdata = "helloPP";
	//int len = strlen(tdata);
	//int tport = 12100;
	//return SendTo(tdata,len,tport,(LPCTSTR)addr);
	/* OK p2p
	((CSoundToPhoneDlg*)m_pDlg)->UpdateStatic(L"数据发送中...");
	CString addrT(addr);
	return SendTo(data,datalen,port,(LPCTSTR)addrT);
	*/

	((CSoundToPhoneDlg*)m_pDlg)->UpdateStatic(L"数据发送中...");

	if (FALSE == ((CSoundToPhoneDlg*)m_pDlg)->GetBroadCastCheck() )
	{
		//((CSoundToPhoneDlg*)m_pDlg)->UpdateStatic(L"数据发送中...");
		CString addrT(addr);
		return SendTo(data,datalen,port,(LPCTSTR)addrT);
	}
	else
	{
		//广播发送

		CString addrT(addr);
		int i = 1;
		//SetSockOpt(SO_BROADCAST,(char *)&(i),sizeof(BOOL), SOL_SOCKET);
		BOOL a= SetSockOpt(SO_BROADCAST, (char *)&(i), sizeof(BOOL), SOL_SOCKET);

		//int nRe = sock.SendTo((LPCTSTR)str, host.GetLength(), nPort, NULL, SOCK_DGRAM|SO_BROADCAST|MSG_PEEK);
		//int nRe = sock.SendTo((LPCTSTR)str, host.GetLength(), nPort);  OK
		//return SendTo(data,datalen,port,(LPCTSTR)addrT);

		sockaddr_in local;
		int len = sizeof(local);
		memset(&local, 0, len);
		local.sin_family = AF_INET;
		local.sin_port = htons(32100);//htons(dwPort);
		local.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		return SendTo(data,datalen,(const SOCKADDR*)&local,len);
	}
	return 0;
}


// CAudioUDPSocket member functions


void CAudioUDPSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	int retvalue = SOCKET_ERROR;
	CString straddr;
	unsigned int port=0;

	//m_dlen = 0;  //一定要
	//memset(m_data,0,8192);
	if(SOCK_PORT_CONTROL == m_type)
	{
		//接收通知信号，并保持起来		
		retvalue= ReceiveFrom(m_data,m_dlen,straddr,port);
		
		//这个参数：m_dlen 搞了一晚上，因为这里要求m_dlen的大小等于 m_data大小。通过int err = GetLastError();google到的。
		
		//SOCKADDR_IN skaddr;
		//int lenSokaddr = sizeof(skaddr);
		//retvalue= ReceiveFrom(m_data,m_dlen,(SOCKADDR*)&skaddr,&lenSokaddr,0);

		switch (retvalue)
		{
		case 0:       // Connection was closed.
			Close();      
			break;
		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				//AfxMessageBox ("Error occurred");
				Close();
			}
			break;
		default: // Normal case: Receive() returned the # of bytes received.
			m_data[retvalue] = 0; //terminate the string (assuming a string for this example)
			CString strReceivedData(m_data);       // This is the input data  
			
			int bufSize = _ttoi(strReceivedData);

			////获取最大公约数   //不行哦
			//if (1!=((CSoundToPhoneDlg*)m_pDlg)->m_bufferSize)
			//{
			//	int CCC = ((CSoundToPhoneDlg*)m_pDlg)->MaxGongYS(((CSoundToPhoneDlg*)m_pDlg)->m_bufferSize,bufSize);
			//	((CSoundToPhoneDlg*)m_pDlg)->m_bufferSize = CCC;  //改变到适合最后上线的手机,实际测试到此还没有效果
			//}
			//else
			//{
			//	((CSoundToPhoneDlg*)m_pDlg)->m_bufferSize = bufSize; 
			//}

			((CSoundToPhoneDlg*)m_pDlg)->m_bufferSize = bufSize; 

		}
		//CAsyncSocket::OnReceive(nErrorCode);

		//could not deduce template argument for 'const std::unique_ptr<_Ty,_Dx> &' fr 错误？？:没有包含<string>
		std::string addr = (CStringA)straddr;
		std::string addrBBB = addr;
		//addrBBB.copy(straddr);
		//参考 ： http://blog.csdn.net/wilsonke/article/details/9238547
		//std::string addr = straddr.GetBuffer(0);
		//SOCKADDR
		//CStringW strw = straddr;
		//CStringA stra(strw.GetBuffer(0));
		//strw.ReleaseBuffer();

		//std::string addr=stra.GetBuffer(0);
		//std::string addrBBB = addr;
		//stra.ReleaseBuffer();
		
		std::map<std::string,int>::iterator iter = ((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.find(addrBBB);
		if( ((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.end() == iter )
			((CSoundToPhoneDlg*)m_pDlg)->m_mapClient.insert(std::make_pair(addrBBB,port));
		
		::SendMessage(m_pDlg->m_hWnd,WM_REFRESH,0,0);
		
		
	}
	else
	{
		//因为不接收其他数据
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CAudioUDPSocket::SetParent( CDialogEx *dlg )
{
	m_pDlg = dlg;
}
