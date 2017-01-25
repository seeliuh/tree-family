#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <gtest/gtest.h>

#include "../rb_tree.hh"
#include "../bst_tree.hh"
#include "bst_check_tool.hh"
#include "test_data.hh"
#include "rb_tree_debug_tool.hh"

#include <list>

rb_tree<test_data > global_rb;
std::vector<test_data > global_vecele;
class RBTreeTestSuite : public testing::Test {
    protected:
        static void SetUpTestCase() {
            std::cout << "RBTreeTestSuite::SetUpTestCase()" << std::endl;
        }
        static void TearDownTestCase() {
            std::cout << "RBTreeTestSuite::TearDownTestCase()" << std::endl;
        }
};

TEST_F(RBTreeTestSuite, insert){
    rbtree_tool::gen_test_tree(global_rb, 100000, global_vecele);
    rbtree_tool::print_rbtree(&global_rb);
}

TEST_F(RBTreeTestSuite, check){
    typedef rb_tree<test_data >::rbnode_t rbnode_t;
    std::list<rbnode_t* > lt;
    std::list<int > lrc; //the black element count of root to current node
    std::list<int > lres; //the black element count of root to every leaf path
    rbnode_t *node = (rbnode_t*)global_rb.head;
    ASSERT_EQ(node->isblack(), true); // the root must be black
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
        if(node->left == global_rb.end_flag && node->right == global_rb.end_flag) {
            lres.push_back(blackbase);
        } else {
            if(node->left != global_rb.end_flag) {
                lt.push_back((rbnode_t*)node->left);
                lrc.push_back(blackbase);
            }
            if(node->right!= global_rb.end_flag) {
                lt.push_back((rbnode_t*)node->right);
                lrc.push_back(blackbase);
            }
            if(node->isred()) {
                if(node->left != global_rb.end_flag) {
                    ASSERT_EQ(true, ((rbnode_t*)node->left)->isblack()) << " data:" << node->data.index; //if the node is red, children must be black
                }
                if(node->right!= global_rb.end_flag) {
                    ASSERT_EQ(true, ((rbnode_t*)node->right)->isblack())<< " data:" << node->data.index;//if the node is red, children must be black
                }
            }
        }
    }

    int last = lres.front();
    //std::cout << "count: ";
    for(int blackcount : lres) {
        ASSERT_EQ(blackcount, last); //the black element count of root to every leaf path must be same
        last = blackcount;
        //std::cout << " " << blackcount ;
    }
    //std::cout << std::endl;
}

TEST_F(RBTreeTestSuite, in_order_iterator_test){
    std::cout << "in_order_iterator_test:";
    auto func_printf = [](const test_data& x)
    { std::cout << x << " ";} ;
    std::for_each(global_rb.begin(), global_rb.end(), func_printf);
    std::cout << std::endl;
}

TEST_F(RBTreeTestSuite, search){
    for(test_data &d: global_vecele) {
        auto end = global_rb.search_end(d);
        ASSERT_EQ( (global_rb.find(d)!= end), true);
    }
}


TEST_F(RBTreeTestSuite, remove){
    while(!global_vecele.empty()) {
        test_data element = global_vecele.back();
        global_rb.remove(element);
        //std::cout << "remove result = " << global_rb.remove(element) << ", " << element << std::endl;
        global_vecele.pop_back();
        //ASSERT_EQ(rbtree_tool::check_rbtree(global_rb), true);
    }
}
