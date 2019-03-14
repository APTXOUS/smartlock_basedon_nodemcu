//socket udp �����
//����
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

const int BUFF_LEN = 200;


struct package{
    char Id[16];    //15λ�豸ID
    char Divide;    //�Ƿ�ְ� 0�����ְ� �������ְ��ı�ʾ
    char Seq;       //�ڼ�����
    char Len;       //������
    char Type;      //������
    char Info[100]; //����
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
    //����socket����
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //��������ͨ�Ŷ���
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    //��socket������ͨ������
    int ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (0 > ret)
    {
        printf("bind\n");
        return -1;
    }

    //---------------------��ʼ�����------------------------------
    //��������
    //1:����udp��
    //2:����udp��
    //  �ظ�ack��
    //  �����豸id�ǳ������ӣ������豸id
    //  ���豸id�Ѿ������棬�����豸����ʱ��
    
    
    //�Ҹо�����û�����⣨�п����Ժ��İȣ�

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
        

        sprintf(buf, "I have recieved %d bytes data!\n", count);                //�ظ�client
        printf("server send: %s\n", buf);                                       //��ӡ�Լ����͵���Ϣ��
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&cli, len);
    }
    close(sockfd);
    return 0;
}
