/** @file avl.h
 *  @brief Functions prototypes for the avl binary search tree.
 *
 *  This file contains the prototypes and macros to control the avl ree directly
 *	Source used: Mastering algorithms in C
 *
 *  @author Bram Vlerick (bram.vlerick@ucast.be)
 *  @bug
 *  * None at the moment
 */


#ifndef _BINARYTREE_BISTREE_H_
#define _BINARYTREE_BISTREE_H_

#include "bitree.h"
#include "log.h"

#define AVL_LFT_HEAVY 1
#define AVL_BALANCED 0
#define AVL_RGT_HEAVY -1

/** @brief Definition of the avl node
 *
 *  This structure defines the avl tree node
 *
 */
typedef struct {
    /**< Node data */
    void *data;
    /**< Set data hidden */
    int hidden;
    /**< AVL Factor */
    int factor;
} avl_node_t;

/**< Macro to define avl_tree_t */
#define avl_tree_t bitree_t

/** @brief Initialise the avl tree
 *
 *  This function initialises an avl tree
 *
 *  @param tree Pointer to an avl tree
 *  @param compare Data compare callback
 *  @param destroy Destroy data callback
 */
avl_tree_t *avl_init(int (*compare)(const void *key1, const void *key2),
              void (*destroy)(void *data));

/** @brief Insert data into avl tree
 *
 *  This function inserts data into an avl tree
 *
 *  @param tree Pointer to the tree in which it will insert data
 *  @param data Void pointer to the data that will be inserted
 *
 *  @return 0 if successful, -1 if failed
 */
int avl_insert(avl_tree_t *tree, const void *data);

/** @brief Remove data from tree
 *
 *  Remove a node containing given data from the tree
 *
 *  @param tree Pointer to the avl tree
 *  @param data Reference data that has to be removed
 *
 *  @return 0 if successful, -1 if failed
 */
int avl_remove(avl_tree_t *tree, const void *data);

/** @brief Lookup data in the tree
 *
 *  This functions looks for a node with data that matches given reference data
 *
 *  @param tree Pointer to the avl tree
 *  @param data Pointer to a data reference
 *
 *  @return Returns 0 if successful, -1 if failed
 */
int avl_lookup(avl_tree_t *tree, void **data);

/**< Macro for accessing tree size */
#define avl_size(tree) ((tree)->size)

#endif
