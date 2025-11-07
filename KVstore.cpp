#include "Reactor.h"
#include "set.h"
#include <iostream>
#include "KVstore.h"
#include <string.h>
#include <assert.h>

const char *commands[] = {
    "SET", "GET", "ADD", "MOD"};

int KVstore_split_token(char *msg, char **tokens)
{
    if (msg == nullptr || tokens == nullptr)
    {
        return -1;
    }
    // 去掉 \n
    char *p = strchr(msg, '\n');
    if (p) *p = '\0';
    
    int idx = 0;
    char *token = strtok(msg, " ");
    while (token != nullptr)
    {
        tokens[idx++] = token;
        token = strtok(nullptr, " ");
    }
    return idx;
}

int kvstore_parser_protocol(Conn_item *item, char **tokens, int count)
{
    if (item == nullptr || tokens[0] == nullptr || count == 0)
    {
        return -1;
    }
    int cmd;
    for (cmd = KVS_CMD_START; cmd < KVS_COM_COUNT; cmd++)
    {
        if (strcmp(commands[cmd], tokens[0]) == 0)
        {
            break;
        }
    }
    char *msg = item->wbuffer_;
    memset(msg, 0, BUFFER_LEN);
    switch (cmd)
    {
    case KVS_COM_SET:
    {   // std::cout << "set" << std::endl;
        int res = kvstore_array_set(tokens[1], tokens[2]);
        if (!res)
        {
            snprintf(msg, BUFFER_LEN, "SUCESS");
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "FAILED");
        }
    }
        break;
    case KVS_COM_GET:
    { // std::cout << "get" << std::endl;
        char *value = kvs_array_get(tokens[1]);
        if (value)
        {
            snprintf(msg, BUFFER_LEN, "%s", value);
        }
        else
        {
            snprintf(msg, BUFFER_LEN, "NO EXIST");
        }
    }
    break;
    case KVS_COM_DEL:
        std::cout << "del" << std::endl;
        break;
    case KVS_COM_MOD:
        std::cout << "mod" << std::endl;
        break;
    default:
        std::cout << commands[cmd] << std::endl;
        assert(0);
    }
}
// Set Key Value
void KVstore_request(Conn_item *item)
{
    std::cout << "recv: " << item->rbuffer_ << std::endl;
    char *msg = item->rbuffer_;
    char *tokens[KVSTORE_MAX_TOKENS];
    int count = KVstore_split_token(msg, tokens);

    for (int idx = 0; idx < count; idx++)
    {
        std::cout << "dix:" << tokens[idx] << std::endl;
    }
    kvstore_parser_protocol(item, tokens, count);
    memset(item->rbuffer_, 0, BUFFER_LEN);
}

void *kvstore_malloc(size_t size)
{
    return malloc(size);
}
void kvstore_free(void *ptr)
{
    return free(ptr);
}

int main()
{
    epoll_entry();
}