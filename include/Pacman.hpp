#pragma once

#include "Entity.hpp"

class Pacman : public Entity{
public:
    Pacman();
    Pacman(double x, double y);
    ~Pacman();

    bool is_energized();


protected:
    // nbre de frames de "super-pacman", 0 = pas super
    int m_energy_timer;
    

};