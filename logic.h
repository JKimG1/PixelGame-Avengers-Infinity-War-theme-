#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

typedef struct {
    int hp;
    int attack;
    int x;
    int y;
} Avenger;

typedef struct {
    int x;
    int y;
    int shot;
} AvengerLaser;

typedef struct {
    int hp;
    int attack;
    int x;
    int y;
    int up;
} Enemy;

typedef struct {
    int x;
    int y;
    int shot;
} EnemyLaser;

typedef struct {
    int x;
    int y;
} Arrow;


typedef struct {
    Avenger hero; 
    Enemy enemy;
    
    // Stores the hero's attack
    AvengerLaser al;
    // Stores the enemy's attack
    EnemyLaser el;
    
    int character_index; // 0 = Ironman, 1 = Captain, 2 = Thor
    int character_selected; // 0 = not_selected, 1 = selected
    Arrow character_arrow;
    
    // Store whether or not the game is over in this member:
    int gameOver;
    int won;
} AppState;


// This function can initialize an unused AppState struct's character.
void initializeCharacter(AppState* appState);

// This function will be used to process character frames.
AppState processCharacterAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
