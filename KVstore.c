#include "Reactor.h"
#include "set.h"
#include <stdio.h>
#include "KVstore.h"
#include <string.h>
#include <assert.h>
#include "nty.h"
#include <stdlib.h>
#include "KVstore_array.h"
#include "rbtree.h"

const char *commands[] = {
    "SET", "GET", "DEL", "MOD", "COUNT",
    "RSET", "RGET", "RDEL", "RMOD", "RCOUNT"};

int KVstore_split_token(char *msg, char **tokens)
{
    if (msg == NULL || tokens == NULL)
    {
        return -1;
    }
    // 去掉 \n
    char *p = strchr(msg, '\n');
    if (p)
        *p = '\0';

    int idx = 0;
    char *token = strtok(msg, " ");
    while (token != NULL)
    {
        tokens[idx++] = token;
        token = strtok(NULL, " ");
    }
    return idx;
}

int kvstore_parser_protocol(Conn_item *item, char **tokens, int count)
{
    if (item == NULL || tokens[0] == NULL || count == 0)
    {
        return -1;
    }
    int cmd;
    for (cmd = KVS_CMD_START; cmd < KVS_COM_SIZE; cmd++)
    {
        if (strcmp(commands[cmd], tokens[0]) == 0)
        {
            break;
        }
    }
    char *msg = item->wbuffer_;
    memset(msg, 0, BUFFER_LEN);
    int res = 1;
    switch (cmd)
    {
    case KVS_COM_SET:
        // std::cout << "set" << std::endl;
        LOG("SET");
        res = kvstore_array_set(tokens[1], tokens[2]);
        if (!res)
        {
            snprintf(msg, BUFFER_LEN, "SUCCESS");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "FAILED");
        }

        break;
    case KVS_COM_GET:
        // std::cout << "get" << std::endl;
        LOG("GET");
        char *value = kvstore_array_get(tokens[1]);
        if (value)
        {
            snprintf(msg, BUFFER_LEN, "%s", value);
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }

        break;
    case KVS_COM_DEL:
        // std::cout << "del" << std::endl;
        // printf("del");
        LOG("DEL");
        res = kvstore_array_del(tokens[1]);
        if (res < 0)
        {
            snprintf(msg, BUFFER_LEN, "ERROR");
        }
        else if (res == 1)
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "SUCCESS");
        }
        break;
    case KVS_COM_MOD:
        // std::cout << "mod" << std::endl;
        // puts("mod");
        LOG("MOD");
        res = kvstore_array_mod(tokens[1], tokens[2]);
        if (res < 0)
        {
            snprintf(msg, BUFFER_LEN, "ERROR");
        }
        else if (res == 1)
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "SUCCESS");
        }
        break;
    case KVS_COM_COUNT:
        LOG("COUNT");
        res = kvstore_array_count();
        if (res == 0)
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "%d", res);
        }
        break;
    case KVS_COM_RSET:
        // printf("rset");
        LOG("rset");
        res = kvs_rbtree_set(tokens[1], tokens[2]);
        if (!res)
        {
            snprintf(msg, BUFFER_LEN, "SUCCESS");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "FAILED");
        }
        break;

    case KVS_COM_RGET:
        // printf("rget");
        LOG("rget");
        char *rvalue = kvs_rbtree_get(tokens[1]);
        if (rvalue)
        {
            snprintf(msg, BUFFER_LEN, "%s", rvalue);
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }

        break;

    case KVS_COM_RDEL:
        // printf("rdel");
        LOG("rdel");
        res = kvs_rbtree_delete(tokens[1]);
        if (res < 0)
        {
            snprintf(msg, BUFFER_LEN, "ERROR");
        }
        else if (res == 1)
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "SUCCESS");
        }
        break;

    case KVS_COM_RMOD:
        // printf("rmod");
        LOG("rmod");
        res = kvs_rbtree_modify(tokens[1], tokens[2]);
        if (res < 0)
        {
            snprintf(msg, BUFFER_LEN, "ERROR");
        }
        else if (res == 1)
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "SUCCESS");
        }
        break;
    case KVS_COM_RCOUNT:
        LOG("RCOUNT");
        res = kvs_rbtree_count();
        if (res == 0)
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "%d", res);
        }
        break;

    default:
        // std::cout << commands[cmd] << std::endl;
        puts(commands[cmd]);
        assert(0);
    }
    return 0;
}
// Set Key Value
void KVstore_request(Conn_item *item)
{
    // std::cout << "recv: " << item->rbuffer_ << std::endl;
    LOG("recv:%s", item->rbuffer_);
    char *msg = item->rbuffer_;
    char *tokens[KVSTORE_MAX_TOKENS];
    int count = KVstore_split_token(msg, tokens);

    for (int idx = 0; idx < count; idx++)
    {
        // std::cout << "dix:" << tokens[idx] << std::endl;
        LOG("dix:%s\n", tokens[idx]);
    }

    kvstore_parser_protocol(item, tokens, count);
    // memset(item->rbuffer_, 0, BUFFER_LEN);
}

void *kvstore_malloc(size_t size)
{
    return malloc(size);
}
void kvstore_free(void *ptr)
{
    return free(ptr);
}

int init_kvengine()
{
#if ENABLE_ARRAY_KVENGINE

#endif

#if ENABLE_RBTREE_KVENGINE
    kvstore_rbtree_create(&Tree);
#endif
}

int main()
{
    kvstore_array_create(&arr);
    init_kvengine();
#if (ENABLE_NETWORK_SELECT == NETWORK_EPOLL)
    epoll_entry();
#elif (ENABLE_NETWORK_SELECT == NETWORK_NTYCO)
    ntyco_entry();

#elif (ENABLE_NETWORK_SELECT == NETWORK_IOURING)

#endif
}