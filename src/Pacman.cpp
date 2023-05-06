#include "Pacman.hpp"


Pacman::Pacman() : Pacman(0.0,0.0){}

Pacman::Pacman(double x, double y) 
: Entity(x,y), m_energy_timer(0) {
    
}

Pacman::~Pacman() {}


bool Pacman::is_energized() {
    return m_energy_timer > 0;
}

