//
// Created by liuhang on 8/2/15.
//

#ifndef TREE_FAMILY_BST_TREE_HH
#define TREE_FAMILY_BST_TREE_HH

#include <ostream>
#include <memory>
#include <assert.h>

template <class T>
class bst_tree_node
{
public:
    bst_tree_node();
    bst_tree_node(const T&);
    bst_tree_node<T> *parent;
    bst_tree_node<T> *left_child;
    bst_tree_node<T> *right_child;
    T data;
};

template<class T>
bst_tree_node<T>::bst_tree_node()
        : parent(0), left_child(0), right_child(0)
{
}

template<class T>
bst_tree_node<T>::bst_tree_node(const T& val)
        : parent(0), left_child(0), right_child(0), data(val)
{
}

// T need to operator < and ==
template <class T, class bst_tree_nodeallocator = std::allocator<bst_tree_node<T> > >
class bst_tree
{
public:
    typedef bst_tree_node<T> tree_node;
    class in_order_iterator;
    class search_iterator;

    class iterator_base {
    public:
        iterator_base();
        iterator_base(tree_node *node, bst_tree *order_tree);

        T& operator*() const;
        T* operator->() const;
        tree_node *node;
        bst_tree<T, bst_tree_nodeallocator > *order;
    };

    class in_order_iterator : public iterator_base {
    public:
        in_order_iterator();
        in_order_iterator(tree_node *node, bst_tree *order_tree);
        in_order_iterator& operator++();
        bool operator==(const in_order_iterator& right);
        bool operator!=(const in_order_iterator& right);
    };
    class search_iterator : public iterator_base {
    public:
        search_iterator(const T& dst, bst_tree *order_tree);
        search_iterator(tree_node *node, const T& dst, bst_tree *order_tree);
        search_iterator& operator++();
        bool operator==(const search_iterator &right);
        bool operator!=(const search_iterator &right);
    private:
        T search_dst;
    };
public:
    bst_tree();
    bool insert(const T&x);
    search_iterator find(const T& dst, search_iterator iter_begin , search_iterator iter_end);
    search_iterator find(const T& dst);

    in_order_iterator begin();
    in_order_iterator end();
    search_iterator search_begin(const T &dst);
    search_iterator search_end(const T &dst);
protected:
    bool insert(tree_node** position, tree_node* parent, const T& x);
    void init();
    void try_swap_max(T *x);
    void try_swap_min(T *x);
public:
    tree_node *head, *end_flag;
    T *max, *min;
    bst_tree_nodeallocator alloc_;
};

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base() {
    node = NULL;
}

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base(tree_node *_node, bst_tree *order_tree) {
    node = _node;
    order = order_tree;
}

template <class T, class bst_tree_nodeallocator >
T& bst_tree<T, bst_tree_nodeallocator >::iterator_base::operator*() const{
    return node->data;
}

template <class T, class bst_tree_nodeallocator >
T* bst_tree<T, bst_tree_nodeallocator >::iterator_base::operator->() const{
    return &node->data;
}

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::in_order_iterator() {

};

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::in_order_iterator(bst_tree<T, bst_tree_nodeallocator >::tree_node *node, bst_tree *order_tree) :
bst_tree<T, bst_tree_nodeallocator >::iterator_base(node, order_tree) {
};

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::in_order_iterator& bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::operator++() {
    assert(this->node!= 0);
    tree_node *next_node = NULL;
    if(*(this->order->max) == this->node->data) {
        this->node = this->order->end_flag;
        return *this;
    }
    if(this->node->right_child) {
        next_node = this->node->right_child;
        while(next_node->left_child) {
            next_node = next_node->left_child;
        }
        this->node = next_node;
        return *this;
    }
    else if(this->node == this->node->parent->left_child) {
        next_node = this->node->parent;
        this->node = next_node;
        return *this;
    }
    else if(this->node == this->node->parent->right_child) {
        next_node = this->node;
        while(next_node == next_node->parent->right_child) {
            next_node = next_node->parent;
        }
        next_node = next_node->parent;
        this->node = next_node;
        return *this;
    }
    this->node = this->order->end_flag;
    return *this;
};

template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::operator==(const bst_tree<T, bst_tree_nodeallocator >::in_order_iterator& right) {
    if(right.node==this->node) return true;
    else return false;
};

template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::operator!=(const bst_tree<T, bst_tree_nodeallocator >::in_order_iterator& right) {
    if(right.node!=this->node) return true;
    else return false;
};

