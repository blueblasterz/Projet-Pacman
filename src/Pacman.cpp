#include "Pacman.hpp"

#include <iostream>

Pacman::Pacman() : Pacman(104.0,204.0){}

Pacman::Pacman(double x, double y) 
: Entity(x,y),
  m_energy_timer(0),
  m_next_dir(Direction::Left) {
    this->set_speed(0.8);
}

// Pacman::~Pacman() {}


void Pacman::print_position() {
    std::cout << m_x << " " << m_y << std::endl;
}

int Pacman::is_energized() {
    return m_energy_timer;
}

void Pacman::set_energy(int energy) {
    if (energy < 0)
        m_energy_timer = 0;
    else
        m_energy_timer = energy;
}
void Pacman::decrease_energy() {
    if (m_energy_timer > 0) {
        m_energy_timer -= 1;
    }
}

void Pacman::set_next_dir(Direction::Direction dir) {
    m_next_dir = dir;
}
Direction::Direction Pacman::get_next_dir() {
    return m_next_dir;
}
