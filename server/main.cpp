//socket udp 服务端
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const int BUFF_LEN = 200;

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
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);
    int count;
    char buf[BUFF_LEN];
    while (1)
    {
        count = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli, &len);
        if (count == -1)
        {
            printf("recieve data fail!\n");
            return 0;
        }
        printf("server recv: %s\n", buf);
        printf("recv from: %s:%d",inet_ntoa(cli.sin_addr).cli.sin_port);
        memset(buf, 0, BUFF_LEN);
        sprintf(buf, "I have recieved %d bytes data!\n", count);           //回复client
        printf("server send: %s\n", buf);                                        //打印自己发送的信息给
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&cli, len);
    }
    close(sockfd);
    return 0;
}