// template <class T, class bst_tree_nodeallocator >
// bst_tree<T, bst_tree_nodeallocator >::search_iterator::search_iterator(const T& dst, bst_tree *order_tree) :
// bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base(order_tree)
// {
//
// }
template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::search_iterator::search_iterator(tree_node *node, const T& dst, bst_tree *order_tree) :
bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base(node, order_tree) ,
search_dst(dst) {

}
template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator& bst_tree<T, bst_tree_nodeallocator >::search_iterator::operator++() {
    assert(this->node!= NULL);
   if(this->search_dst < this->node->data)  {
       if(this->node->left_child) {
           this->node = this->node->left_child;
           return *this;
       }
       else {
           this->node = this->order->end_flag;
           return *this;
       }
   }
   else if(this->node->data < this->search_dst) {
       if(this->node->right_child) {
           this->node = this->node->right_child;
           return *this;
       }
       else {
           this->node = this->order->end_flag;
           return *this;
       }
   }
   else {
           this->node = this->order->end_flag;
           return *this;
   }

}

template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::search_iterator::operator==(const bst_tree<T, bst_tree_nodeallocator >::search_iterator &right) {
    if(right.node==this->node) return true;
    else return false;
}

template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::search_iterator::operator!=(const bst_tree<T, bst_tree_nodeallocator >::search_iterator &right) {
    if(right.node!=this->node) return true;
    else return false;
}

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::in_order_iterator bst_tree<T, bst_tree_nodeallocator >::begin() {
    if(!head)
        return in_order_iterator(end_flag, this);
    tree_node *temp_node = head;
    while(temp_node->left_child) {
        temp_node = temp_node->left_child;
    }
    return in_order_iterator(temp_node, this);
};

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::in_order_iterator bst_tree<T, bst_tree_nodeallocator >::end() {
    return in_order_iterator(end_flag, this);
}

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator bst_tree<T, bst_tree_nodeallocator >::search_begin(const T &dst) {
    if(head) {
        return search_iterator(head, dst, this);
    }
    else {
        return search_iterator(end_flag, dst, this);
    }
}

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator bst_tree<T, bst_tree_nodeallocator >::search_end(const T &dst) {
    return search_iterator(end_flag, dst, this);
}

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator
bst_tree<T, bst_tree_nodeallocator >::find(const T& dst, search_iterator iter_begin , search_iterator iter_end) {
    search_iterator tmp = iter_begin;
    while(*tmp == dst && tmp!= iter_end) {
        ++tmp;
    }
    return tmp;
}
template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator
bst_tree<T, bst_tree_nodeallocator >::find(const T& dst)
{
    search_iterator tmp = this->search_begin(dst);
    while(*tmp == dst && tmp!= this->search_end(dst)) {
        ++tmp;
    }
    return tmp;
}

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::bst_tree() :
head(0), end_flag(0), max(0), min(0) {
    init();
};
template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::insert(const T&x) {
    return this->insert(&head, 0, x);
};

template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::insert(tree_node** position, tree_node *parent, const T& x) {
    tree_node *cur_node = *position;

    if(cur_node!= NULL && cur_node->data == x)
    {
        return false;
    }

    if(cur_node == NULL) {
        tree_node *new_node = alloc_.allocate(1, 0);
        alloc_.construct(new_node, x);
        cur_node = new_node;
        *position = cur_node;
        cur_node->parent = parent;
        try_swap_max((T*)&cur_node->data);
        try_swap_min((T*)&cur_node->data);
        return true;
    }
    else if(x < cur_node->data ) {
        return insert(&cur_node->left_child, cur_node, x);
    }
    else if(cur_node->data < x) {
        return insert(&cur_node->right_child, cur_node, x);
    }
    else {
        return false;
    }
}

template <class T, class bst_tree_nodeallocator >
void bst_tree<T, bst_tree_nodeallocator >::init() {
    end_flag = alloc_.allocate(1, 0);
    alloc_.construct(end_flag);
};


template <class T, class bst_tree_nodeallocator >
void bst_tree<T, bst_tree_nodeallocator >::try_swap_max(T *dst) {
    if(!max) {
        max = dst;
    }
    if(*max < *dst) {
        max = dst;
    }
};

template <class T, class bst_tree_nodeallocator >
void bst_tree<T, bst_tree_nodeallocator >::try_swap_min(T *dst) {
    if(!min) {
        min = dst;
    }
    if(*dst < *min) {
        min = dst;
    }
}

#endif //TREE_FAMILY_BST_TREE_HH
