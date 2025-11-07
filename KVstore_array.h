#ifndef __KVSTORE_ARRAY_H__
#define __KVSTORE_ARRAY_H__
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
int kvstore_array_del(char* key);
int kvstore_array_mod(char* key, char *value);

void array_table_del(int loca);

#endif
#endif