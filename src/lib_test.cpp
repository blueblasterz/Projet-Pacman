#include "lib_test.hpp"


Lib_test::Lib_test(int a) : m_a(a) {}

Lib_test::~Lib_test() {}

int Lib_test::get_a() {
    return m_a;
}

void Lib_test::set_a(int a) {
    if(a <0) {
        m_a = 0;
    }
    else {
        m_a = a;
    }
}