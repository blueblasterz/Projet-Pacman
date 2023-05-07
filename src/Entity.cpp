#include "Entity.hpp"

#include <iostream>
#include <cmath>

using std::array, std::cout, std::cin, std::endl;

Direction::Direction Direction::reverse(Direction dir) {
    switch(dir) {
        case Direction::Up: return Direction::Down;
        case Direction::Down: return Direction::Up;
        case Direction::Right: return Direction::Left;
        case Direction::Left: return Direction::Right;
    }
    return Direction::Left;
}

std::pair<int,int> Direction::tile_in_dir(
    std::pair<int,int> tile, Direction dir) {
    switch(dir) {
        case Direction::Up: return {tile.first, tile.second-1};
        case Direction::Down: return {tile.first, tile.second+1};
        case Direction::Left: return {tile.first-1, tile.second};
        case Direction::Right: return {tile.first+1, tile.second};
    }
    return {-1,-1};
}

std::ostream& operator<<(std::ostream& os, const Direction::Direction& dir) {
    switch(dir) {
        case Direction::Left: os << "Left"; break;
        case Direction::Right: os << "Right"; break;
        case Direction::Up: os << "Up"; break;
        case Direction::Down: os << "Down"; break;
    }
    return os;
}



Entity::Entity() : Entity(0.0,0.0) {}


Entity::Entity(double x, double y) 
: m_x(x),
  m_y(y),
  m_speed(0),
  m_direction(Direction::Left),
  m_frame_stopped(0) {
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

std::pair<double,double> Entity::get_pos() {
    return {m_x,m_y};
}

void Entity::set_x(double x) {
    m_x = x;
    m_tile_x = int(x)/8;
}
void Entity::set_y(double y) {
    m_y = y;
    m_tile_y = floor(y)/8;
}
void Entity::set_pos(double x, double y) {
    this->set_x(x);
    this->set_y(y);
}
void Entity::set_pos(std::pair<double,double> pos) {
    this->set_x(pos.first);
    this->set_y(pos.second);
}

void Entity::move(double dx, double dy) {
    this->set_x(this->get_x() + dx);
    this->set_y(this->get_y() + dy);
}

std::pair<int,int> Entity::get_tile(int ahead) {
    if(ahead == 0)
        return {m_tile_x, m_tile_y};
    switch(m_direction) {
        case Direction::Up :
            return {m_tile_x-ahead, m_tile_y-ahead};
        case Direction::Down :
            return {m_tile_x, m_tile_y+ahead};
        case Direction::Left :
            return {m_tile_x-ahead, m_tile_y};
        case Direction::Right:
            return {m_tile_x+ahead, m_tile_y};
        default:
            return {m_tile_x, m_tile_y};
            break;
    }
}

void Entity::set_tile(int x, int y) {
    m_tile_x = x;
    m_tile_y = y;
    m_x = x*8 + 4;
    m_y = y*8 + 4;
}
void Entity::set_tile(std::pair<int,int> pos) {
    this->set_tile(pos.first, pos.second);
}

Direction::Direction Entity::get_direction() {
    return m_direction;
}

void Entity::set_direction(Direction::Direction dir) {
    m_direction = dir;
}

double Entity::dist(double x, double y) const {
    // cout << m_x << " " << m_y << endl;
    // cout << x << " " << y << endl;
    return sqrt( pow(floor(m_x/8)*8-floor(x/8)*8,2) 
               + pow(floor(m_y/8)*8-floor(y/8)*8,2) );
}
double Entity::dist(std::pair<double,double> pos) const {
    return this->dist(pos.first, pos.second);
}

double Entity::dist(std::shared_ptr<Entity> e) const {
    return this->dist(e->get_pos());
}

double Entity::get_speed() {
    return m_speed*default_speed;
}

void Entity::set_speed(double speed) {
    if (speed < 0) {
        m_speed = 0;
    }
    else if (speed > 1) {
        m_speed = 1;
    }
    else {
        m_speed = speed;
    }
}

bool Entity::is_stopped() {
    if (m_frame_stopped != 0) {
        m_frame_stopped -= 1;
        return true;
    }
    return false;
}

void Entity::add_stopped(int add) {
    if(m_frame_stopped + add > 0)
        m_frame_stopped += add;
    else
        m_frame_stopped = 0;
}