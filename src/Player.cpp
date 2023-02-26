#include "Player.hpp"


Player::Player() : Player(0.0,0.0){}

Player::Player(double x, double y) 
: Entity(x,y), m_energy_timer(0) {
    
}

Player::~Player() {}


bool Player::is_energized() {
    return m_energy_timer > 0;
}

