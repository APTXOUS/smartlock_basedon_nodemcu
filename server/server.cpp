//test_server.c
//1650254 ��Ң��
#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<signal.h>

#include<fcntl.h>
#include <sys/prctl.h>
#include <string.h>
#include <sys/times.h>

#include <sys/ioctl.h>
#include<arpa/inet.h>
#include<errno.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>


#define OK 0
#define ERROR -1
int Create_Damon();
int init_server(int _port,const char* _ip);
int check_is_integer(const char * source);

int main(int argc,char **argv)
{
    int n;
    if(OK==check_is_integer(argv[1]))
        n=atoi(argv[1]);
    else
        exit(ERROR);
    printf("Ҫ�󶨵�ip�˿�Ϊ%d\n",4000);




    int listen_sock = init_server(n,"0.0.0.0");//��ʼ��
    fd_set rfds, wfds;

    struct sockaddr_in remote;
    socklen_t len = sizeof(struct sockaddr_in);
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(listen_sock, &rfds);
    FD_SET(listen_sock, &wfds);
    int sock;
    if(select(listen_sock+1,&rfds,&wfds,NULL,NULL)>0){
        sock = accept(listen_sock, (struct sockaddr*)&remote, &len);
        if(sock < 0)
        {
            perror("accept");
            exit(-1);
        }
    }
    printf("get a client, ip:%s, port:%d\n",inet_ntoa(remote.sin_addr),ntohs(remote.sin_port));
    //listen
    //���÷�����
    int flag=1;
    if( ioctl(sock,FIONBIO,&flag)<0)
    {  
        perror("ioctl");
        exit(-1);
    }

    while(1)
    {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(sock, &rfds);
        FD_SET(sock, &wfds);
        ssize_t _s;
        switch(select(sock+1,NULL,&wfds,NULL,NULL))
        {
            case -1: exit(-1);break; //select�����˳����� 
            case 0:break; //�ٴ���ѯ 
            default: 
            _s = write(sock, "123456789", 10);
            printf("�ѷ��ͣ�%d�ֽ�\n",_s);
        }
        sleep(1);
    }
    close(listen_sock);
    printf("���ӽ���\n");
    return 0;


}




int init_server(int _port,const char* _ip)//������
{
    fd_set rfds, wfds;
    int sock = socket(AF_INET,SOCK_STREAM,0);
    //����socket()����һ���׽�������ͨѶ
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    int flag=1;
    if( ioctl(sock,FIONBIO,&flag)<0)
     {  
          perror("ioctl");
          exit(-1);
     }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons( _port);
    local.sin_addr.s_addr = inet_addr(_ip);
    socklen_t len = sizeof(local);
    //����bind()���а�����ļ�������


    
    if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {

        perror("setsockopt fail");

    }

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sock, &rfds);
    FD_SET(sock, &wfds);

    if(select(sock+1,&rfds,&wfds,NULL,NULL)>0)
        if(bind(sock,(struct sockaddr*)&local , len) < 0)
        {
            perror("bind");
            exit(2);
        }
    printf("bind�ɹ�,socket:%d\n",sock);
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sock, &rfds);
    FD_SET(sock, &wfds);
    if(select(sock+1,&rfds,&wfds,NULL,NULL)>0)
        if(listen(sock, 5) < 0) //�������ӵ��������Ϊ5
        {
            perror("listen");
            exit(3);
        }
    printf("listen�ɹ�,socket:%d\n",sock);

    return sock;

}


int check_is_integer(const char * source)
{
    int n=strlen(source);
    int i;
    for(i=0;i<n;i++)
        if(source[i]>'9'||source[i]<'0')
            return ERROR;
    return OK;
}