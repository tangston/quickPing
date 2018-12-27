#include "myping.h"

myPing::myPing()//构造函数里放初始化的东西
{
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0 ){
        cout << GetLastError;
        system("pause");
    }
    //初始化Windows sockets网络环境
    this->sRaw = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);//创建原始套接字
    SetTimeOut(sRaw, 1000, true);                  //设置接受超时
}
myPing::~myPing(   ){
    WSACleanup();
}
BOOL myPing::ping(QString ip){
    QByteArray ba =ip.toLatin1();
    //设置目的地址
    char * myDestIP  = ba.data();
    SOCKADDR_IN dest;
    dest.sin_addr.S_un.S_addr = inet_addr(myDestIP);
    dest.sin_port = htons(0);//
    dest.sin_family = AF_INET;//IPV4 家族

    //创建ICMP
    char buff[sizeof(ICMP_HDR) + 32];
    ICMP_HDR *plcmp = (ICMP_HDR*)buff;
    //初始化ICMP包
    plcmp->icmp_checksum = 0;
    plcmp->icmp_code = 0;
    plcmp->icmp_id = (USHORT)::GetCurrentProcessId();//一般用目前程序进程号代替
    plcmp->icmp_type = 8;// 8是请求ICMP echo request
    plcmp->icmp_sequence = 0;
    //将其余空间置0
    memset(&buff[sizeof(ICMP_HDR)], 0, 32);


    //----------------开始发送、接受ICMP包
    USHORT nSeq = 0;
    char recvBuff[1024];//接受的时候才用
    SOCKADDR_IN from;//存接受来的地址
    int fromLen = sizeof(from);
    int nRet;//返回的错误type
    plcmp->icmp_checksum = 0;
    plcmp->icmp_timestamp = ::GetTickCount();
    plcmp->icmp_sequence = nSeq++;
    plcmp->icmp_checksum = checksum((USHORT*)buff, sizeof(ICMP_HDR) + 32);
    nRet = ::sendto( sRaw, buff, sizeof(icmp_hdr)+32,0 , (sockaddr*)&dest, sizeof(dest));
    //s 套接字  ，buff 待发送数据的缓冲区，size 缓冲区长度
    //Flags 调用方式标志位, 一般为0, 改变Flags，将会改变Sendto发送的形式
    //addr （可选）指针，指向目的套接字的地址，len addr所指地址的长度
    if (nRet == SOCKET_ERROR){

        cout << "recvfrom()failed:" << WSAGetLastError() << endl;
        system("pause");
        return false;

    }
    nRet = ::recvfrom(sRaw, recvBuff, 1024, 0, (sockaddr*)&from, &fromLen);
    if (nRet == SOCKET_ERROR) {

        if (::WSAGetLastError() == WSAETIMEDOUT) {
            cout << "timed out\n";
            return false;

        }
        cout << "recvfrom()failed:" << WSAGetLastError() << '\n';
        return false;
    }

    //下面解析ICMP包,因为发出去了，所以收到的（发出去的）包会再封装一层ip报的格式，此时读的时候就要用ip报文格式读
    int nTick = ::GetTickCount();
    if (nRet < sizeof(IPHeader)+sizeof(ICMP_HDR)) {
        cout << "too short for byte"<<::inet_ntoa(from.sin_addr);
        return false;
    }
    ICMP_HDR* pRecvIcmp = (ICMP_HDR*)(recvBuff + sizeof(IPHeader));//用来跳过IP头，直接读icmp部分
    if (pRecvIcmp->icmp_type != 0) {//0是响应回显
        cout << "echo type not echo request";
        cout <<(unsigned int)(pRecvIcmp->icmp_type);
        //::system("pause");
        return false;
    }
    if (pRecvIcmp->icmp_id != ::GetCurrentProcessId()) {
        cout << "someone else's packet!\n" ;//不是你的程序的包
       // :: system("pause");
        return false;
    }
    cout << " bytes from " << inet_ntoa(from.sin_addr);
    cout << " icmp sequnce is  " << pRecvIcmp->icmp_sequence;
    cout << " time: " << nTick-pRecvIcmp->icmp_timestamp<<" ms\n";
    ::Sleep(700); //用户可根据需要在100~10000毫秒之间调节
    return true;
}

QString myPing::getIP2()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            //我们使用IPv4地址
            return address.toString();
    }
    return 0;

}
QString myPing::ipCount(QString &ip,int index){//将形参作为引用类型
    QString lastThree=QString::number(index,10);//第二参数是十进制的意思
    QStringList list=ip.split(".");
    QString str=list[0]+"."+list[1]+"."+list[2]+"."+lastThree;
    ip=str;
    qDebug()<<ip;
    return str;
}

BOOL myPing:: SetTimeOut(SOCKET s, int nTime, BOOL bRecv) {
    int ret = setsockopt(s, SOL_SOCKET, bRecv ? SO_RCVTIMEO : SO_SNDTIMEO, (char *)&nTime, sizeof(nTime));
    return ret != SOCKET_ERROR;
}
USHORT myPing:: checksum(USHORT *pBuf, int iSize) {
    unsigned long cksum = 0;
    while (iSize > 1)
    {
        cksum += *pBuf++;
        iSize -= sizeof(USHORT);
    }
    if (iSize) { cksum += *(UCHAR *)pBuf; }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (USHORT)(~cksum);
}
