//test_server.c
//1650254 尤尧寅
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
    printf("要绑定的ip端口为%d\n",4000);




    int listen_sock = init_server(n,"0.0.0.0");//初始化
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
    //设置非阻塞
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
            case -1: exit(-1);break; //select错误，退出程序 
            case 0:break; //再次轮询 
            default: 
            _s = write(sock, "123456789", 10);
            printf("已发送：%d字节\n",_s);
        }
        sleep(1);
    }
    close(listen_sock);
    printf("连接结束\n");
    return 0;


}




int init_server(int _port,const char* _ip)//非阻塞
{
    fd_set rfds, wfds;
    int sock = socket(AF_INET,SOCK_STREAM,0);
    //调用socket()创建一个套接字用来通讯
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
    //调用bind()进行绑定这个文件描述符


    
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
    printf("bind成功,socket:%d\n",sock);
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sock, &rfds);
    FD_SET(sock, &wfds);
    if(select(sock+1,&rfds,&wfds,NULL,NULL)>0)
        if(listen(sock, 5) < 0) //允许连接的最大数量为5
        {
            perror("listen");
            exit(3);
        }
    printf("listen成功,socket:%d\n",sock);

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