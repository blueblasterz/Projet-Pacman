#include <iostream>
#include "lib_test.hpp"

using std::cout, std::cin, std::endl;

int main() {
    cout << "COUCOUCOUCOUCOUCOCUOCUOCUCOU" << endl;

    Lib_test l(5);
    cout << l.get_a() << endl;
    l.set_a(-5);
    cout << l.get_a() << endl;
    l.set_a(2);
    cout << l.get_a() << endl;

    return 0;
}