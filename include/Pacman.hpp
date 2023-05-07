#pragma once

#include "Entity.hpp"

class Pacman : public Entity{
public:
    Pacman();
    Pacman(double x, double y);
    // ~Pacman();

    void print_position();

    // renvois le nombre de frame restant en "super-pacman"
    int is_energized();

    void set_energy(int energy);
    void decrease_energy();

    void set_next_dir(Direction::Direction dir);
    Direction::Direction get_next_dir();

protected:
    // nbre de frames de "super-pacman", 0 = pas super
    int m_energy_timer;
    
    Direction::Direction m_next_dir;

};