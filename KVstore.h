#ifndef __KVSTORE_H__
#define __KVSTORE_H__
#include "Reactor.h"  
#include <cstddef>

#define KVSTORE_MAX_TOKENS 128
extern const char*commands[];
enum
{
    KVS_CMD_START = 0,
    KVS_COM_SET = KVS_CMD_START,
    KVS_COM_GET,
    KVS_COM_DEL,
    KVS_COM_MOD,
    KVS_COM_COUNT
};

int kvstore_parser_protocol(Conn_item *item, char** tokens, int count);

void KVstore_request(Conn_item *item);
int KVstore_split_token(char *msg, char **tokens);

void* kvstore_malloc(size_t size);
void kvstore_free(void *ptr);


#define ENABLE_ARRAY_KVENGINE   1
#if ENABLE_ARRAY_KVENGINE
struct kvs_array_item
{
    char *key;
    char *value;
};
#define KVS_ARRAY_SIZE  1024
int kvstore_array_set(char* key, char *value);
char* kvs_array_get(char* key);

#endif



#endif
