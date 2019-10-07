/** @file avl.h
 *  @brief Functions for the avl binary search tree.
 *
 *  This file contains the functions to control the avl ree directly
 *	Source used: Mastering algorithms in C
 *
 *  @author Bram Vlerick (bram.vlerick@ucast.be)
 *  @bug
 *  * None at the moment
 *
 *	This code works, needs no changes
 */

#include "avl.h"

static void destroy_right(avl_tree_t *tree, bitree_node_t *node);

static void rotate_left(bitree_node_t **node) {
    debug(D_AVLTREE, "Rotating right");
    bitree_node_t *left, *grandchild;
    left = bitree_left(*node);
    if (((avl_node_t *)bitree_data(*node))->factor == AVL_LFT_HEAVY) {
        bitree_left(*node) = bitree_right(left);
        bitree_right(left) = *node;
        ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
        ((avl_node_t *)bitree_data(left))->factor = AVL_BALANCED;
        *node = left;
    } else {
        grandchild = bitree_right(left);
        bitree_right(left) = bitree_left(grandchild);
        bitree_left(grandchild) = left;
        bitree_left(*node) = bitree_right(grandchild);
        bitree_right(grandchild) = *node;

        switch (((avl_node_t *)bitree_data(grandchild))->factor) {
        case AVL_LFT_HEAVY:
            ((avl_node_t *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
            ((avl_node_t *)bitree_data(left))->factor = AVL_BALANCED;
            break;
        case AVL_BALANCED:
            ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
            ((avl_node_t *)bitree_data(left))->factor = AVL_BALANCED;
            break;
        case AVL_RGT_HEAVY:
            ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
            ((avl_node_t *)bitree_data(left))->factor = AVL_LFT_HEAVY;
            break;
        }
        ((avl_node_t *)bitree_data(grandchild))->factor = AVL_BALANCED;
        *node = grandchild;
    }
    return;
}

static void rotate_right(bitree_node_t **node) {
    debug(D_AVLTREE, "Rotating left");
    bitree_node_t *right, *grandchild;
    right = bitree_right(*node);
    if (((avl_node_t *)bitree_data(*node))->factor == AVL_RGT_HEAVY) {
        bitree_right(*node) = bitree_left(right);
        bitree_left(right) = *node;
        ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
        ((avl_node_t *)bitree_data(right))->factor = AVL_BALANCED;
    } else {
        grandchild = bitree_left(right);
        bitree_left(right) = bitree_right(grandchild);
        bitree_right(grandchild) = right;
        bitree_right(*node) = bitree_left(grandchild);
        bitree_left(grandchild) = *node;

        switch (((avl_node_t *)bitree_data(grandchild))->factor) {
        case AVL_LFT_HEAVY:
            ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
            ((avl_node_t *)bitree_data(right))->factor = AVL_RGT_HEAVY;
            break;
        case AVL_BALANCED:
            ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
            ((avl_node_t *)bitree_data(right))->factor = AVL_BALANCED;
            break;
        case AVL_RGT_HEAVY:
            ((avl_node_t *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
            ((avl_node_t *)bitree_data(right))->factor = AVL_BALANCED;
        }
        ((avl_node_t *)bitree_data(grandchild))->factor = AVL_BALANCED;
        *node = grandchild;
    }
    return;
}

static void destroy_left(avl_tree_t *tree, bitree_node_t *node) {
    debug(D_AVLTREE, "Destroying left branch");
    bitree_node_t **position;
    if (bitree_size(tree) == 0)
        return;
    if (node == NULL)
        position = &tree->root;
    else
        position = &node->left;

    if (*position != NULL) {
        destroy_left(tree, *position);
        destroy_right(tree, *position);
        if (tree->destroy != NULL) {
            tree->destroy(((avl_node_t *)(*position)->data)->data);
        }
        free((*position)->data);
        free(*position);
        *position = NULL;
        tree->size--;
    }
    return;
}

static void destroy_right(avl_tree_t *tree, bitree_node_t *node) {
    debug(D_AVLTREE, "Destroying right branch");
    bitree_node_t **position;
    if (bitree_size(tree) == 0)
        return;
    if (node == NULL)
        position = &tree->root;
    else
        position = &node->right;

    if (*position != NULL) {
        destroy_left(tree, *position);
        destroy_right(tree, *position);
        if (tree->destroy != NULL) {
            tree->destroy(((avl_node_t *)(*position)->data)->data);
        }
        free((*position)->data);
        free(*position);
        *position = NULL;
        tree->size--;
    }
    return;
}

static int insert(avl_tree_t *tree, bitree_node_t **node, const void *data,
                  int *balanced) {
    avl_node_t *avl_data;
    int cmpval, retval = 2;
    if (bitree_is_eob(*node)) {
        if ((avl_data = malloc(sizeof(avl_node_t))) == NULL) {
            error("Failed to allocate data");
            return -1;
        }
        avl_data->factor = AVL_BALANCED;
        avl_data->hidden = 0;
        avl_data->data = (void *)data;
        debug(D_AVLTREE,"End of branch");
        return bitree_ins_left(tree, *node, avl_data);
    } else {
        cmpval = tree->compare(data, ((avl_node_t *)bitree_data(*node))->data);
        if (cmpval < 0) {
            if (bitree_is_eob(bitree_left(*node))) {
                if ((avl_data = malloc(sizeof(avl_node_t))) == NULL) {
                    error("Failed to allocate data");
                    return -1;
                }
                avl_data->factor = AVL_BALANCED;
                avl_data->hidden = 0;
                avl_data->data = (void *)data;

                if (bitree_ins_left(tree, *node, avl_data) != 0) {
                    error("Failed to insert left");
                    return -1;
                }
                *balanced = 0;
            } else {
                if ((retval = insert(tree, &bitree_left(*node), data,
                                     balanced)) != 0) {
                    error("%d : Failed to insert data into left branch", retval);
                    return retval;
                } else
                    debug(D_AVLTREE, "Data inserted");
            }

            if (!(*balanced)) {
                debug(D_AVLTREE, "Balancing tree");
                switch (((avl_node_t *)bitree_data(*node))->factor) {
                case AVL_LFT_HEAVY:
                    rotate_left(node);
                    *balanced = 1;
                    break;
                case AVL_BALANCED:
                    ((avl_node_t *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
                    break;
                case AVL_RGT_HEAVY:
                    ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
                    *balanced = 1;
                }
            }
        } else if (cmpval > 0) {
            if (bitree_is_eob(bitree_right(*node))) {
                if ((avl_data = malloc(sizeof(avl_node_t))) == NULL){
                    error("Failed to allocate data");
                    return -1;
                }
                avl_data->factor = AVL_BALANCED;
                avl_data->hidden = 0;
                avl_data->data = (void *)data;

                if (bitree_ins_right(tree, *node, avl_data) != 0) {
                    error("Failed to insert right");
                    return -1;
                }
                debug(D_AVLTREE,"Inserted right");
                *balanced = 0;
            } else {
                if ((retval = insert(tree, &bitree_right(*node), data,
                                     balanced)) != 0) {
                    error("%d : Failed to insert data into right branch", retval);
                    return retval;
                } else
                    debug(D_AVLTREE, "Data inserted");
            }

            if (!(*balanced)) {
                debug(D_AVLTREE, "Balancing tree");
                switch (((avl_node_t *)bitree_data(*node))->factor) {
                case AVL_LFT_HEAVY:
                    ((avl_node_t *)bitree_data(*node))->factor = AVL_BALANCED;
                    *balanced = 1;
                    break;
                case AVL_BALANCED:
                    ((avl_node_t *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
                    break;
                case AVL_RGT_HEAVY:
                    rotate_right(node);
                    *balanced = 1;
                }
            }
        } else {
            if (!((avl_node_t *)bitree_data(*node))->hidden) {
                error("Data already exists");
                return 1;
            } else {
                debug(D_AVLTREE, "Unhiding data");
                if (tree->destroy != NULL) {
                    tree->destroy(((avl_node_t *)bitree_data(*node))->data);
                }
                ((avl_node_t *)bitree_data(*node))->data = (void *)data;
                ((avl_node_t *)bitree_data(*node))->hidden = 0;
                *balanced = 1;
                return 1;
            }
        }
    }
    return retval;
}

static int hide(avl_tree_t *tree, bitree_node_t *node, const void *data) {
    debug(D_AVLTREE, "Hiding data");
    int cmpval, retval;
    if (bitree_is_eob(node))
        return -1;
    cmpval = tree->compare(data, ((avl_node_t *)bitree_data(node))->data);
    if (cmpval < 0) {
        retval = hide(tree, bitree_left(node), data);
    } else if (cmpval > 0) {
        retval = hide(tree, bitree_right(node), data);
    } else {
        ((avl_node_t *)bitree_data(node))->hidden = 1;
        retval = 0;
    }
    return retval;
}

static int lookup(avl_tree_t *tree, bitree_node_t *node, void **data) {
    debug(D_AVLTREE, "Performing lookup");
    int cmpval, retval;
    if (bitree_is_eob(node)) {
        return -1;
    }
    cmpval = tree->compare(*data, ((avl_node_t *)bitree_data(node))->data);
    if (cmpval < 0) {
        retval = lookup(tree, bitree_left(node), data);
    } else if (cmpval > 0) {
        retval = lookup(tree, bitree_right(node), data);
    } else {
        if (!((avl_node_t *)bitree_data(node))->hidden) {
            *data = ((avl_node_t *)bitree_data(node))->data;
            retval = 0;
        } else {
            return -1;
        }
    }
    return retval;
}

avl_tree_t *avl_init(int (*compare)(const void *key1, const void *key2),
                     void (*destroy)(void *data)) {
    avl_tree_t *tree;
    tree = bitree_init(destroy);
    tree->compare = compare;
    debug(D_AVLTREE, "Initialised AVL Tree");
    return tree;
}

void avl_destroy(avl_tree_t *tree) {
    debug(D_AVLTREE, "Destroying Binary search tree");
    if (!tree) {
        debug(D_AVLTREE, "Tree pointer cannot be NULL");
        debug(D_AVLTREE, "Allocate tree first");
        return;
    }
    destroy_left(tree, NULL);
    memset(tree, 0, sizeof(avl_tree_t));
    free(tree);
    tree = NULL;
    return;
}

int avl_insert(avl_tree_t *tree, const void *data) {
    debug(D_AVLTREE, "Inserting data");
    if (!tree) {
        debug(D_AVLTREE, "Tree pointer cannot be NULL");
        debug(D_AVLTREE, "Allocate tree first");
        return -1;
    }
    int balanced = 0;
    return insert(tree, &bitree_root(tree), data, &balanced);
}

int avl_remove(avl_tree_t *tree, const void *data) {
    debug(D_AVLTREE, "Removing data");
    if (!tree) {
        debug(D_AVLTREE, "Tree pointer cannot be NULL");
        debug(D_AVLTREE, "Allocate tree first");
        return -1;
    }
    // TODO: Change to remove instead of hide
    return hide(tree, bitree_root(tree), data);
}

int avl_lookup(avl_tree_t *tree, void **data) {
    if (!tree) {
        debug(D_AVLTREE, "Tree pointer cannot be NULL");
        debug(D_AVLTREE, "Allocate tree first");
        return -1;
    }
    debug(D_AVLTREE, "Performing lookup");
    return lookup(tree, bitree_root(tree), data);
}
