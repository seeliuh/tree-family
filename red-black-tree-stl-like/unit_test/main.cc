/*
 * tree_test.cpp
 *
 *  Created on: Jul 12, 2015
 *      Author: liuhang
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <gtest/gtest.h>

#include "../rb_tree.hh"
#include "../bst_tree.hh"
#include "bst_check_tool.hh"
//#include "test_data.hh"
#include "rb_tree_debug_tool.hh"
//#include "rbtree_testor.hh"

typedef rb_tree<test_data> tree_t;
//typedef bst_tree<test_data> tree_t;
int main(int argc, char **argv) {
    std::cout << "done";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
