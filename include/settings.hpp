#pragma once

#include <array>

/*
Ce fichier contient les paramètres de difficulté du jeu
Notamment : 
* la vitesse de Pacman
* la vitesse des fantômes
* les timings pour le "scatter mode" (fantôme qui arrête de chasser Pacman)
* la durée des SuperDots

On ignore quelques règles un peu pointues
Par exemple, Blinky est censé accélerer un petit peu quand il ne reste
plus beaucoup de points à manger.

*/
namespace Setting {
    struct level {
        // vitesse de pacman
        double pm_speed;
        // vitesse des fantômes
        double g_speed;
        // vitesse fantome quand Pacman a mangé un SuperDot
        double g_speed_fright;
        // vitesse fantome dans un tunnel 
        double g_speed_tunnel;
        // temps de scatter en secondes (0 = 1 frame)
        std::array<int,4> scatter;
        // temps de chase
        std::array<int,3> chase;
        // durée des SuperDots, en seconde
        int fright_time;
    };
    const level level1 {
        0.8,
        0.75,
        0.5,
        0.4,
        {7,34,59,84},
        {27,54,79},
        6
    };
    const level level2_4 {
        0.9,
        0.85,
        0.55,
        0.45,
        {7,34,59,1092},
        {27,54,1092},
        5
    };
    const level level5_20 {
        1,
        0.95,
        0.6,
        0.5,
        {5,30,55,1092},
        {25,50,1092},
        3
    };
    const level level21 {
        0.9,
        0.95,
        1, // plus de fright à ce niveau là
        0.5,
        {5,30,55,1092},
        {25,50,1092},
        0

    };
}


