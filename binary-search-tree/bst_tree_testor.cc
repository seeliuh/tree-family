/*
 * tree_test.cpp
 *
 *  Created on: Jul 12, 2015
 *      Author: liuhang
 */

#include <iostream>
#include <fstream>
#include <algorithm>

#include "bst_tree.hh"

class test_data
{
public:
    test_data();
    test_data(int _index, const std::string& _name);
public:
    friend bool operator < (const test_data& left, const test_data& right);
    bool operator == (const test_data& right) {
        return this->index == right.index;
    }
    friend std::ostream& operator << (std::ostream& os, const test_data& x );
    void random();
private:
    int index;
    std::string name;
};
test_data::test_data()
        : index(0), name("")
{

}
test_data::test_data(int _index, const std::string& _name)
        : index(_index),name(_name)
{

}
void test_data::random()
{
    index = rand()%1000;
}

std::ostream& operator << (std::ostream& os, const test_data& x) {
    os << x.index;
    return os;
}
bool operator < (const test_data& left, const test_data& right) {
    return left.index < right.index;
}

std::ofstream success_log;
std::ofstream failed_log;

void gen_test_tree(bst_tree<test_data>& tr)
{
    for (int i = 0; i< 100; i++) {
        std::cout << i << std::endl;
        test_data _tmp;
        _tmp.random();
        if(!tr.insert(_tmp)) {
            failed_log << i << ", " << _tmp << std::endl ;
        }
        else {
            success_log << i << ", " << _tmp << std::endl ;
        }
    }
}

int main(int argc, char **argv) {
    success_log.open("./success.log" );
    failed_log.open("./failed.log" );
    bst_tree<test_data> bst;
    gen_test_tree(bst);
    std::cout << *bst.max;
    int input;
    std::cin >> input;
    std::for_each(bst.begin(), bst.end(), [](const test_data& x)
    { std::cout << x << std::endl;}
    );
    std::cout << "done";
}

