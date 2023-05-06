#pragma once
#include "Ghost.hpp"

// yellow Ghost
class Clyde : public Ghost {
public:
    Clyde(
        double x,
        double y,
        std::shared_ptr<Pacman> &pacman);
    ~Clyde();
    void compute_target();

protected:

};