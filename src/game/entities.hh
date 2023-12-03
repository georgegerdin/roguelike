//
// Created by george on 2023-12-03.
//

#ifndef ROGUELIKE_ENTITIES_HH
#define ROGUELIKE_ENTITIES_HH


namespace Entities {
    struct Player {
        const char* type() const {return "Player";}
    };

    struct Flower {
        const char* type() const {return "Flower";}
    };
}

#include "entities_meta.hh"

#endif //ROGUELIKE_ENTITIES_HH
