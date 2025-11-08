#ifndef __RBTREE_H__

#define __RBTREE_H__

#define ENABLE_RBTREE_KVENGINE   1

#ifdef ENABLE_RBTREE_KVENGINE

#define RED				1
#define BLACK 			2


#define ENABLE_KEY_CHAR		1

#define MAX_KEY_LEN			256
#define MAX_VALUE_LEN		1024

#if ENABLE_KEY_CHAR
typedef char* KEY_TYPE;
#else
typedef int KEY_TYPE;
#endif


typedef struct _rbtree_node {
	unsigned char color;
	struct _rbtree_node *right;
	struct _rbtree_node *left;
	struct _rbtree_node *parent;

	KEY_TYPE key;
	void *value;
} rbtree_node;

typedef struct _rbtree {
	rbtree_node *root;
	rbtree_node *nil;
	
	int count;
} rbtree;

extern rbtree Tree;
int kvstore_rbtree_create(rbtree *tree);
void kvstore_rbtree_destory(rbtree *tree);

int rbtree_set(rbtree *tree, char *key, char *value);
char* rbtree_get(rbtree *tree, char *key);
int rbtree_del(rbtree *tree, char *key);
int rbtree_mod(rbtree *tree, char *key, char *value);
int rbtree_size(rbtree *tree);

int kvs_rbtree_set(char *key, char *value);
char* kvs_rbtree_get(char *key);
int kvs_rbtree_delete(char *key);
int kvs_rbtree_modify(char *key, char *value);
int kvs_rbtree_count();

#endif

#endif