#include "Pacman.hpp"

#include <iostream>

Pacman::Pacman() : Pacman(104.0,204.0){}

Pacman::Pacman(double x, double y) 
: Entity(x,y),
  m_energy_timer(0){
    this->set_speed(0.8);
}

// Pacman::~Pacman() {}


void Pacman::print_position() {
    std::cout << m_x << " " << m_y << std::endl;
}

bool Pacman::is_energized() {
    return m_energy_timer > 0;
}
