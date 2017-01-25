#include "test_data.hh"
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
    index = rand()%1000000;
}

std::ostream& operator << (std::ostream& os, const test_data& x) {
    os << x.index;
    return os;
}
bool operator < (const test_data& left, const test_data& right) {
    return left.index < right.index;
}

