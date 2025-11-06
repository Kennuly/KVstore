#ifndef __KVSTORE_H__
#define __KVSTORE_H__
#include "Reactor.h"  

#define KVSTORE_MAX_TOKENS 128

void KVstore_request(Conn_item *item);
int KVstore_split_token(char *msg, char **tokens);
#endif
