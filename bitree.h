/** @file bitree.h
 *  @brief Functions prototypes for the bitree.
 *
 *  This file contains the prototypes and macros to control the bitree directly
 *	Source used: Mastering algorithms in C
 *
 *  @author Bram Vlerick (bram.vlerick@ucast.be)
 *  @bug
 *  * None at the moment
 */

#ifndef _BITREE_H_
#define _BITREE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "log.h"

/** @brief Definition of the binary tree node struct
 *
 *  This structure contains bitree node data
 *
 */
typedef struct bitree_node_ {
    /**< Node data */
    void *data;
    /**< Left node pointer */
    struct bitree_node_ *left;
    /**< Right node pointer */
    struct bitree_node_ *right;
} bitree_node_t;

/** @brief Definition of the binary tree structure
 *
 *  This structure contains al tree data
 *
 */
typedef struct {
    /**< This binary tree size */
    long size;
    /**< Compare callback function */
    int (*compare)(const void *key1, const void *key2);
    /**< Destroy data callback function */
    void (*destroy)(void *data);
    /**< Tree root node pointer */
    bitree_node_t *root;
} bitree_t;

/** @brief Initialise the binary tree
 *
 *  This function will initialise a given binary tree
 *
 *  @param tree Pointer to the binary tree
 *  @param destroy Destroy data callback
 */
bitree_t *bitree_init(void (*destroy)(void *data));

/** @brief Destroy the binary tree
 *
 *  Destroy a given binary tree
 *
 *  @param tree Pointer to a binary tree
 */
void bitree_destroy(bitree_t *tree);

/** @brief Insert data at the left
 *
 *  Insert data into the binary tree in the left branch of a given node
 *
 *  @param tree Pointer to the binary tree
 *  @param node Pointer to the node where we will do a left insert
 *  @param data The data that will be inserted
 */
int bitree_ins_left(bitree_t *tree, bitree_node_t *node, const void *data);

/** @brief Insert data at the right
 *
 *  Insert data into the binary tree in the right branch of a given node
 *
 *  @param tree Pointer to the binary tree
 *  @param node Pointer to the node where we will do a left insert
 *  @param data The data that will be inserted
 */
int bitree_ins_right(bitree_t *tree, bitree_node_t *node, const void *data);

/** @brief Remove the left data from a node
 *
 *  Remove the left branch of a given node
 *
 *  @param tree The pointer to the tree
 *  @param node The node from which we will remove the left branch
 */
void bitree_rem_left(bitree_t *tree, bitree_node_t *node);

/** @brief Remove the right data from a node
 *
 *  Remove the left branch of a given node
 *
 *  @param tree The pointer to the tree
 *  @param node The node from which we will remove the right branch
 */
void bitree_rem_right(bitree_t *tree, bitree_node_t *node);

/** @brief Merge two trees
 *
 * This function will merge two trees with given data as the root node
 *
 * @param merge The merged tree result
 * @param left The left branch of the root node
 * @param right the right branch of the root node
 * @param data The data that will be used to create the root node
 * @return Returns 0 if successful, -1 if failed
 */
int bitree_merge(bitree_t *merge, bitree_t *left, bitree_t *right,
                 const void *data);

/**< Macro to retrieve tree size */
#define bitree_size(tree) ((tree)->size)

/**< Macro to retrieve the root of a tree */
#define bitree_root(tree) ((tree)->root)

/**< Macro to check if node is the end of a branch */
#define bitree_is_eob(node) ((node) == NULL)

/**< Check if the node is a leaf */
#define bitree_is_leaf(node) ((node)->left == NULL && (node)->right == NULL)

/**< Macro to retrieve data from a node */
#define bitree_data(node) ((node)->data)

/**< Macro to retrieve the left branch */
#define bitree_left(node) ((node)->left)

/**< Macro to retrieve the right branch */
#define bitree_right(node) ((node)->right)

#endif
