#pragma once

#include <memory>
#include "Pacman.hpp"
#include "Pinky.hpp"
#include "Blinky.hpp"
#include "Inky.hpp"
#include "Clyde.hpp"
#include "Terrain.hpp"
#include "Logic.hpp"
#include "View.hpp"

// class Pacman;
// class Pinky;
// class Blinky;
// class Inky;
// class Clyde;
// class Terrain;
// class Logic;
// class View;


class Game {
public:
    Game();
    ~Game();

    void launch();

protected:
// à instancier dans le constructeur
    std::shared_ptr<Pacman> m_pacman;
    std::shared_ptr<Pinky> m_pinky;
    std::shared_ptr<Blinky> m_blinky;
    std::shared_ptr<Inky> m_inky;
    std::shared_ptr<Clyde> m_clyde;
    std::shared_ptr<Terrain> m_terrain;
    std::shared_ptr<Logic> m_logic;
    std::shared_ptr<View> m_view;

    int score;


};