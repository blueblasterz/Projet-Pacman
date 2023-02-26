#include "Entity.hpp"

#include <iostream>
#include <cmath>

using std::array, std::cout, std::cin, std::endl;

Entity::Entity() : Entity(0.0,0.0) {}

Entity::Entity(double x, double y) 
: m_x(x), m_y(y) {
    m_tile_x = round(m_x);
    m_tile_y = round(m_y);
}

Entity::~Entity() {}


double Entity::get_x() {
    return m_x;
}
double Entity::get_y() {
    return m_y;
}

void Entity::set_x(double x) {
    m_x = x;
}
void Entity::set_y(double y) {
    m_y = y;
}
void Entity::move(double dx, double dy) {
    this->set_x(this->get_x() + dx);
    this->set_y(this->get_y() + dy);
}

array<int,2> Entity::get_tile() {
    return {m_tile_x, m_tile_y};
}