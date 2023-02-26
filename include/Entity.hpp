#pragma once

#include <array>

class Entity {
public:
    Entity();
    Entity(double x, double y);
    ~Entity();

    bool is_energized();

    double get_x();
    double get_y();
    void set_x(double x);
    void set_y(double y);

    std::array<int,2> get_tile();

    void move(double dx, double dy);


protected:
    double m_x;
    double m_y;

    // le centre de l'entité
    // (pour gérer les colisions entre fantome/pacman)
    int m_tile_x; 
    int m_tile_y;

    // la vitesse (en pixel/frame ? ou pixel/seconde ?)
    double speed;
    
    // type SDL pour l'image / sprite, truc à afficher ?

};