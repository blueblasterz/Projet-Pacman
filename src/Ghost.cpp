#include "Ghost.hpp"

Ghost::Ghost() : Ghost(0.0,0.0){}

Ghost::Ghost(double x, double y) 
: Entity(x,y) {
    
}

Ghost::~Ghost() {}

std::pair<int,int> Ghost::get_target() {
    return target;
}