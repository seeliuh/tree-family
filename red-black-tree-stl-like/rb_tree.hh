#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_
#include "bst_tree.hh"

template <class T>
class rb_tree_node : public bst_tree_node<T > {
    public:
        rb_tree_node();
        rb_tree_node(const T &data);
        virtual ~rb_tree_node();
        short color;
        inline void setblack() {color = 0;}
        inline bool isblack() { return (color == 0); }
        inline void setred() {color = 1;}
        inline bool isred() { return (color == 1); }
};

template <class T>
rb_tree_node<T >::rb_tree_node():
    bst_tree_node<T >(),color(1){

    }
template <class T>
rb_tree_node<T >::rb_tree_node(const T &data):
    bst_tree_node<T >(data),color(1){

    }

template<class T>
rb_tree_node<T>::~rb_tree_node()
{
}

template <class T, class rb_tree_nodeallocator = std::allocator<rb_tree_node<T> > >
class rb_tree: public bst_tree<T, rb_tree_nodeallocator >
{
    public:
        typedef rb_tree_node<T > rbnode_t;
        typedef bst_tree_node<T> bstnode_t;
    public:
        virtual bool insert(const T&x);
        virtual bool remove(const T&x);
    protected:
        void left_rotate(rbnode_t *node);
        void right_rotate(rbnode_t *node);
        void insert_fixup(rbnode_t *new_node);
        void remove_fixup(rbnode_t *temp);
    protected:
        virtual rbnode_t *allocate_node(const T &x) {
            return new rbnode_t(x);
        }
        virtual rbnode_t *allocate_node() {
            return new rbnode_t();
        }
        //virtual void deallocate_node(rbnode_t *node) {
            //delete node;
        //}
};

template <class T, class rb_tree_nodeallocator >
bool rb_tree<T, rb_tree_nodeallocator >::insert(const T&x) {
    if(this->isnull(this->head)) {
        this->head = this->allocate_node(x);
        ((rbnode_t*)this->head)->setblack();
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
    this->insert_fixup((rbnode_t*)new_node);
    return true;
}

template <class T, class rb_tree_nodeallocator >
bool rb_tree<T, rb_tree_nodeallocator >::remove(const T&x) {
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
                ((rbnode_t*)temp)->setblack();
                this->deallocate_node(next);
                return true;
            }

            red = ((rbnode_t*)subst)->isred();

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
                ((rbnode_t*)subst)->color = ((rbnode_t*)next)->color;
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
            this->remove_fixup((rbnode_t*)temp);
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
}

template <class T, class rb_tree_nodeallocator >
void rb_tree<T, rb_tree_nodeallocator >::left_rotate(rbnode_t *node) {
    if(this->isnull(node->right))
        return;
    bstnode_t *p = node->parent;
    bstnode_t *y = node->right;
    node->right = y->left;
    if(!this->isnull(y->left))
        y->left->parent = node;
    y->parent = p;
    if(node == this->head)
        this->head = y;
    else if(node == p->left)
        p->left = y;
    else
        p->right = y;
    y->left = node;
    node->parent = y;
}

template <class T, class rb_tree_nodeallocator >
void rb_tree<T, rb_tree_nodeallocator >::right_rotate(rbnode_t *node) {
    if(this->isnull(node->left))
        return;
    bstnode_t *p = node->parent;
    bstnode_t *x = node->left;
    node->left = x->right;
    if(!this->isnull(x->right))
        x->right->parent = node;
    x->parent = p;
    if(node == this->head)
        this->head = x;
    else if(node == p->left)
        p->left = x;
    else
        p->right = x;
    x->right = node;
    node->parent = x;
}

