#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <memory.h>

#define ARGS_CHECK(lh, rh)                          \
    do                                              \
    {                                               \
        if (lh != rh)                               \
        {                                           \
            std::cerr << "arcg error" << std::endl; \
            return -1;                              \
        }                                           \
    } while (0);
#define ERROR_CHECK(cond, msg, retval) \
    do                                 \
    {                                  \
        if (cond == -1)                \
        {                              \
            perror("msg");             \
            return retval;             \
        }                              \
    }while(0);

int main(int argc, char *argv[])
{
    //  ./client 192.168.20.135 1234
    ARGS_CHECK(argc, 3);
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);   //ipv4 , tcp
    ERROR_CHECK(sockFd, "socket", -1);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = connect(sockFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    ERROR_CHECK(ret, "connect", -1);

    //select监听
    fd_set rdset;
    char buf[4096] = {0};
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockFd, &rdset);
        select(std::max(sockFd, STDIN_FILENO) + 1, &rdset, nullptr, nullptr, nullptr);
        if(FD_ISSET(STDIN_FILENO, &rdset))
        {
            bzero(buf, sizeof(buf));    //清理
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            if(ret == 0)
                break;
            send(sockFd, buf, strlen(buf), 0);
        }
        if(FD_ISSET(sockFd, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = recv(sockFd, buf, sizeof(buf), 0);
            if(ret == 0)
            {
                break;
            }
            puts(buf);
        }

    }
    close(sockFd);
}