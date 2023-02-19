#pragma once


// truc inutile, pour vérifier que le Makefile fonctionne
class Lib_test {
public:
    Lib_test(int a=0);
    ~Lib_test();

    int get_a();
    void set_a(int a=0);

protected:
    int m_a;


};