#include "KVstore.h"
#include <string.h>
#include "KVstore_array.h"

array_t arr;
int kvstore_array_create(array_t* array)
{
    if(array == NULL)
    {   
        return -1;
    }
    array->array_idx = 0;
    array->array_table = kvstore_malloc(KVS_ARRAY_SIZE * sizeof(struct kvs_array_item));
    if(!array->array_table)
    {
        return -1;
    }
    memset(array->array_table, 0, KVS_ARRAY_SIZE * sizeof(struct kvs_array_item));
    return 0;
}

int kvstore_arrya_destory(array_t *array)
{
    if(!array)
        return -1;
    if(!array->array_table)
        kvstore_free(array->array_table);
    return 0;
}

int array_set(array_t *array, char* key, char *value)
{
    if(key == NULL || value == NULL || array->array_idx == KVS_ARRAY_SIZE)
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

    array->array_table[array->array_idx].key = kcopy;
    array->array_table[array->array_idx].value = vcopy;
    ++array->array_idx;
    return 0;
}

char* array_get(array_t* array, char* key)
{
    if(key == NULL)
    {
        return NULL;
    }
    for(int i = 0; i < array->array_idx; i++)
    {
        if(strcmp(array->array_table[i].key, key) == 0)
        {
            return array->array_table[i].value;
        }
    }
    return NULL;
}

int array_del(array_t* array, char* key)
{
    if(key == NULL)
    {
        return -1;
    }
    for(int i = 0; i < array->array_idx; i++)
    {
        if(strcmp(array->array_table[i].key, key) == 0)
        {
            array_table_del(array, i);
            return 0;
        }
    }
    return 1;
}
int array_mod(array_t* array, char* key, char *value)
{
    if(key == NULL || value == NULL)
    {
        return -1;
    }
    for(int i = 0; i < array->array_idx; i++)
    {
        if(strcmp(array->array_table[i].key, key) == 0)
        {
            kvstore_free(array->array_table[i].value);
            array->array_table[i].value = NULL;
            char* vcopy = kvstore_malloc(strlen(value) + 1);
            strncpy(vcopy, value, strlen(value) + 1);
            array->array_table[i].value = vcopy;
            return 0;
        }
    }
    return 1;
}

void array_table_del(array_t* array, int loca)
{
    int i = 0;
    for(i = loca; i < array->array_idx - 1; i++)
    {
        array->array_table[i].key = array->array_table[i + 1].key;
        array->array_table[i].value = array->array_table[i + 1].value;
    }
    kvstore_free(array->array_table[i].key);
    kvstore_free(array->array_table[i].value);
    --array->array_idx;
}
int array_count(array_t* array)
{
    return array->array_idx;
}

int kvstore_array_set(char* key, char *value)
{
    return array_set(&arr, key, value);
}
char* kvstore_array_get(char* key)
{
    return array_get(&arr, key);
}
int kvstore_array_del(char* key)
{
    return array_del(&arr, key);
}
int kvstore_array_mod(char* key, char *value)
{
    return array_mod(&arr, key, value);
}

int kvstore_array_count()
{
    return array_count(&arr);
}