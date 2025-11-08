#ifndef __KVSTORE_ARRAY_H__
#define __KVSTORE_ARRAY_H__
#define ENABLE_ARRAY_KVENGINE   1
#if ENABLE_ARRAY_KVENGINE
struct kvs_array_item
{
    char *key;
    char *value;
};
typedef struct array_s
{
    struct kvs_array_item* array_table;
    int array_idx;
}array_t;
extern array_t arr;
int kvstore_array_create(array_t *array);
int kvstore_array_destory(array_t *array);

#define KVS_ARRAY_SIZE  1024
int array_set(array_t *array, char* key, char *value);
char* array_get(array_t* array ,char* key);
int array_del(array_t* array, char* key);
int array_mod(array_t* array, char* key, char *value);
int array_count(array_t *array);
void array_table_del(array_t* array, int loca);

int kvstore_array_set(char* key, char *value);
char* kvstore_array_get(char* key);
int kvstore_array_del(char* key);
int kvstore_array_mod(char* key, char *value);
int kvstore_array_count();
#endif
#endif