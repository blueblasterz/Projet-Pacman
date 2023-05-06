#pragma once
#include "Ghost.hpp"

// pink Ghost
class Pinky : public Ghost {
public:
    Pinky(
        double x,
        double y,
        std::shared_ptr<Pacman> &pacman);
    ~Pinky();
    void compute_target();

protected:

};