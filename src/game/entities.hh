//
// Created by george on 2023-12-03.
//

#ifndef ROGUELIKE_ENTITIES_HH
#define ROGUELIKE_ENTITIES_HH


namespace Entities {
    struct Player {
        const char* type() const {return "Player";}
        void position(int* x, int* y) const {
            *x = m_x;
            *y = m_y;
        }

        char symbol() const {return '@';}

        void set_position(int x, int y) {
            m_x = x;
            m_y = y;
        }

        int m_x = 0;
        int m_y = 0;
    };

    struct Flower {
        const char* type() const {return "Flower";}
        void position(int* x, int* y) const {
            *x = m_x;
            *y = m_y;
        }

        char symbol() const {return 'f';}

        void set_position(int x, int y) {
            m_x = x;
            m_y = y;
        }

        int m_x = 0;
        int m_y = 0;
    };
}

#include "entities_meta.hh"

#endif //ROGUELIKE_ENTITIES_HH
