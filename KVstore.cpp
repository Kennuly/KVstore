#include "Reactor.h"
#include "set.h"
#include <iostream>
#include "KVstore.h"
#include <string.h>


int KVstore_split_token(char *msg, char **tokens)
{
    if(msg == nullptr || tokens == nullptr)
    {
        return -1;
    }
    int idx = 0;
    char* token = strtok(msg, " ");
    while(token != nullptr)
    {
        tokens[idx++] = token;
        token = strtok(nullptr, " ");

    }
    return idx;
}

//Set Key Value
void KVstore_request(Conn_item *item)
{
    std::cout << "recv: " << item->rbuffer_ << std::endl;
    char *msg = item->rbuffer_;
    char *tokens[KVSTORE_MAX_TOKENS];
    int count = KVstore_split_token(msg, tokens);

    
    for(int idx = 0; idx < count; idx++)
    {
        std::cout << "dix:" << tokens[idx];
    }
    std::cout << std::endl;
}

int main()
{
    epoll_entry();
}