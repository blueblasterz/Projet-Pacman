#pragma once

#include <array>

/*
Ce fichier contient les paramètres de difficulté du jeu
Notamment : 
* la vitesse de Pacman
* la vitesse des fantômes
* les timings pour le "scatter mode" (fantôme qui arrête de chasser Pacman)
*/
namespace Setting {
    typedef struct level {
        // vitesse de pacman
        double pm_speed;
        // vitesse des fantômes
        double g_speed;
        // vitesse fantome quand Pacman a mangé un SuperDot
        double g_speed_fright;
        // vitesse fantome dans un tunnel 
        double g_speed_tunnel;
        // temps de scatter
        std::array<int,4> scatter;
        // temps de chase
        std::array<int,3> chase;
    };
    // static level level1 {
    //     0.8,
    //     0.75,
    //     0.5,
    //     0.4,
    //     {}
    // }
}


