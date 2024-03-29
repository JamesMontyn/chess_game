#ifndef GAME_H 
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdint.h>
#include <vector>

#include "state.h"

class Game {
    public: 
        Game();

        ~Game();

        State* state;

        void setGamemode(bool const color, int const gamemode);

        std::vector<int>* getLegalMoves(uint8_t const pieceByte, int const pos);

    private:
        int gamemodeWhite;
        int gamemodeBlack;

        bool isPosAttacked(State* state, int const pos, bool const colorOfAttacker);

        std::vector<int>* getPossibleMoves(State* state, uint8_t const pieceByte, int const pos);


};

#endif