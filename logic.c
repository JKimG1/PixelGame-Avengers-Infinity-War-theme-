#include "logic.h"

void initializeCharacter(AppState *appState) {
    appState->character_arrow.x = 46;
    appState->character_arrow.y = 40;
    appState->character_selected = 0; // initially, not_selected
    appState->character_index = 0; // initially, Ironman
}


void initializeAppState(AppState *appState) {
    appState->gameOver = 0; // initially, false
    appState->hero.hp = 100;
    appState->hero.attack = 10;
    appState->hero.x = 20;
    appState->hero.y = 100;
    
    appState->al.shot = 0; // initially, false
    appState->el.shot = 0; // initially, false
    
    appState->enemy.hp = 100;
    appState->enemy.attack = 10;
    appState->enemy.x = 180;
    appState->enemy.y = 100;
    appState->enemy.up = 0;
}

// This function processes your current character app state and returns the new (i.e. next)
// state of your application.
AppState processCharacterAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    
    AppState nextAppState = *currentAppState;
    
    // When index is 0 and right arrow is pushed, move to index 1
    // When index is 1 and right arrow is pushed, move to index 2
    // When index is 2 and left arrow is pushed, move to index 1
    // When index is 1 and left arrow is pushed, move to index 0
    if (KEY_JUST_PREESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
        if (nextAppState.character_index == 0) {
            nextAppState.character_arrow.x = 115;
            nextAppState.character_index = 1;
        } else if (nextAppState.character_index == 1) {
            nextAppState.character_arrow.x = 185;
            nextAppState.character_index = 2;
        }
    } else if (KEY_JUST_PREESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore)) {
        if (nextAppState.character_index == 2) {
            nextAppState.character_arrow.x = 115;
            nextAppState.character_index = 1;
        } else if (nextAppState.character_index == 1) {
            nextAppState.character_arrow.x = 46;
            nextAppState.character_index = 0;
        }
    } else if (KEY_JUST_PREESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        nextAppState.character_selected = 1;
    }
    
    return nextAppState;
}
// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */
    
    AppState nextAppState = *currentAppState;
    
    // move hero to right
    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {
        if (nextAppState.hero.x + 66 < 120) {
            nextAppState.hero.x += 2;
        }
    }
    // move hero to left
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {
        if (nextAppState.hero.x - 2 > 0) {
            nextAppState.hero.x -= 2;
        }
    }
    // move hero up
    if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {
        if (nextAppState.hero.y-2 > 0) {
            nextAppState.hero.y -= 2;
        }
    }
    // move hero down
    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)) {
        if (nextAppState.hero.y + 66 < 160) {
            nextAppState.hero.y += 2;
        }
    }
    // Attack
    if (KEY_JUST_PREESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        if (nextAppState.al.shot == 0) {
            nextAppState.al.x = nextAppState.hero.x + 50;
            nextAppState.al.y = nextAppState.hero.y + 40;
        }
        nextAppState.al.shot = 1;
    }
    
    // This portion controls the hero's attack. (bound checking for hit, if not, move the position)
    int hit_al = 0;
    if (nextAppState.al.shot == 1 &&
        nextAppState.al.y < nextAppState.enemy.y + 64 &&
        nextAppState.al.y + 5 > nextAppState.enemy.y &&
        nextAppState.al.x < nextAppState.enemy.x + 64 &&
        nextAppState.al.x + 5 > nextAppState.enemy.x) {
        hit_al = 1;
    } else if (nextAppState.al.shot == 1) {
        nextAppState.al.x += 3;
    }
    
    if (nextAppState.al.shot == 1 && (nextAppState.al.x + 5 >= 240 || hit_al == 1)) {
        nextAppState.al.shot = 0;
        nextAppState.al.x = 0;
        nextAppState.al.y = 0;
    }
    
    if (hit_al) {
        nextAppState.enemy.hp -= nextAppState.hero.attack;
    }
    
    
    // This portion controls the enemy's attack. Enemy's attack is shot every 25 frames.
    if (vBlankCounter % 25 == 0) {
        if (nextAppState.el.shot == 0) {
            nextAppState.el.x = nextAppState.enemy.x - 10;
            nextAppState.el.y = nextAppState.enemy.y + 35;
        }
        nextAppState.el.shot = 1;
    }
    
    int hit_el = 0;
    if (nextAppState.el.shot == 1 &&
        nextAppState.el.y < nextAppState.hero.y + 64 &&
        nextAppState.el.y + 5 > nextAppState.hero.y &&
        nextAppState.el.x < nextAppState.hero.x + 64 &&
        nextAppState.el.x + 5 > nextAppState.hero.x) {
        hit_el = 1;
    } else if (nextAppState.el.shot == 1) {
        nextAppState.el.x -= 3;
    }
    
    if (nextAppState.el.shot == 1 && (nextAppState.el.x <= 0 || hit_el == 1)) {
        nextAppState.el.shot = 0;
        nextAppState.el.x = 0;
        nextAppState.el.y = 0;
    }

    if (hit_el) {
        nextAppState.hero.hp -= nextAppState.enemy.attack;
    }
    
    // Move enemy's position in y direction.
    if (nextAppState.enemy.y < 0) {
        nextAppState.enemy.up = 1;
    } else if (nextAppState.enemy.y + 60 > 160){
        nextAppState.enemy.up = 0;
    }
    
    if (nextAppState.enemy.up == 1) {
        nextAppState.enemy.y += 1.5;
    } else {
        nextAppState.enemy.y -= 2;
    }
    
    // if enemy's hp is 0, win!
    // if hero's hp is 0, lose!
    // Game is over!!!
    if (nextAppState.enemy.hp == 0) {
        nextAppState.won = 1;
        nextAppState.gameOver = 1;
    }
    if (nextAppState.hero.hp == 0) {
        nextAppState.won = 0;
        nextAppState.gameOver = 1;
    }

    return nextAppState;
}
