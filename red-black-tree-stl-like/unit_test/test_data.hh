#ifndef __TEST_DATA_H__
#define __TEST_DATA_H__
#include <string>
#include <iostream>
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
    int index;
    std::string name;
};
#endif
