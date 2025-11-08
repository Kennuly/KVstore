#include "KVstore.h"
#include <string.h>
#include "KVstore_array.h"

struct kvs_array_item array_table[KVS_ARRAY_SIZE] = {0};
int array_idx = 0;

int kvstore_array_set(char* key, char *value)
{
    if(key == NULL || value == NULL || array_idx == KVS_ARRAY_SIZE)
    {
        return -1;
    }
    char* kcopy = (char*)kvstore_malloc(strlen(key) + 1);
    if(kcopy == NULL)
    {
        return -1;
    }
    strcpy(kcopy, key);

    char* vcopy = (char*)kvstore_malloc(strlen(value) + 1);
    if(vcopy == NULL)
    {
        kvstore_free(kcopy);
        return -1;
    }
    strcpy(vcopy, value);

    array_table[array_idx].key = kcopy;
    array_table[array_idx].value = vcopy;
    ++array_idx;
    return 0;
}

char* kvs_array_get(char* key)
{
    if(key == NULL)
    {
        return NULL;
    }
    for(int i = 0; i < array_idx; i++)
    {
        if(strcmp(array_table[i].key, key) == 0)
        {
            return array_table[i].value;
        }
    }
    return NULL;
}

int kvstore_array_del(char* key)
{
    if(key == NULL)
    {
        return -1;
    }
    for(int i = 0; i < array_idx; i++)
    {
        if(strcmp(array_table[i].key, key) == 0)
        {
            array_table_del(i);
            return 0;
        }
    }
    return 1;
}
int kvstore_array_mod(char* key, char *value)
{
    if(key == NULL || value == NULL)
    {
        return -1;
    }
    for(int i = 0; i < array_idx; i++)
    {
        if(strcmp(array_table[i].key, key) == 0)
        {
            kvstore_free(array_table[i].value);
            array_table[i].value = NULL;
            char* vcopy = kvstore_malloc(strlen(value) + 1);
            strncpy(vcopy, value, strlen(value) + 1);
            array_table[i].value = vcopy;
            return 0;
        }
    }
    return 1;
}

void array_table_del(int loca)
{
    int i = 0;
    for(i = loca; i < array_idx - 1; i++)
    {
        array_table[i].key = array_table[i + 1].key;
        array_table[i].value = array_table[i + 1].value;
    }
    kvstore_free(array_table[i].key);
    kvstore_free(array_table[i].value);
    --array_idx;
}
int kvstore_array_count()
{
    return array_idx;
}