#pragma once

#include "Entity.hpp"

class Player : public Entity{
public:
    Player();
    Player(double x, double y);
    ~Player();

    bool is_energized();


protected:
    // nbre de frames de "super-pacman", 0 = pas super
    int m_energy_timer;
    

};