template <class T, class rb_tree_nodeallocator >
void rb_tree<T, rb_tree_nodeallocator >::insert_fixup(rbnode_t *new_node) {
    while(new_node != (rbnode_t*)this->head && ((rbnode_t*)new_node->parent)->isred()) {
        if(new_node->parent == new_node->parent->parent->left) {
            rbnode_t *temp = (rbnode_t *)new_node->parent->parent->right;
            if(temp->isred()) {
                ((rbnode_t*)new_node->parent)->setblack();
                temp->setblack();
                ((rbnode_t*)new_node->parent->parent)->setred();
                new_node = (rbnode_t*)new_node->parent->parent;
            } else {
                if(new_node == new_node->parent->right) {
                    new_node = (rbnode_t*)new_node->parent;
                    this->left_rotate(new_node);
                }
                ((rbnode_t*)new_node->parent)->setblack();
                ((rbnode_t*)new_node->parent->parent)->setred();
                this->right_rotate((rbnode_t*)new_node->parent->parent);
            }
        } else {
            rbnode_t *temp = (rbnode_t *)new_node->parent->parent->left;
            if(temp->isred()) {
                ((rbnode_t*)new_node->parent)->setblack();
                temp->setblack();
                ((rbnode_t*)new_node->parent->parent)->setred();
                new_node = (rbnode_t*)new_node->parent->parent;
            } else {
                if(new_node == new_node->parent->left) {
                    new_node = (rbnode_t*)new_node->parent;
                    this->right_rotate(new_node);
                }
                ((rbnode_t*)new_node->parent)->setblack();
                ((rbnode_t*)new_node->parent->parent)->setred();
                this->left_rotate((rbnode_t*)new_node->parent->parent);
            }
        }
    }
    ((rbnode_t*)this->head)->setblack();
}

template <class T, class rb_tree_nodeallocator >
void rb_tree<T, rb_tree_nodeallocator >::remove_fixup(rbnode_t *temp) {
    while(temp != this->head && temp->isblack()) {
        if(temp == temp->parent->left) {
            rbnode_t *w = (rbnode_t *)temp->parent->right;
            if(w->isred()) {
                w->setblack();
                ((rbnode_t*)temp->parent)->setred();
                this->left_rotate((rbnode_t*)temp->parent);
                w = (rbnode_t*)temp->parent->right;
            }

            if(((rbnode_t*)w->left)->isblack() && ((rbnode_t*)w->right)->isblack()) {
                w->setred();
                temp = (rbnode_t*)temp->parent;
            } else {
                if(((rbnode_t*)w->right)->isblack()) {
                    ((rbnode_t*)w->left)->setblack();
                    w->setred();
                    this->right_rotate(w);
                    w = (rbnode_t*)temp->parent->right;
                }
                w->color = ((rbnode_t*)temp->parent)->color;
                ((rbnode_t*)temp->parent)->setblack();
                ((rbnode_t*)w->right)->setblack();
                this->left_rotate((rbnode_t*)temp->parent);
                temp = (rbnode_t*)this->head;
            }
        } else {
            rbnode_t *w = (rbnode_t*)temp->parent->left;
            if(w->isred()) {
                w->setblack();
                ((rbnode_t*)temp->parent)->setred();
                this->right_rotate((rbnode_t*)temp->parent);
                w = (rbnode_t*)temp->parent->left;
            }

            if(((rbnode_t*)w->left)->isblack() && ((rbnode_t*)w->right)->isblack()) {
                w->setred();
                temp = (rbnode_t*)temp->parent;
            } else {
                if(((rbnode_t*)w->left)->isblack()) {
                    ((rbnode_t*)w->right)->setblack();
                    w->setred();
                    this->left_rotate(w);
                    w = (rbnode_t*)temp->parent->left;
                }
                w->color = ((rbnode_t*)temp->parent)->color;
                ((rbnode_t*)temp->parent)->setblack();
                ((rbnode_t*)w->left)->setblack();
                this->right_rotate((rbnode_t*)temp->parent);
                temp = (rbnode_t*)this->head;
            }
        }
    }
    temp->setblack();
}
#endif
