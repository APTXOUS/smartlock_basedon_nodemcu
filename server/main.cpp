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

#define SMART_LOCK 99
#define ACK_ID "!!!!!!!!!!!!!!!"
#define ACK_PACKAGE 0
#define PACKAGE_MIN 20

#define OPEN_COMMAND 12 //����
#define LOCK_COMMAND 13 //
#define LOCK_COMFIRM 14 //

#define SOUND_COMMAND 15 //
#define SOUND_CONFIRM 16

#define START_LIGHTING 21
#define STOP_LIGHTING 22

struct package
{
    char IfM;       //�ǿͻ��˻���app
    char Id[15];    //15λ�豸ID
    char Divide;    //���ı�ʾ
    char Seq;       //Ϊ0��ʾ���ְ�������Ϊ0��ʾΪ�ڼ�����
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
    int save_machince_info(struct machine *info, struct package pack);
    void do_package_command(struct package info);
    void react_to_package(char *buf, int count, struct package &pak);
    void send_ack(struct sockaddr_in addr, struct package pack);
    void init_server();
    void MainTask();
    void init_mysql();
    int get_machine_info(struct machine *info, struct package pack);
    int add_machine_info(struct machine *info, struct package pack);
    int get_machine_info(struct machine *info);
    int get_machine_finger_machine(struct machine *info, struct machine *finger);
    int get_finger_machine_finger(struct machine *finger, struct machine *info);
    int get_sound_machine_sound(struct machine *sound, struct machine *info);
    int get_machine_sound_machine(struct machine *info, struct machine *sound);
    int set_machine_status(int id, const char * status);
    void print(const char *info)
    {
        printf(info);
    }

protected:
    int sockfd;
    MYSQL *mysql;
    struct package ACK;
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
    if (mysql_real_connect(mysql, "localhost", "root", "114514", "NodeMcu", 0, NULL, 0) == NULL)
    {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        exit(-1);
    }

    /* �����ַ���������������ַ����룬��ʹ/etc/my.cnf������Ҳ���� */
    mysql_set_character_set(mysql, "gbk");

    print("-----���ݿ����ӳɹ�-----\n");
}

int NodeMcuServer::add_machine_info(struct machine *info, struct package pack)
{
    char select_user[255];
    MYSQL_RES *result;
    sprintf(select_user, "INSERT INTO Machine_base (Machine_id,Machine_Ip,Machine_Port,Machine_Type,Machine_Lastack) VALUES (\"%s\",%d,%d,%d,%d);", info->id, info->addr_ip, info->addr_port, info->type, info->last_ack);
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }
    /* ��ӡ��ǰ��ѯ���ļ�¼������ */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;
    mysql_free_result(result);
    return 0;
}

int NodeMcuServer::get_machine_info(struct machine *info, struct package pack)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char select_user[255];
    //sprintf(select_user, "select * from Machine_base where Machine_id = \"%s\"",pack.Id);
    sprintf(select_user, "select * from Machine_base where Machine_id = \"XXXXXXXXXXXXXXX\"");
    memcpy(select_user + 47, pack.Id, 15);
    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
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
        memcpy(info->id, row[0], 15);
        info->addr_ip = atoi(row[1]);
        info->addr_port = atoi(row[2]);
        info->type = atoi(row[3]);
        info->last_ack = atoi(row[4]);
    }
    /* �ͷ�result */
    mysql_free_result(result);
    return 0;
}

int NodeMcuServer::get_machine_info(struct machine *info)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char select_user[255];
    //sprintf(select_user, "select * from Machine_base where Machine_id = \"%s\"",pack.Id);
    sprintf(select_user, "select * from Machine_base where Machine_id = \"XXXXXXXXXXXXXXX\"");
    memcpy(select_user + 47, info->id, 15);
    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
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
        memcpy(info->id, row[0], 15);
        info->addr_ip = atoi(row[1]);
        info->addr_port = atoi(row[2]);
        info->type = atoi(row[3]);
        info->last_ack = atoi(row[4]);
    }
    /* �ͷ�result */
    mysql_free_result(result);
    return 0;
}

