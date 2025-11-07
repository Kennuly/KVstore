#ifndef __KVSTORE_H__
#define __KVSTORE_H__

#include <stddef.h>

struct Conn_item;   // ✅ 仅声明，不需要引入 Reactor.h

#define KVSTORE_MAX_TOKENS 128

#define NETWORK_EPOLL   0
#define NETWORK_NTYCO   1
#define NETWORK_IOURING 2

#define ENABLE_NETWORK_SELECT   NETWORK_NTYCO



#ifdef ENABLE_LOG

#define LOG(_fmt, ...) fprintf(stdout, "[%s:%d]:%s"_fmt, __FILE__, __LINE__, __VAR_ARGS__)
#else
#define LOG(_fmt, ...)
#endif

int ntyco_entry();

extern const char* commands[];

enum
{
    KVS_CMD_START = 0,
    KVS_COM_SET = KVS_CMD_START,
    KVS_COM_GET,
    KVS_COM_DEL,
    KVS_COM_MOD,
    KVS_COM_COUNT
};

int kvstore_parser_protocol(struct Conn_item *item, char** tokens, int count);

void KVstore_request(struct Conn_item *item);
int KVstore_split_token(char *msg, char **tokens);

void* kvstore_malloc(size_t size);
void kvstore_free(void *ptr);




#endif
