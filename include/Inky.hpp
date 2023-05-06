#pragma once
#include "Ghost.hpp"

class Blinky;

// Cyan ghost
class Inky : public Ghost { 
public:
    Inky(
        double x,
        double y,
        std::shared_ptr<Pacman> &pacman,
        std::shared_ptr<Blinky> &blinky);
    ~Inky();
    void compute_target();

protected:
    std::shared_ptr<Blinky> m_blinky;
};