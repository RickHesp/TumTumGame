#ifndef GAME_STATE_H
#define GAME_STATE_H

enum GameState {
    STATE_START,
    STATE_PLACE_BOATS,
    STATE_SETUP_GAME,
    STATE_YOUR_TURN,
    STATE_OPPONENT_TURN,
    STATE_GAME_OVER
};

extern GameState currentGameState;

void HandleGameState(GameState GameState);
#endif
