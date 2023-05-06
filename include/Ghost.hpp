#pragma once

#include <memory>

#include "Entity.hpp"

class Pacman;

class Ghost : public Entity{
public:
    Ghost();
    Ghost(
        double x,
        double y,
        std::shared_ptr<Pacman> &pacman,
        std::pair<int,int> scatter_pos);
    Ghost(
        std::pair<int,int> pos,
        std::shared_ptr<Pacman> &pacman,
        std::pair<int,int> scatter_pos);
    ~Ghost();

    virtual void compute_target() =0;
    
    std::pair<int,int> get_target();

    // renvois la distance (en pixels) entre this et pacman
    double dist_pacman();



protected:
    // contient la case visée par le fantome actuellement
    std::pair<int,int> m_target;
    
    // contient la case que le fantome vise quand il est en mode "scatter"
    std::pair<int,int> m_scatter_target;
    
    // pour savoir où est le pacman
    std::shared_ptr<Pacman> m_pacman;

    // true si le fantome est en "scatter", false sinon
    bool m_scatter;

};