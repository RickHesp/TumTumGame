#include "game_state.h"
#include "grid.h"
#include "display.h"
#include <Arduino.h>



GameState currentGameState = STATE_START;

void HandleGameState(GameState gameState)
{   

    switch (gameState)
    {
    case STATE_START:
        currentGameState=STATE_PLACE_BOATS;
        break;

    case STATE_PLACE_BOATS:
        // Code to handle boat placement
        grid_init();
        initCells(own_grid);
        placeBoat(0);
        placeBoat(7);
        fill_grid(own_grid);
        break;

    case STATE_SETUP_GAME:
        // Code to handle game setup like starting player and initializing variables
        grid_init();
        initCells(opp_grid);
        oppPlaceBoat(14);
        oppPlaceBoat(21);
        fill_grid(opp_grid);
        currentGameState=STATE_PLACE_BOATS;
        break;

    case STATE_YOUR_TURN:    
        fill_grid(own_grid);
        break;

    case STATE_OPPONENT_TURN:
        // Code to handle opponent's turn
        break;

    case STATE_GAME_OVER:
        // Code to handle game over
        break;

    default:
        break;
    }
}