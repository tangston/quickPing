#ifndef MYPING_H
#define MYPING_H

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <QString>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
//IP报头
typedef struct icmp_hdr
{
    unsigned char icmp_type;      //消息类型
    unsigned char icmp_code;        //标志代码
    unsigned short icmp_checksum;        //校验和
    //----------------------------------下面是 回显头
    unsigned short icmp_id;        //通常设置为进程ID
    unsigned short icmp_sequence;    //序列号
    unsigned long icmp_timestamp;        //时间戳
} ICMP_HDR, *PICMP_HDR;
//ICMP 报头

typedef struct  ip_hdr {
    UCHAR iphVerLen;
    UCHAR ipTOS;
    USHORT ipLength;
    USHORT ipID;
    USHORT ipFlag;
    UCHAR ipTTL;
    UCHAR ipProtocol;
    USHORT ipCheckSum;
    ULONG ipSource;
    ULONG ipDestination;
} IPHeader, *PIPHeader;

class myPing
{
private:
    SOCKET sRaw;

public://公有方法
    WSADATA wsaData;
    // SOCKET initSocket();
    myPing();
    myPing::~myPing(   );
    myPing::BOOL ping(QString ip);
    QString myPing::ipCount(QString &ip,int index);
    QString myPing::getIP2();

private:
    BOOL SetTimeOut(SOCKET s, int nTime, BOOL bRecv);
    USHORT checksum(USHORT *pBuf, int iSize);
};

#endif // MYPING_H
