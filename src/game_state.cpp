#include "game_state.h"
#include "grid.h"
#include "display.h"
#include "touch.h"
#include "irreceiver.h"
#include "sendcommand.h"
#include "rc5decoder.h"
#include <Adafruit_ILI9341.h>
#include <Arduino.h>



GameState currentGameState = STATE_START;
bool startingPlayer = false; //true if this device starts first

void HandleGameState(GameState gameState)
{   
    switch (gameState)
    {
    case STATE_START:{
        Startscreen_init();
        
        if(screen_touched()){ //wait for screen touch or ir signal to start game
            grid_init();
            startingPlayer = true;
            //stuur signaal om te beginnen naar andere speler de eerste die het scherm aanraakt is ook als eerst aan de beurt
            send_command(1, 1, 1); //field 1, address 1, command 1=start game
            currentGameState=STATE_PLACE_BOATS;
        }else{
            //wait for ir signal from other player to start game
            decode_ir();
        }
        
        break;
}
    case STATE_PLACE_BOATS:{
        // Code to handle boat placement
        grid_init();
        initCells(own_grid);
        placeBoat(0);
        placeBoat(7);
        fill_grid(own_grid);
        if(startingPlayer){
            currentGameState= STATE_YOUR_TURN;
        }else{
            currentGameState= STATE_OPPONENT_TURN;
        }
        break;
    }

    case STATE_SETUP_GAME: {
        // Code to handle game setup like starting player and initializing variables
        grid_init();
        initCells(opp_grid);
        oppPlaceBoat(14);
        oppPlaceBoat(21);
        fill_grid(opp_grid);
        currentGameState=STATE_PLACE_BOATS;
        break;
    }

    case STATE_YOUR_TURN: {   
        fill_grid(opp_grid);
        for(uint8_t i=0; i<36; i++){        //draw cross if hit
            if(opp_grid[i].hit==1){
                draw_cross(i, ILI9341_RED);
            }
        }
        currentGameState=STATE_OPPONENT_TURN;
        
        break;
    }

    case STATE_OPPONENT_TURN:{
        bool hit = 0; //placeholder for hit detection
        // Code to handle opponent's turn
        //recieve ir code and update board
        decode_ir();
        send_command(1,3, hit); //placeholder command to send hit/miss info back
        fill_grid(own_grid);
        currentGameState=STATE_YOUR_TURN;

        break;
    }

    case STATE_GAME_OVER:{
        int amount_boats = 0; //placeholder for boat counting 
        // Code to handle game over
        if(amount_boats == 0){ //placeholder for win/loss detection
            endscreen_init(false); //lost
        }else{
            endscreen_init(true); //won
        }
        break;
    }

    default:{
    break;
}
    }
}