int NodeMcuServer::get_sound_machine_sound(struct machine *sound, struct machine *info)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char select_user[255];
    sprintf(select_user, "select * from Machine_sound where MS_Machine_sound = \"XXXXXXXXXXXXXXX\"");
    memcpy(select_user + 54, sound->id, 15);
    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
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
        memcpy(info->id, row[0], 15);
        get_machine_info(info);
    }
    /* �ͷ�result */
    mysql_free_result(result);
    return 0;
}
int NodeMcuServer::get_finger_machine_finger(struct machine *finger, struct machine *info)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char select_user[255];
    sprintf(select_user, "select * from Machine_finger where MF_Machine_finger = \"XXXXXXXXXXXXXXX\"");
    memcpy(select_user + 56, finger->id, 15);
    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
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
        memcpy(info->id, row[0], 15);
        get_machine_info(info);
    }
    /* �ͷ�result */
    mysql_free_result(result);
    return 0;
}
int NodeMcuServer::get_machine_sound_machine(struct machine *info, struct machine *sound)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char select_user[255];
    sprintf(select_user, "select * from Machine_sound where MS_Machine_id = \"XXXXXXXXXXXXXXX\"");
    memcpy(select_user + 51, info->id, 15);
    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
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
        memcpy(sound->id, row[1], 15);
        get_machine_info(sound);
    }
    /* �ͷ�result */
    mysql_free_result(result);
    return 0;
}

int NodeMcuServer::get_machine_finger_machine(struct machine *info, struct machine *finger)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char select_user[255];
    sprintf(select_user, "select * from Machine_finger where MF_Machine_id = \"XXXXXXXXXXXXXXX\"");
    memcpy(select_user + 52, info->id, 15);
    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
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
        memcpy(finger->id, row[1], 15);
        get_machine_info(finger);
    }
    /* �ͷ�result */
    mysql_free_result(result);
    return 0;
}


int NodeMcuServer::set_machine_status(int id, const char * status)
{
	char select_user[255];
	sprintf(select_user, "update OnenetMachine set Status=\"%s\" where On_Machine_id=%d", status, id);
	MYSQL_RES *result;
	if (mysql_query(mysql, select_user))
	{
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return (-1);
	}
	/* ��ӡ��ǰ��ѯ���ļ�¼������ */
	cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;
	return 0;
}

int NodeMcuServer::save_machince_info(struct machine *info, struct package pack)
{
    char select_user[255];
    char where_use[255] = "where Machine_id=\"YYYYYYYYYYYYYYY\"";
    memcpy(where_use + 18, info->id, 15);
    MYSQL_RES *result;
    // sprintf(select_user, "select * from user where UserName='%s'", body.userName);

    sprintf(select_user, "update Machine_base set Machine_Ip= %d,Machine_Port=%d,Machine_Type=%d,Machine_Lastack=%d ", info->addr_ip, info->addr_port, info->type, info->last_ack);
    strcat(select_user, where_use);

    cout << select_user << endl;
    if (mysql_query(mysql, select_user))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return (-1);
    }
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return (-1);
    }
    /* ��ӡ��ǰ��ѯ���ļ�¼������ */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;
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
    socklen_t len = sizeof(sockaddr_in);
    this->ACK.Divide = pack.Divide;
    sendto(sockfd, (char *)&ACK, PACKAGE_MIN, 0, (struct sockaddr *)&addr, len);
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
    memcpy(this->ACK.Id, ACK_ID, 15);
    this->ACK.IfM = 'S';
    this->ACK.Divide = 0;
    this->ACK.Seq = 0;
    this->ACK.Len = 20;
    this->ACK.Type = ACK_PACKAGE;
}

