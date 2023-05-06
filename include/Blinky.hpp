#pragma once
#include "Ghost.hpp"

// red Ghost
class Blinky : public Ghost {
public:
    Blinky(
        double x,
        double y,
        std::shared_ptr<Pacman> &pacman);
    ~Blinky();

    void compute_target();
protected:

};