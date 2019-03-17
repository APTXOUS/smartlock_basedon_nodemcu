//socket udp �����
//����
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream> // cin,cout��
#include <iomanip>  // setw��
#include <cstdlib>
#include <mysql.h> // mysql����
#include <iostream>
using namespace std;

const int BUFF_LEN = 200;
#define AWAKE_COMMAND 15

struct package
{
    char Id[15];    //15λ�豸ID
    char Divide;    //�Ƿ�ְ� 0�����ְ� �������ְ��ı�ʾ
    char Seq;       //�ڼ�����
    char Len;       //������
    char Type;      //������
    char Info[100]; //����
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

    /* ��ʼ�� mysql ������ʧ�ܷ���NULL */
    if ((mysql = mysql_init(NULL)) == NULL)
    {
        cout << "mysql_init failed" << endl;
        exit(-1);
    }

    /* �������ݿ⣬ʧ�ܷ���NULL
       1��mysqldû����
       2��û��ָ�����Ƶ����ݿ���� */
    if (mysql_real_connect(mysql, "localhost", "root", "yyy19980425", "NodeMcu", 0, NULL, 0) == NULL)
    {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        exit(-1);
    }

    /* �����ַ���������������ַ����룬��ʹ/etc/my.cnf������Ҳ���� */
    mysql_set_character_set(mysql, "gbk");

    print("-----���ݿ����ӳɹ�-----\n");
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
    /* ��ӡ��ǰ��ѯ���ļ�¼������ */
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
    /* �ͷ�result */
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
        exit(-1);
    }
    print("-----��������ʼ����-----\n");
}

void NodeMcuServer::MainTask()
{
    print("-----��ʼ�����˿�-----\n");
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
            //�����������豸��������Ϣʱ
            struct machine now_machine;
            this->react_to_package(buf, count, temp);
            int flag = this->get_machine_info(&now_machine, temp);
            if (flag == -1)
                this->add_machine_info(&now_machine, temp);
            else
            {
                if (now_machine.last_ack != temp.Divide)
                {
                    //�յ��˹�ʱ����Ϣ������ackʲô������
                    this->send_ack(cli, temp);
                }
                else
                {
                    //�յ����µ���Ϣ
                    this->save_machince_info(temp); //�����豸���µ�ַ
                    this->do_package_command(temp); //Ӧ�԰�����
                    this->send_ack(cli, temp);      //����ack
                }
            }
        }
        else
        {
            //����app��������Ϣʱ
        }
    }
    close(sockfd);
}

int main()
{
    //---------------------��ʼ�����------------------------------
    //��������
    //1:����udp��
    //2:����udp��
    //  �ظ�ack��
    //  �����豸id�ǳ������ӣ������豸id
    //  ���豸id�Ѿ������棬�����豸����ʱ��

    //�Ҹо�����û�����⣨�п����Ժ��İȣ�

    NodeMcuServer server;
    server.init_server();
    server.init_mysql();

    struct package temp;
    struct machine m;
    memcpy(temp.Id, "123456789012345", 15);
    server.get_machine_info(&m, temp);
    return 0;
}
