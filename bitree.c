/** @file bitree.h
 *  @brief Functions prototypes for the bitree.
 *
 *  This file contains the functions and macros to control the bitree directly
 *	Source used: Mastering algorithms in C
 *
 *  @author Bram Vlerick (bram.vlerick@ucast.be)
 *  @bug
 *  * None at the moment
 *
 *	This code works, needs no changes
 */

#include "bitree.h"

bitree_t *bitree_init(void (*destroy)(void *data)) {
    bitree_t *tree = malloc(sizeof(bitree_t));
    if (!tree) {
        debug(D_BITREE, "Tree pointer cannot be NULL");
        debug(D_BITREE, "Allocate tree first");
        return NULL;
    }
    tree->size = 0;
    tree->destroy = destroy;
    tree->root = NULL;
    debug(D_BITREE, "Binary tree initialised");
    return tree;
}

void bitree_destroy(bitree_t *tree) {
    if (!tree) {
        debug(D_BITREE, "Tree pointer cannot be NULL");
        debug(D_BITREE, "Allocate tree first");
        return;
    }
    debug(D_BITREE, "Destroying binary tree");
    bitree_rem_left(tree, NULL);
    memset(tree, 0, sizeof(bitree_t));
    free(tree);
    tree = NULL;
    return;
}

int bitree_ins_left(bitree_t *tree, bitree_node_t *node, const void *data) {
    if (!tree) {
        error("Tree pointer cannot be NULL");
        error("Allocate tree first");
        return -1;
    }
    debug(D_BITREE, "Trying to insert into left branch");
    bitree_node_t *new_node, **position;
    if (node == NULL) {
        if (bitree_size(tree) > 0) {
            error("Invalid tree size");
            return -1;
        }
        position = &tree->root;
    } else {
        if (bitree_left(node) != NULL) {
            error("Left node is not null");
            return -1;
        }
        position = &node->left;
    }
    if ((new_node = malloc(sizeof(bitree_node_t))) == NULL)
        return -1;

    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;
    debug(D_BITREE, "Inserted into left branch");
    tree->size++;
    return 0;
}

int bitree_ins_right(bitree_t *tree, bitree_node_t *node, const void *data) {
    if (!tree) {
        debug(D_BITREE, "Tree pointer cannot be NULL");
        debug(D_BITREE, "Allocate tree first");
        return -1;
    }
    debug(D_BITREE, "Trying to insert into right branch");
    bitree_node_t *new_node, **position;
    if (node == NULL) {
        if (bitree_size(tree) > 0) {
            error("invalid tree size");
            return -1;
        }
        position = &tree->root;
    } else {
        if (bitree_right(node) != NULL){
            error("Right node is not NULL");
            return -1;
        }
        position = &node->right;
    }
    if ((new_node = malloc(sizeof(bitree_node_t))) == NULL)
        return -1;
    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;
    debug(D_BITREE, "Inserted into right branch");
    tree->size++;
    return 0;
}

void bitree_rem_left(bitree_t *tree, bitree_node_t *node) {
    if (!tree) {
        debug(D_BITREE, "Tree pointer cannot be NULL");
        debug(D_BITREE, "Allocate tree first");
        return;
    }
    debug(D_BITREE, "Trying to remove left node");
    bitree_node_t **position;
    if (bitree_size(tree) == 0)
        return;
    if (node == NULL)
        position = &tree->root;
    else
        position = &node->left;

    if (*position != NULL) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);
        if (tree->destroy != NULL)
            tree->destroy((*position)->data);
        free(*position);
        *position = NULL;
        debug(D_BITREE, "Left node removed");
        tree->size--;
    }
    return;
}

void bitree_rem_right(bitree_t *tree, bitree_node_t *node) {
    if (!tree) {
        error("Tree pointer cannot be NULL");
        error("Allocate tree first");
        return;
    }
    debug(D_BITREE, "Trying to remove right node");
    bitree_node_t **position;
    if (node == NULL)
        position = &tree->root;
    else
        position = &node->right;

    if (*position != NULL) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);
        if (tree->destroy != NULL)
            tree->destroy((*position)->data);
        free(*position);
        *position = NULL;
        debug(D_BITREE, "Right node removed");
        tree->size--;
    }
    return;
}

int bitree_merge(bitree_t *merge, bitree_t *left, bitree_t *right,
                 const void *data) {
    if (!left || !right) {
        return -1;
    }
    merge = bitree_init(left->destroy);
    if (!merge || !left || !right) {
        debug(D_BITREE, "Tree pointer cannot be NULL");
        debug(D_BITREE, "Allocate tree first");
        return -1;
    }
    debug(D_BITREE, "Trying to merge trees");
    if (bitree_ins_left(merge, NULL, data) != 0) {
        bitree_destroy(merge);
        debug(D_BITREE, "Merge failed");
        return -1;
    }

    bitree_root(merge)->left = bitree_root(left);
    bitree_root(merge)->right = bitree_root(right);
    merge->size = merge->size + bitree_size(left) + bitree_size(right);

    left->root = NULL;
    left->size = 0;
    right->root = NULL;
    right->size = 0;
    debug(D_BITREE, "Merge successful");

    return 0;
}
