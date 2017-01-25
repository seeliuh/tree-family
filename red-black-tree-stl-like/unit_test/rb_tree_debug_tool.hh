#ifndef _RB_TREE_DEBUG_TOOL_H_
#define _RB_TREE_DEBUG_TOOL_H_
#include <iostream>
#include <vector>
#include <list>

#include "test_data.hh"
#include "../rb_tree.hh"

typedef rb_tree<test_data> tree_t;
class rbtree_tool {
    public:
        static void gen_test_tree(tree_t& tr, int elementCount, std::vector<test_data> &vecElement)
        {
            for (int i = 0; i< elementCount; i++) {
                test_data _tmp;
                _tmp.random();
                vecElement.push_back(_tmp);
                if(!tr.insert(_tmp)) {
                    std::cout << "[error]" << i << ", " << _tmp << std::endl ;
                }
                else {
                    //std::cout << i << ", " << _tmp << std::endl ;
                }
            }
        }
        static void print_rbtree(rb_tree<test_data> *tr) {
            std::list<tree_t::bstnode_t* > prs;
            std::list<tree_t::bstnode_t* > prs2;
            tree_t::rbnode_t *next = (tree_t::rbnode_t*)tr->head;
            prs.push_back(tr->head);
            while(!prs.empty()) {
                prs2 = prs;
                prs.clear();
                while(!prs2.empty()) {
                    tree_t::rbnode_t* node = (tree_t::rbnode_t*)prs2.front();
                    prs2.pop_front();
                    if(node != tr->end_flag) {
                        std::cout << node->data << "(" << node->color<< ")";
                        prs.push_back(node->left);
                        prs.push_back(node->right);
                    }
                }
                std::cout << std::endl;
            }
        }
        static bool check_rbtree(rb_tree<test_data> &tr) {
            typedef rb_tree<test_data >::rbnode_t rbnode_t;
            std::list<rbnode_t* > lt;
            std::list<int > lrc; //the black element count of root to current node
            std::list<int > lres; //the black element count of root to every leaf path
            rbnode_t *node = (rbnode_t*)tr.head;
            if(node == tr.end_flag) return true;
            if(!node->isblack()) return false;
            lt.push_back(node);
            lrc.push_back(1);
            while(!lt.empty()) {
                node = lt.front();
                int bbc = lrc.front();
                lt.pop_front();
                lrc.pop_front();
                int blackbase = bbc;
                if(node->isblack())
                    blackbase++;
                if(node->left == tr.end_flag && node->right == tr.end_flag) {
                    lres.push_back(blackbase);
                } else {
                    if(node->left != tr.end_flag) {
                        lt.push_back((rbnode_t*)node->left);
                        lrc.push_back(blackbase);
                    }
                    if(node->right!= tr.end_flag) {
                        lt.push_back((rbnode_t*)node->right);
                        lrc.push_back(blackbase);
                    }
                    if(node->isred()) {
                        if(node->left != tr.end_flag) {
                            if(!((rbnode_t*)node->left)->isblack()) return false;
                        }
                        if(node->right!= tr.end_flag) {
                            if(!((rbnode_t*)node->right)->isblack()) return false;
                        }
                    }
                }
            }

            int last = lres.front();
            //std::cout << "count: ";
            for(int blackcount : lres) {
                if(blackcount!=last) return false;
                last = blackcount;
                //std::cout << " " << blackcount ;
            }
            //std::cout << std::endl;
            return true;
        }

};

#endif
