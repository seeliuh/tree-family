#ifndef TREE_FAMILY_CHECK_TOOL
#define TREE_FAMILY_CHECK_TOOL
#include "../bst_tree.hh"

template <class T, class bst_tree_nodeallocator = std::allocator<bst_tree_node<T> > >
bool bst_check(bst_tree<T, bst_tree_nodeallocator > &tree) {
    return bst_check_recursion(tree.head);
}

template <class T >
bool bst_check_recursion(bst_tree_node<T > *node ) {
    if(node && node->left) {
        assert(node->left->data < node->data );
        return bst_check_recursion(node->left);
    }
    if(node && node->right) {
        assert(node->data < node->right->data );
        return bst_check_recursion(node->right);
    }
    return true;
}

#endif
