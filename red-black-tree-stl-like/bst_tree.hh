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
    virtual ~bst_tree_node();
    bst_tree_node<T> *parent;
    bst_tree_node<T> *left;
    bst_tree_node<T> *right;
    T data;
};

template<class T>
bst_tree_node<T>::bst_tree_node()
        : parent(0), left(0), right(0)
{
}

template<class T>
bst_tree_node<T>::bst_tree_node(const T& val)
        : parent(0), left(0), right(0), data(val)
{
}

template<class T>
bst_tree_node<T>::~bst_tree_node()
{
}
// T need to operator < and ==
template <class T, class bst_tree_nodeallocator = std::allocator<bst_tree_node<T> > >
class bst_tree
{
public:
    typedef bst_tree_node<T> bstnode_t;
    class in_order_iterator;
    class pre_order_iterator;
    class search_iterator;

    class iterator_base {
    public:
        iterator_base();
        iterator_base(bstnode_t *node, bst_tree *order_tree);

        T& operator*() const;
        T* operator->() const;
        bstnode_t *node;
        bst_tree<T, bst_tree_nodeallocator > *order;
    };

    class in_order_iterator : public iterator_base {
    public:
        in_order_iterator();
        in_order_iterator(bstnode_t *node, bst_tree *order_tree);
        in_order_iterator& operator++();
        bool operator==(const in_order_iterator& right);
        bool operator!=(const in_order_iterator& right);
    };
    class search_iterator : public iterator_base {
    public:
        //search_iterator(const T& dst, bst_tree *order_tree);
        search_iterator(bstnode_t *node, const T& dst, bst_tree *order_tree);
        search_iterator& operator++();
        bool operator==(const search_iterator &right);
        bool operator!=(const search_iterator &right);
    private:
        T search_dst;
    };
public:
    bst_tree();
    virtual bool insert(const T&x);
    virtual bool remove(const T&x);
    virtual search_iterator find(const T& dst, search_iterator iter_begin , search_iterator iter_end);
    virtual search_iterator find(const T& dst);
    virtual in_order_iterator begin();
    virtual in_order_iterator end();
    virtual search_iterator search_begin(const T &dst);
    virtual search_iterator search_end(const T &dst);
protected:
    void init();
    void try_swap_max(T *x);
    void try_swap_min(T *x);
    virtual bstnode_t *allocate_node(const T &x){
        return new bstnode_t(x);
        //bstnode_t * r = alloc_.allocate(1,0);
        //alloc_.construct((bstnode_t *)r);
        //return r;
    }
    virtual void deallocate_node(bstnode_t *node){
        delete node;
        //alloc_.destroy((bstnode_t *)node);
        //alloc_.deallocate((bstnode_t *)node,1);
    }
    virtual bool isnull(bstnode_t *node);

public:
    bstnode_t *head, *end_flag;
    T *max, *min;
    bst_tree_nodeallocator alloc_;
};

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base() {
    node = NULL;
}

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base(bstnode_t *_node, bst_tree *order_tree) {
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
bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::in_order_iterator(bst_tree<T, bst_tree_nodeallocator >::bstnode_t *node, bst_tree *order_tree) :
bst_tree<T, bst_tree_nodeallocator >::iterator_base(node, order_tree) {
};

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::in_order_iterator&
bst_tree<T, bst_tree_nodeallocator >::in_order_iterator::operator++() {
    assert(this->node!= 0);
    bstnode_t *next_node = NULL;
    if(*(this->order->max) == this->node->data) {
        this->node = this->order->end_flag;
        return *this;
    }
    if(!this->order->isnull(this->node->right)) {
        next_node = this->node->right;
        while(!this->order->isnull(next_node->left)) {
            next_node = next_node->left;
        }
        this->node = next_node;
        return *this;
    }
    else if(this->node == this->node->parent->left) {
        next_node = this->node->parent;
        this->node = next_node;
        return *this;
    }
    else if(this->node == this->node->parent->right) {
        next_node = this->node;
        while(next_node == next_node->parent->right) {
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
bst_tree<T, bst_tree_nodeallocator >::search_iterator::search_iterator(bstnode_t *node, const T& dst, bst_tree *order_tree) :
bst_tree<T, bst_tree_nodeallocator >::iterator_base::iterator_base(node, order_tree) ,
search_dst(dst) {

}
template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator&
bst_tree<T, bst_tree_nodeallocator >::search_iterator::operator++() {
    assert(this->node!= NULL);
    if(this->search_dst < this->node->data)  {
        if(this->node->left) {
            this->node = this->node->left;
            return *this;
        }
        else {
            this->node = this->order->end_flag;
            return *this;
        }
    }
    else if(this->node->data < this->search_dst) {
        if(this->node->right) {
            this->node = this->node->right;
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
typename bst_tree<T, bst_tree_nodeallocator >::in_order_iterator
bst_tree<T, bst_tree_nodeallocator >::begin() {
    if(this->isnull(head))
        return in_order_iterator(end_flag, this);
    bstnode_t *temp_node = head;
    while(!this->isnull(temp_node->left)) {
        temp_node = temp_node->left;
    }
    return in_order_iterator(temp_node, this);
};

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::in_order_iterator bst_tree<T, bst_tree_nodeallocator >::end() {
    return in_order_iterator(end_flag, this);
}

template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator bst_tree<T, bst_tree_nodeallocator >::search_begin(const T &dst) {
    if(!this->isnull(head)) {
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
    while(tmp!= iter_end) {
        if(*tmp == dst) {
            return tmp;
        } else {
            ++tmp;
        }
    }
    return tmp;
}
template <class T, class bst_tree_nodeallocator >
typename bst_tree<T, bst_tree_nodeallocator >::search_iterator
bst_tree<T, bst_tree_nodeallocator >::find(const T& dst)
{
#if 0
    search_iterator tmp = this->search_begin(dst);
    while(tmp!= this->search_end(dst)) {
        if(*tmp == dst) {
            return tmp;
        } else {
            ++tmp;
        }
    }
    return tmp;
#else
    bstnode_t *node = this->head;
    while(1) {
        if(this->isnull(node))
            return this->search_end(dst);
        if(node->data == dst ) {
            return search_iterator(node, node->data, this);
        } else if(dst < node->data){
            node = node->left;
        } else {
            node = node->right;
        }
    }
#endif
}

template <class T, class bst_tree_nodeallocator >
bst_tree<T, bst_tree_nodeallocator >::bst_tree() :
head(0), end_flag(0), max(0), min(0) {
    init();
};
template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::insert(const T&x) {
    if(this->isnull(this->head)) {
        this->head = this->allocate_node(x);
        this->head->left = this->end_flag;
        this->head->right = this->end_flag;
        return true;
    }
    bstnode_t *next = this->head;
    bstnode_t *new_node = 0;
    while(1) {
        bstnode_t *p = next->parent;
        if(next->data == x) {
            return false;
        } else if(x < next->data) {
            if(this->isnull(next->left)) {
                next->left = this->allocate_node(x);
                next->left->parent = next;
                new_node = next->left;
                break;
            }
            next = next->left;
        } else {
            if(this->isnull(next->right)) {
                next->right = this->allocate_node(x);
                next->right->parent = next;
                new_node = next->right;
                break;
            }
            next = next->right;
        }
    }
    new_node->left = this->end_flag;
    new_node->right = this->end_flag;
    this->try_swap_max((T*)&new_node->data);
    this->try_swap_min((T*)&new_node->data);
    return true;
};

template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::remove(const T &x) {
    if(this->head == this->end_flag)
        return false;
    bstnode_t *next = this->head;
    bstnode_t *temp = 0, *subst = 0;
    bool red = false;
    while(1) {
        if(next->data == x) {
            if(this->isnull(next->left)) {
                temp = next->right;
                subst = next;
            } else if(this->isnull(next->right)) {
                temp = next->left;
                subst = next;
            } else {
                subst = next->right;
                while(!this->isnull(subst->left)) {
                    subst = subst->left;
                }
                if(!this->isnull(subst->left)) {
                    temp = subst->left;
                } else {
                    temp = subst->right;
                }
            }

            if(subst == this->head) {
                this->head = temp;
                this->deallocate_node(next);
                return true;
            }

            if(subst == subst->parent->left) {
                subst->parent->left = temp;
            } else {
                subst->parent->right = temp;
            }
            if(subst == next) {
                temp->parent = subst->parent;
            } else {
                if(subst->parent == next) {
                    temp->parent = subst;
                } else {
                    temp->parent = subst->parent;
                }
                subst->left = next->left;
                subst->right = next->right;
                subst->parent = next->parent;

                if(next == this->head) {
                    this->head = subst;
                } else {
                    if(next == next->parent->left) {
                        next->parent->left = subst;
                    } else {
                        next->parent->right = subst;
                    }
                }

                if(!this->isnull(subst->left)) {
                    subst->left->parent = subst;
                }

                if(!this->isnull(subst->right)) {
                    subst->right->parent = subst;
                }
            }
            this->deallocate_node(next);
            if(red) return true;
            return true;
        } else if(x < next->data){
            next = next->left;
        } else {
            next = next->right;
        }
        if(this->isnull(next))
            return false;
    }
    return true;
};

template <class T, class bst_tree_nodeallocator >
void bst_tree<T, bst_tree_nodeallocator >::init() {
    end_flag = alloc_.allocate(1, 0);
    alloc_.construct(end_flag);
    head = end_flag;
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
template <class T, class bst_tree_nodeallocator >
bool bst_tree<T, bst_tree_nodeallocator >::isnull(bstnode_t *node) {
    return node == this->end_flag;
}

#endif //TREE_FAMILY_BST_TREE_HH
