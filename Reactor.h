#ifndef __REACTOR_H__
#define __REACTOR_H__

#define BUFFER_LEN 1024

typedef int(*RCALLBACK)(int);
struct Conn_item
{
    int fd_;
    char rbuffer_[BUFFER_LEN];
    int rlen_;

    char wbuffer_[BUFFER_LEN];
    int wlen_;

    union 
    {
        RCALLBACK accept_callback;
        RCALLBACK recv_callback;        
    }recv_t;
    RCALLBACK send_callback;
};

extern Conn_item conlist[1024];
int accept_cb(int fd);
int recv_cb(int fd);
int send_cb(int fd);

//flag   1 --> add      0 --> mod
void set_event(int fd, int event, int flag);
int epoll_entry();


#endif