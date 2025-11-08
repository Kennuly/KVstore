#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <getopt.h>

#define TIME_SUB_MS(tv1, tv2) ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)

#define MAX_MAS_LENGTH 512

int connect_tcpserver(const char *ip, unsigned short port)
{

    int connfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in tcpserver_addr;
    memset(&tcpserver_addr, 0, sizeof(struct sockaddr_in));

    tcpserver_addr.sin_family = AF_INET;
    tcpserver_addr.sin_addr.s_addr = inet_addr(ip);
    tcpserver_addr.sin_port = htons(port);

    int ret = connect(connfd, (struct sockaddr *)&tcpserver_addr, sizeof(struct sockaddr_in));
    if (ret)
    {
        perror("connect");
        return -1;
    }

    return connfd;
}

int send_msg(int fd, char *msg, int len)
{
    int res = send(fd, msg, len, 0);
    if (res < 0)
    {
        perror("send");
        exit(1);
    }
    return res;
}

int recv_msg(int fd, char *msg, int len)
{
    int res = recv(fd, msg, len, 0);
    if (res < 0)
    {
        perror("recv");
        exit(1);
    }
    return res;
}
void equals(char *pattern, char *result, char *casename)
{
    /* if (strcmp(pattern, result) == 0)
    {
        std::cout << "==> PASS --> " << casename << std::endl;
    }
    else
    {
        std::cout << "==> FAILED --> " << casename << std::endl;
    } */
}
//"SET name king"   "SUCCESS"
void test_case(int fd, char *msg, char *pattern, char *casename)
{
    if (!msg || !pattern || !casename)
        exit(1);
    send_msg(fd, msg, strlen(msg));
    char result[MAX_MAS_LENGTH] = {0};
    recv_msg(fd, result, MAX_MAS_LENGTH);
    equals(result, pattern, casename);
}

void arrar_testcase10W(int fd)
{
    int i = 0, count = 100000;
    for (i = 0; i < count; i++)
    {
        test_case(fd, "SET Name King", "SUCCESS", "SETCase");
        test_case(fd, "GET Name", "King", "GETCase");
        test_case(fd, "MOD Name Darren", "SUCCESS", "MODCase");
        test_case(fd, "GET Name", "Darren", "GETCase");
        test_case(fd, "DEL Name", "SUCCESS", "DELCase");
        test_case(fd, "GET Name", "NO EXIST", "GETCase");
    }
}

void arrar_testcase(int fd)
{
    test_case(fd, "SET Name King", "SUCCESS", "SETCase");
    test_case(fd, "GET Name", "King", "GETCase");
    test_case(fd, "MOD Name Darren", "SUCCESS", "MODCase");
    test_case(fd, "GET Name", "Darren", "GETCase");
    test_case(fd, "DEL Name", "SUCCESS", "DELCase");
    test_case(fd, "GET Name", "NO EXIST", "GETCase");
}
void rbtree_testcase(int fd)
{
    test_case(fd, "SET Name King", "SUCCESS", "SETCase");
    test_case(fd, "GET Name", "King", "GETCase");
    test_case(fd, "MOD Name Darren", "SUCCESS", "MODCase");
    test_case(fd, "GET Name", "Darren", "GETCase");
    test_case(fd, "DEL Name", "SUCCESS", "DELCase");
    test_case(fd, "GET Name", "NO EXIST", "GETCase");
}
void rbtree_testcase10W(int fd)
{
    int i = 0, count = 100000;
    for (i = 0; i < count; i++)
    {
        test_case(fd, "RSET Name King", "SUCCESS", "SETCase");
        test_case(fd, "RGET Name", "King", "GETCase");
        test_case(fd, "RMOD Name Darren", "SUCCESS", "MODCase");
        test_case(fd, "RGET Name", "Darren", "GETCase");
        test_case(fd, "RDEL Name", "SUCCESS", "DELCase");
        test_case(fd, "RGET Name", "NO EXIST", "GETCase");
    }
}
// ./Taskcase_Client -s 192.168.20.128 -p 1234 -m 1
int main(int argc, char *argv[])
{

    int ret = 0;

    char ip[16] = {0};
    int port = 0;
    int mode = 2;

    int opt;
    while ((opt = getopt(argc, argv, "s:p:m:?")) != -1)
    {

        switch (opt)
        {

        case 's':
            strcpy(ip, optarg);
            break;

        case 'p':
            port = atoi(optarg);
            break;

        case 'm':

            mode = atoi(optarg);
            break;
        default:
            return -1;
        }
    }
    int connfd = connect_tcpserver(ip, port);
    if (mode & 0x01)
    {
        struct timeval tv_begin;
        gettimeofday(&tv_begin, nullptr);
        // arrar_testcase(connfd);
        arrar_testcase10W(connfd);
        struct timeval tv_end;
        gettimeofday(&tv_end, nullptr);
        int time_used = TIME_SUB_MS(tv_end, tv_begin);
        std::cout << "time_used:" << time_used << "qps:" << 1000000 * 1000 / time_used << std::endl;
    }
    if (mode & 0x02)
    {
        struct timeval tv_begin;
        gettimeofday(&tv_begin, nullptr);
        // arrar_testcase(connfd);
        rbtree_testcase10W(connfd);
        struct timeval tv_end;
        gettimeofday(&tv_end, nullptr);
        int time_used = TIME_SUB_MS(tv_end, tv_begin);
        std::cout << "time_used:" << time_used << "qps:" << 1000000 * 1000 / time_used << std::endl;
    }
}