//test_server.c
//1650254 尤尧寅
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <stddef.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/prctl.h>
#include <sys/times.h>
#include <string.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <sys/file.h>

#include <sys/times.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

#define status int
#define OK 0
#define ERROR -1
#define Father_MAX_Connect 1024
#define Max_BUff 100000

#define OK 0
#define ERROR -1
int Create_Damon();
int init_server(int _port, const char *_ip);
int check_is_integer(const char *source);
int updatebfds(fd_set fds);

int main(int argc, char **argv)
{
    int n=4000;
    printf("要绑定的ip端口为%d\n", 4000);
    int listen_sock = init_server(n, "0.0.0.0"); //初始化
    fd_set rfds, wfds, bfds;
    ;
    fd_set lfds;
    struct sockaddr_in remote;
    socklen_t len = sizeof(struct sockaddr_in);

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&bfds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int book[100];
    memset(book, 0, sizeof(book));
    int sock = 0;
    int flag = 1;
    char buf[Max_BUff];
    while (1)
    {
        FD_ZERO(&lfds);
        FD_SET(listen_sock, &lfds);
        if (select(listen_sock + 1, &lfds, &lfds, NULL, &timeout) > 0)
        {
            sock = accept(listen_sock, (struct sockaddr *)&remote, &len);
            if (sock < 0)
            {
                perror("accept");
                exit(-1);
            }
            else
            {
                printf("get a client, ip:%s, port:%d sock:%d\n", inet_ntoa(remote.sin_addr), ntohs(remote.sin_port), sock);
                book[sock] = 1;
                FD_SET(sock, &bfds);
                if (ioctl(sock, FIONBIO, &flag) < 0)
                {
                    perror("ioctl");
                    exit(-1);
                }
            }
        }
        ssize_t _s;
        int count = updatebfds(bfds);
        rfds = bfds;
        wfds = bfds;
        switch (select(count + 1, &rfds, &wfds, NULL, &timeout))
        {
        case -1:
        {
            //printf("sock:%d \n",sock);
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                break;
            else
            {
                perror("select");
                break;
            }
        }
        case 0:
        {
            break;
        }
        default:
        {
            memset(buf, 0, sizeof(buf));
            int i;
            for (i = 0; i <= count; i++)
            {
                if (FD_ISSET(i, &bfds))
                {
                    int wflag = 0;
                    if (FD_ISSET(i, &rfds))
                    {

                        _s = read(i, buf, Max_BUff);
                        if (_s == 0)
                        {
                            printf("client drop out sock: %d\n", i);
                            close(i);
                            FD_CLR(i, &bfds);
                            book[i] = 0;
                            continue;
                        }
                        else if (_s < 0)
                        {
                            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                                continue;
                            else
                            {
                                printf("client drop out sock: %d\n", i);
                                close(i);
                                FD_CLR(i, &bfds);
                                book[i] = 0;
                                continue;
                            }
                        }
                        else
                        {
                            buf[_s] = 0;
                            wflag = 1;
                        }
                    }
                    if (FD_ISSET(i, &wfds))
                    {
                        if (wflag)
                        {
                            _s = write(i, buf, _s);
                            //需要进行错误处理
                        }
                    }
                }
            }
        }
        }
    }
    return 0;
}

int init_server(int _port, const char *_ip) //非阻塞
{
    fd_set rfds, wfds;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //调用socket()创建一个套接字用来通讯
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    //设置为非阻塞
    int flag = 1;
    if (ioctl(sock, FIONBIO, &flag) < 0)
    {
        perror("ioctl");
        exit(-1);
    }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = inet_addr(_ip);
    socklen_t len = sizeof(local);
    //调用bind()进行绑定这个文件描述符

    if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)))
    {

        perror("setsockopt fail");
    }

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sock, &rfds);
    FD_SET(sock, &wfds);

    if (select(sock + 1, &rfds, &wfds, NULL, NULL) > 0)
        if (bind(sock, (struct sockaddr *)&local, len) < 0)
        {
            perror("bind");
            exit(2);
        }
    printf("bind成功,socket:%d\n", sock);
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sock, &rfds);
    FD_SET(sock, &wfds);
    if (select(sock + 1, &rfds, &wfds, NULL, NULL) > 0)
        if (listen(sock, 10) < 0) //允许连接的最大数量为5
        {
            perror("listen");
            exit(3);
        }
    printf("listen成功,socket:%d\n", sock);

    return sock;
}

int check_is_integer(const char *source)
{
    int n = strlen(source);
    int i;
    for (i = 0; i < n; i++)
        if (source[i] > '9' || source[i] < '0')
            return ERROR;
    return OK;
}

int updatebfds(fd_set fds)
{
    int i;
    int res_maxfd = 0;
    for (i = 0; i < FD_SETSIZE; ++i)
        if (FD_ISSET(i, &fds) && i > res_maxfd)
            res_maxfd = i;
    return res_maxfd;
}