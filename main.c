#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "avl.h"
#include "log.h"

struct key_value_t {
	char key[200];
	char val[200];
};

struct key_value_t key1 = {
	.key = "ip",
	.val = "192.168.1.22",
};

struct key_value_t key2 = {
	.key = "ms_ip",
	.val = "192.168.1.253",
};

struct key_value_t key3 = {
	.key = "radio_ip",
	.val = "192.168.2.22",
};


int compare_keys(const void *k1, const void *k2)
{
	struct key_value_t *x1, *x2;
	x1 = (struct key_value_t*)k1;
	x2 = (struct key_value_t*)k2;
	return strcmp(x1->key, x2->key);
}

void populate_db(avl_tree_t *tree)
{
	avl_insert(tree, (void*)&key1);
	avl_insert(tree, (void*)&key2);
	avl_insert(tree, (void*)&key3);
}

int main()
{
	avl_tree_t *tree = avl_init(compare_keys, free); 
	struct key_value_t *search = malloc(sizeof(struct key_value_t));
	strcpy(search->key, "ip");

	populate_db(tree);

	int result = avl_lookup(tree, (void **)&search);
	if (!result) {
		printf("result found!\n");
		printf("%s\n", search->val);
	}

	bitree_destroy(tree);
}
