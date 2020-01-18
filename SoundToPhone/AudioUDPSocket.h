#pragma once


#define SOCK_PORT_CONTROL   22100
#define SOCK_PORT_AUDIO     22101

// CAudioUDPSocket command target


class CAudioUDPSocket : public CAsyncSocket
{
	char* m_data;
	int m_dlen;
	CDialogEx* m_pDlg;
public:
	CAudioUDPSocket();
	virtual ~CAudioUDPSocket();
	void CreateSocket(int type,CDialogEx* dlg);
	int SendDataTo(char* data,int datalen,char* addr,unsigned int port);

private:
	int m_type; //类型，控制、数据
	static char m_addr[512];
	int remotePort;
	
public:
	virtual void OnReceive(int nErrorCode);
	void SetParent(CDialogEx *dlg);
};