void showPackage(struct package pack)
{
    cout << pack.IfM << endl;
    cout << pack.Id << endl;
    cout << (int)pack.Divide << endl;
    cout << (int)pack.Seq << endl;
    cout << (int)pack.Len << endl;
    cout << (int)pack.Type << endl;
    cout << pack.Info << endl;
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
        memset(buf, 0, sizeof(buf));
        struct package temp;
        count = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli, &len);
        if (count == -1)
        {
            printf("recieve data fail!\n");
            continue;
        }
        printf("server recv:length:%d, data: %s\n", count, buf);
        printf("recv from: %s:%d\n", inet_ntoa(cli.sin_addr), cli.sin_port);
        memcpy((char *)&temp, buf, count);
        if (buf[0] == 'N')
        {
            //�����������豸��������Ϣʱ
            struct machine now_machine;
            this->react_to_package(buf, count, temp);
            int flag = this->get_machine_info(&now_machine, temp);
            if (flag == -1)
            {
                now_machine.addr_ip = cli.sin_addr.s_addr;
                now_machine.addr_port = cli.sin_port;
                memcpy(now_machine.id, temp.Id, 15);
                now_machine.last_ack = (temp.Divide + 1) % 255;
                now_machine.type = SMART_LOCK;
                this->add_machine_info(&now_machine, temp);
            }
            else
            {
                //�յ����µ���Ϣ
                now_machine.addr_ip = cli.sin_addr.s_addr;
                now_machine.addr_port = cli.sin_port;
                memcpy(now_machine.id, temp.Id, 15);
                now_machine.last_ack = (temp.Divide + 1) % 255;
                now_machine.type = SMART_LOCK;
                this->save_machince_info(&now_machine, temp); //�����豸���µ�ַ
                if (temp.Type == 14)
                {
                    struct machine info;
                    int flag = this->get_finger_machine_finger(&now_machine, &info);
                    struct sockaddr_in send_machine;
                    send_machine = cli;
                    send_machine.sin_addr.s_addr = info.addr_ip;
                    send_machine.sin_port = info.addr_port;
                    char openthemachine[20] = "STHISISSERVERMEG";
                    openthemachine[16] = 0;
                    openthemachine[17] = 0;
                    openthemachine[18] = 20;
                    openthemachine[19] = OPEN_COMMAND; //
                    cout << "send:" << sendto(sockfd, openthemachine, 20, 0, (struct sockaddr *)&send_machine, len) << endl;
                }
                if (temp.Type == 16)
                {
                    struct machine info;
                    int flag = this->get_sound_machine_sound(&now_machine, &info);
                    struct sockaddr_in send_machine;
                    send_machine = cli;
                    send_machine.sin_addr.s_addr = info.addr_ip;
                    send_machine.sin_port = info.addr_port;
                    char openthemachine[20] = "STHISISSERVERMEG";
                    openthemachine[16] = 0;
                    openthemachine[17] = 0;
                    openthemachine[18] = 20;
                    openthemachine[19] = OPEN_COMMAND; //
                    cout << "----------------------------------------" << endl;
                    cout << "send:" << sendto(sockfd, openthemachine, 20, 0, (struct sockaddr *)&send_machine, len) << endl;
                    cout << info.id << endl;
                }
				// �������ߴ�������⵽�˵�ʱ��
				// ����ͱȽϸ���
				// ��˼��һ��
				// emmmm
				// ��������
				// �����ǲ����ݿ⣬���Ҷ�Ӧ���豸
				// Ȼ�����ݿ��н�һ���豸λ�ü�ΪON RUNNING
				// ����һ������ÿ��ѭ�����һ���Ƿ���ON RUNNINg���豸
				// �еĻ�������EDP�������豸��������Ӧ��Ҫ����ͨ����
				// �豸����һ����Ƭ���������ݿ�
				// �豸����һ����Ƶ���������ݿ�
				// ǰ�˽���һ���������豸λON RUNING��js����һ��ģ̬��
				// �����ת��һ�����棬��ѯ��ʾͼƬ
				// �����Ƶ���û������Ƶ
				// ��һ����ť
				if (temp.Type == 17) 
				{
                    cout<<"#############################################"<<endl;
					set_machine_status(527923817,"FINDPEOPLE");

					char openthemachine[20] = "STHISISSERVERMEG";
					openthemachine[16] = 0;
					openthemachine[17] = 0;
					openthemachine[18] = 20;
					openthemachine[19] = STOP_LIGHTING; //
					cout << "----------------------------------------" << endl;

					cout << "send:" << sendto(sockfd, openthemachine, 20, 0, (struct sockaddr *)&cli, len) << endl;
                    cout<<"#############################################"<<endl;
				}
                //this->do_package_command(temp);               //Ӧ�԰�����
                this->send_ack(cli, temp); //����ack
            }
        }
        else if (buf[0] == 'M') //��������
        {
            socklen_t len = sizeof(cli);
            cout << "send:" << sendto(sockfd, buf, count, 0, (struct sockaddr *)&cli, len) << endl;
        }
        else
        {
            //����app��������Ϣʱ
            this->react_to_package(buf, count, temp);
            showPackage(temp);

            if (temp.Type == 10) //�ֻ���֤��
            {
                char userid[20];
                memcpy(userid, temp.Info, 20);
                struct machine info;
                memcpy(info.id, temp.Info + 20, 15);
                int flag = this->get_machine_info(&info);
                struct sockaddr_in send_machine;
                send_machine = cli;
                send_machine.sin_addr.s_addr = info.addr_ip;
                send_machine.sin_port = info.addr_port;
                char openthemachine[20] = "STHISISSERVERMEG";
                openthemachine[16] = 0;
                openthemachine[17] = 0;
                openthemachine[18] = 20;
                openthemachine[19] = OPEN_COMMAND; //
                cout << "send:" << sendto(sockfd, openthemachine, 20, 0, (struct sockaddr *)&send_machine, len) << endl;
            }

            if (temp.Type == 13) //�ֻ���֤��
            {
                char userid[20];
                memcpy(userid, temp.Info, 20);
                struct machine info;
                memcpy(info.id, temp.Info + 20, 15);
                struct machine finger;
                int flag = get_machine_finger_machine(&info, &finger);

                struct sockaddr_in send_machine;
                send_machine = cli;
                send_machine.sin_addr.s_addr = finger.addr_ip;
                send_machine.sin_port = finger.addr_port;
                char openthemachine[20] = "STHISISSERVERMEG";
                openthemachine[16] = 0;
                openthemachine[17] = 0;
                openthemachine[18] = 20;
                openthemachine[19] = LOCK_COMMAND; //
                cout << "send:" << sendto(sockfd, openthemachine, 20, 0, (struct sockaddr *)&send_machine, len) << endl;
            }
            if (temp.Type == 15) //������֤��
            {
                char userid[20];
                memcpy(userid, temp.Info, 20);
                struct machine info;
                memcpy(info.id, temp.Info + 20, 15);
                struct machine sound;
                int flag = get_machine_sound_machine(&info, &sound);
                struct sockaddr_in send_machine;
                send_machine = cli;
                send_machine.sin_addr.s_addr = sound.addr_ip;
                send_machine.sin_port = sound.addr_port;
                char openthemachine[23] = "STHISISSERVERMEG";
                openthemachine[16] = 0;
                openthemachine[17] = 0;
                openthemachine[18] = 20;
                openthemachine[19] = SOUND_COMMAND; //
                memcpy(openthemachine + 20, "bbb", 3);
                cout << "send:" << sendto(sockfd, openthemachine, 23, 0, (struct sockaddr *)&send_machine, len) << endl;
            }
            if (temp.Type == 21) //����֤��
            {
                struct machine info;
                memcpy(info.id, temp.Info,15);
                int flag = get_machine_info(&info);
                struct sockaddr_in send_machine;
                send_machine = cli;
                send_machine.sin_addr.s_addr = info.addr_ip;
                send_machine.sin_port = info.addr_port;
                char openthemachine[23] = "STHISISSERVERMEG";
                openthemachine[16] = 0;
                openthemachine[17] = 0;
                openthemachine[18] = 21;
                openthemachine[19] = START_LIGHTING; //
                cout << "send:" << sendto(sockfd, openthemachine, 20, 0, (struct sockaddr *)&send_machine, len) << endl;
            }
            // app ���ļ������ܰ���
            // ԤԼ����
            // ��ԤԼʱ��
            // ÿ���СʱΪһ������
            // ���߷�������ҪԤԼ���������
            // �������ݿ⣬����
            // �������ݸ�NodeMcu ����
            // 1.NodeMcu�������ݣ����ʱ��
            //   ��ʱ����������ط���Ϣ��
            // 2.����app��ѯ���ݿⷢ�ֵ�ʱ����
            //   ���͸�NodeMcu����

            // nodemcu������֤ģʽ��
            // 1.ָ�Ʊ�����֤
            // 2.������֤
            // 3.appԶ����֤

            // ����֪���Ͳ�д����ģʽ��
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
    server.MainTask();
    return 0;
}
