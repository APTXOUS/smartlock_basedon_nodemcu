//socket udp 服务端
//包名
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

const int BUFF_LEN = 200;


struct package{
    char Id[16];    //15位设备ID
    char Divide;    //是否分包 0：不分包 其他：分包的标示
    char Seq;       //第几个包
    char Len;       //包长度
    char Type;      //包类型
    char Info[100]; //属性
};

struct machine{
    char Id[16];  
    struct sockaddr_in addr_info;
    int status;
    char buf[500];
};  

void save_machince_info(struct package info)
{

}

void save_status()
{

}

void do_package_command(struct package info)
{

}

int main()
{
    //创建socket对象
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    //绑定socket对象与通信链接
    int ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (0 > ret)
    {
        printf("bind\n");
        return -1;
    }

    //---------------------初始化完成------------------------------
    //任务流程
    //1:监听udp包
    //2:分析udp包
    //  回复ack包
    //  若是设备id是初次连接，保存设备id
    //  若设备id已经被保存，更新设备连接时间
    
    
    //我感觉阻塞没有问题（有可能以后会改叭）

    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int count;
    char buf[BUFF_LEN];
    while (1)
    {
        struct package temp;
        count = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli, &len);
        if (count == -1)
        {
            printf("recieve data fail!\n");
            return 0;
        }
        printf("server recv: %s\n", buf);
        printf("recv from: %s:%d\n",inet_ntoa(cli.sin_addr),cli.sin_port);
        memcpy((char *)&temp,buf,count);
        save_machince_info(temp);
        do_package_command(temp);
        

        sprintf(buf, "I have recieved %d bytes data!\n", count);                //回复client
        printf("server send: %s\n", buf);                                       //打印自己发送的信息给
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&cli, len);
    }
    close(sockfd);
    return 0;
}
