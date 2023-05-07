#pragma once

#include <memory>
#include <array>

// #define DEFAULT_SPEED 75.75757625/60 
#define DEFAULT_SPEED 1.0 

namespace Direction {
    enum Direction : int {
        Left,
        Right,
        Up,
        Down
    };
    Direction reverse(Direction dir);
    std::pair<int,int> tile_in_dir(std::pair<int,int> tile, Direction dir);
}


class Entity {
public:
    Entity();
    // Entity(const Entity & e);
    Entity(double x, double y);
    ~Entity();

    double get_x();
    double get_y();
    std::pair<double,double> get_pos();
    void set_x(double x);
    void set_y(double y);
    void set_pos(double x, double y);
    void set_pos(std::pair<double,double>);

    // ahead pour renvoyer la case n cases dans la direction actuelle
    // prend en compte le "bug" de quand on regarde vers le haut
    std::pair<int,int> get_tile(int ahead=0);

    void set_tile(int x, int y);
    void set_tile(std::pair<int,int> pos);

    Direction::Direction get_direction();
    void set_direction(Direction::Direction dir);

    void move(double dx, double dy);

    // renvois la distance (en pixels) entre la case de this et la case de l'entité donnée
    double dist(std::shared_ptr<Entity> e) const;
    
    // renvois la distance (en pixels) entre la case de this et la case de la position donnée
    double dist(double x, double y) const;
    double dist(std::pair<double,double> pos) const;

    double get_speed();
    void set_speed(double speed);

    // renvois vrai si m_frame_stopped != 0, et réduit de 1 m_frame_stopped
    // renvois faux sinon
    bool is_stopped();
    // ajoute des frame d'immobilité à l'entitée
    void add_stopped(int add);

    // vitesse par défaut, en px/frame (ne dépend pas de l'entité)
    static constexpr double default_speed = DEFAULT_SPEED;
protected:
    double m_x;
    double m_y;

    // le centre de l'entité
    // (pour gérer les colisions entre fantome/pacman)
    int m_tile_x; 
    int m_tile_y;


    // pourcentage de la vitesse par défaut
    double m_speed;
    
    // la direction vers laquelle l'entité regarde
    Direction::Direction m_direction;

    // nombre de frame où l'entitée ne bouge pas
    // si != 0, est réduit de 1 à la fin d'une frame
    // si 0, l'entitée peut bouger normalement
    int m_frame_stopped;

};

