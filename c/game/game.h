#pragma once
#include "asset_builder.h"

int game_foo();

// TODO: implement these and hook into these from world
//void game_setup();
//void game_update();
//void game_render();
//void game_teardown();


// object pool
// generic game object with void* state
// types for common functions
// - update(State)
// - setup
// - teardown
// object to hold the entire game's state

// flow
// - iterate through events and update global state
// - object loop + self.state modifications + emit actions + fill draw queue
// - draw

