//socket udp 服务端
//包名
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream> // cin,cout等
#include <iomanip>  // setw等
#include <cstdlib>
#include <mysql.h> // mysql特有
#include <iostream>
using namespace std;

const int BUFF_LEN = 200;
#define AWAKE_COMMAND 15

struct package
{
    char Id[15];    //15位设备ID
    char Divide;    //是否分包 0：不分包 其他：分包的标示
    char Seq;       //第几个包
    char Len;       //包长度
    char Type;      //包类型
    char Info[100]; //属性
};

struct machine
{
    char id[15];
    unsigned long addr_ip;
    unsigned short int addr_port;
    int type;
    int last_ack;
};

class NodeMcuServer
{
  public:
    void save_machince_info(struct machine info, struct package pack);
    void do_package_command(struct package info);
    void react_to_package(char *buf, int count, struct package &pak);
    void send_ack(struct package pak);
    void init_server();
    void MainTask();
    void init_mysql();
    int get_machine_info(struct machine *info, struct package pack) void print(const char *info)
    {
        printf(info);
    }

  protected:
    int sockfd;
    MYSQL *mysql;
};

void NodeMcuServer::init_mysql()
{

    /* 初始化 mysql 变量，失败返回NULL */
    if ((mysql = mysql_init(NULL)) == NULL)
    {
        cout << "mysql_init failed" << endl;
        exit(-1);
    }

    /* 连接数据库，失败返回NULL
       1、mysqld没运行
       2、没有指定名称的数据库存在 */
    if (mysql_real_connect(mysql, "localhost", "root", "yyy19980425", "NodeMcu", 0, NULL, 0) == NULL)
    {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        exit(-1);
    }

    /* 设置字符集，否则读出的字符乱码，即使/etc/my.cnf中设置也不行 */
    mysql_set_character_set(mysql, "gbk");

    print("-----数据库连接成功-----\n");
}

int NodeMcuServer::get_machine_info(struct machine *info, struct package pack)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    string query = "select * from Machine_base where Machine_Id =";
    query += pack.Id;
    query += ";";
    if (mysql_query(mysql, query.data()))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return;
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return;
    }
    /* 打印当前查询到的记录的数量 */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;
    if ((int)mysql_num_rows(result) == 0)
    {
        return -1;
    }
    else
    {
        row = mysql_fetch_row(result);
        memcpy(info.id, row[0], 15);
        info.addr_ip = atoi(row[1]);
        info.addr_port = atoi(row[2]);
        info.type = atoi(row[3]);
        info.last_ack = atoi(row[4]);
    }
    /* 释放result */
    mysql_free_result(result);
    return 0;
}

void NodeMcuServer::save_machince_info(struct machine info, struct package pack)
{
    char select_user[255];
    MYSQL_RES *result;
    sprintf(select_user, "select * from user where UserName='%s'", body.userName);
}

void NodeMcuServer::do_package_command(struct package info)
{
}

void NodeMcuServer::react_to_package(char *buf, int count, struct package &pak)
{
    memcpy(&pak, buf, count);
}

void NodeMcuServer::send_ack(struct sockaddr_in addr, struct package pack)
{
}

void NodeMcuServer::init_server()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
        exit(-1);
    }
    print("-----服务器开始运行-----\n");
}

void NodeMcuServer::MainTask()
{
    print("-----开始监听端口-----\n");
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
            continue;
        }
        printf("server recv: %s\n", buf);
        printf("recv from: %s:%d\n", inet_ntoa(cli.sin_addr), cli.sin_port);
        memcpy((char *)&temp, buf, count);
        if (buf[0] == 'N')
        {
            //当是物联网设备发来的消息时
            struct machine now_machine;
            this->react_to_package(buf, count, temp);
            int flag = this->get_machine_info(&now_machine, temp);
            if (flag == -1)
                this->add_machine_info(&now_machine, temp);
            else
            {
                if (now_machine.last_ack != temp.Divide)
                {
                    //收到了过时的消息，发送ack什么都不做
                    this->send_ack(cli, temp);
                }
                else
                {
                    //收到了新的消息
                    this->save_machince_info(temp); //保存设备最新地址
                    this->do_package_command(temp); //应对包命令
                    this->send_ack(cli, temp);      //发送ack
                }
            }
        }
        else
        {
            //当是app发来的消息时
        }
    }
    close(sockfd);
}

int main()
{
    //---------------------初始化完成------------------------------
    //任务流程
    //1:监听udp包
    //2:分析udp包
    //  回复ack包
    //  若是设备id是初次连接，保存设备id
    //  若设备id已经被保存，更新设备连接时间

    //我感觉阻塞没有问题（有可能以后会改叭）

    NodeMcuServer server;
    server.init_server();
    server.init_mysql();

    struct package temp;
    struct machine m;
    memcpy(temp.Id, "123456789012345", 15);
    server.get_machine_info(&m, temp);
    return 0;
}
