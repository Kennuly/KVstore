#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Reactor.h"
#include "set.h"
#include <string.h>
#include "KVstore.h"
int epfd = -1;
struct Conn_item conlist[1024] = {0};
void set_event(int fd, int event, int flag)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    (flag == 1) ? epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) : epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

int accept_cb(int fd)
{
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept(fd, (struct sockaddr*)&clientaddr, &len);
    if (clientfd < 0)
    {
        return -1;
    }
    set_event(clientfd, EPOLLIN, 1);

    conlist[clientfd].fd_ = clientfd;
    conlist[clientfd].rlen_ = 0;
    conlist[clientfd].wlen_ = 0;
    memset(conlist[clientfd].rbuffer_, 0, BUFFER_LEN);
    memset(conlist[clientfd].wbuffer_, 0, BUFFER_LEN);

    conlist[clientfd].recv_t.recv_callback = recv_cb;
    conlist[clientfd].send_callback = send_cb;
    return clientfd;
}
int recv_cb(int fd)
{
    
    char *buf = conlist[fd].rbuffer_;
    memset(buf, 0, BUFFER_LEN);
    int idx = conlist[fd].rlen_;
    int count = recv(fd, buf, BUFFER_LEN, 0);
    if (count <= 0)
    {
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
        return -1;
    }
    conlist[fd].rlen_ = count;

    /* memcpy(conlist[fd].wbuffer_, conlist[fd].rbuffer_, conlist[fd].rlen_);
    conlist[fd].wlen_ = conlist[fd].rlen_; */
    KVstore_request(&conlist[fd]);
    conlist[fd].wlen_ = strlen(conlist[fd].wbuffer_);
    
    set_event(fd, EPOLLOUT, 0);
    return count;
}

int send_cb(int fd)
{
    char *buf = conlist[fd].wbuffer_;
    int idx = conlist[fd].wlen_;
    int count = send(fd, buf, idx, 0);
    set_event(fd, EPOLLIN, 0);
    return count;
}

int epoll_entry()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, "socket", -1);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("192.168.20.128");
    int ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, "bind", -1);
    ret = listen(sockfd, 100);
    ERROR_CHECK(ret, "listen", -1);
    epfd = epoll_create(1);
    set_event(sockfd, EPOLLIN, 1);
    conlist[sockfd].fd_ = sockfd;
    conlist[sockfd].recv_t.accept_callback = accept_cb;
    struct epoll_event event[1024] = {0};
    while (1)
    {
        int nready = epoll_wait(epfd, event, 1024, -1);
        for (int i = 0; i < nready; i++)
        {
            int confd = event[i].data.fd;
            if (event[i].events & EPOLLIN)
            {
                int count = conlist[confd].recv_t.recv_callback(confd);
            }
            else if (event[i].events & EPOLLOUT)
            {
                int count = conlist[confd].send_callback(confd);
            }
        }
    }
    return 0;
}
