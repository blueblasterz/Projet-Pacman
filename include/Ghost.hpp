#pragma once

#include "Entity.hpp"

class Ghost : public Entity{
public:
    Ghost();
    Ghost(double x, double y);
    ~Ghost();

    virtual void compute_target(
        Entity & pacman_pos,
        bool scatter=false
    ) =0;
    
    std::pair<int,int> get_target();



protected:
    std::pair<int,int> target;
    std::pair<int,int> scatter_target;

};