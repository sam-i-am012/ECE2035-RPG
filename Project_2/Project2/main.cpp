//=================================================================
// The main program file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// Projet includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <math.h>

// added for delay: 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))

// Helper function declarations
void playSound(char* wav);



/////////////////////////
// Struct of Player 
/////////////////////////

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;            // Current locations
    int px, py;         // Previous locations
    bool has_key;       // flag when obtained the key
    bool game_solved;   // flag when game is complete
    bool talked_to_npc; // flag when you've talked to npc
    
    //flags i added
    bool item_aquired;   // flag when player defeated an enemy
    bool has_token;     // flag when player has defeated bellatrix
    bool has_snitch;    // for enemy1
    bool has_felix;     // for enemy2
    int lives;          // lives for the player
    bool has_broom;     // for sidequest 
    bool talked_to_Ron;  // for sidequest
    bool has_teleport; // for obj other than enemies drop items 
    int teleportTimeLeft; //to see how many times left player has to teleport

    //You will need to add more flags as needed

} Player;



/////////////////////////
// Get Action
/////////////////////////

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define ATTACK_BUTTON 3
#define GO_LEFT 4
#define GO_RIGHT 5
#define GO_UP 6
#define GO_DOWN 7
#define NAV_CENTER 8

int get_action(GameInputs inputs)
{
    //******************
    // TODO: Implement
    //******************
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    if (inputs.b1) {   //action button
        return ACTION_BUTTON;
    }
    if (inputs.b2) {   //menu button
        return MENU_BUTTON;
    }
    if (inputs.b3) {   //attack button
        return ATTACK_BUTTON;
    }
    
    // 2. Check for your navigation switch inputs and return the corresponding action value
    if (inputs.ns_left) {
        return GO_LEFT;
    }
    if (inputs.ns_right) {
        return GO_RIGHT;
    }
    if (inputs.ns_up) {
        return GO_UP;
    }
    if (inputs.ns_down) {
        return GO_DOWN;
    }

    // added for middle nav button 
    if (inputs.ns_center) {
        return NAV_CENTER;
    }

    // If no button is pressed, just return no action value
    return NO_ACTION;


}




/////////////////////////
// Helper Functions:
/////////////////////////
// Feel free to define any helper functions here for update game





/////////////////////////
// Update Game
/////////////////////////

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    MapItem* item = NULL;

    
    
    //******************
    // TODO: Implement
    //******************
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        // NAVIGATION 
        case GO_UP:
            //TODO: Implement
            //1. Check the item north of the player
            item = get_north(Player.x, Player.y);
            //2. Make sure to not walk through walls
            if (item->walkable) {
                //3. If it is not a wall, the walk up by updating player's coordinates
                Player.y -= 1;
            }
            break;
            
        case GO_LEFT:
            //TODO: Implement
            item = get_west(Player.x, Player.y);
            if (item->walkable) {
                Player.x -= 1;
            }
            break;
            
        case GO_DOWN:
            //TODO: Implement
            item = get_south(Player.x, Player.y);
            if (item->walkable) {
                Player.y += 1;
            }
            break;
            
        case GO_RIGHT:
            //TODO: Implement
            item = get_east(Player.x, Player.y);
            if (item->walkable) {
                Player.x += 1;
            }
            break;
            
        // BUTTONS
        case ACTION_BUTTON:

            //******************
            // TODO: Implement
            //******************

            // 1. Check if near NPC, 
            //     - if so, mark the player has talked and give instructions on what to do
            //     - if the game is solved (defeated Buzz), give the player the key
            //     - return FULL_DRAW to redraw the scene
            item = get_east(Player.px, Player.py);
            if (item->type == NPC) {         
                draw_speech(60,48); // coordinates are for 11 x 11 map, not my main map       
                if (Player.has_key == true && Player.has_token) { //this happens after main character (harry) deafeats bosses
                    Player.game_solved = true;
                    const char* speech[] = {"The boy who", " lived! You", "actually did it.", "You defeated He-", 
                        "Who-Must-Not-Be-", "Named! You have", "aquired the key", "and token", "required", "to get through", 
                        "the portal. Go", "find the door", "and unlock it."};
                    long_speech(speech, 13);
                } else if (Player.item_aquired == true) { //happens after player defeats an enemy
                    Player.talked_to_npc = true;
                    
                    // remove enemy items from inventory
                    Player.has_snitch = false;
                    Player.has_felix = false;

                    const char* speech[] = {"Thanks for", "bringing these", "items back", "to me. Now...", "Here is the",
                    "spell you need.", "For Voldemort,", "It is an", "unforgivable", "curse so be", "careful. The", "spell is...", 
                    "Avada Kedavra!", "(hint: it is a", "little greenish", "in color).", "To defeat", "Bellatrix, the",
                    "spell will be", "the color blue", "and red make", "together...","Now find the", "portal and", "go defeat", 
                    "Voldemort and", "Bellatrix!"};
                    long_speech(speech, 27);
                } else { //first dialogue that happens                                    
                    const char* speech[] = {"Harry Potter!", "I knew you would", "come help. You", "need to save the", 
                        "wizarding world", "from He-Who-Must", "-Not-Be Named.", "He went through", "a portal. You", 
                        "need to fight", "him AND his", "loyal follower, ", "Bellatrix.", "But first!", "You need to", 
                        "defeat at least", "one enemy and", "bring me back", "the item that it", "dropped. So I", 
                        "can teach you", "the spells that", "you need to", "use to defeat", "them both."};
                    long_speech(speech, 25);
                }
                return FULL_DRAW;
            }
            
            // 2. Check if near a door
            //    - if the player has the key, you win the game
            //    - if not, show speech bubbles that the play needs to get the key 
            //     -return FULL_DRAW to redraw the scene
            item = get_north(Player.px, Player.py);            
            if (item->type == DOOR) {
                if (!Player.has_key) {
                    const char* speech[] = {"Sorry, you can't", " return to", "Hogwarts", "without",
                        "obtaining the", "key. Go talk", "to Hedwig", "first!"};
                    long_speech(speech, 8);
                } else {
                    item->walkable = 1; //let the player be able to walk through the gate
                    const char* speech[] = {"You got the", "key and token!", "Congrats!!", "now you can", "return to", "Hogwarts! Yay!"};
                    long_speech(speech, 6);
                }
                return FULL_DRAW;
            }

            // 3. Check if on portal to go to small map
            //    - if the player has talked to the npc, then start the speech bubble to fight buzz
            //    - set the players coordinates for the small map
            //    - and set the map to the small map
            //     -return FULL_DRAW to redraw the scene
            item = get_here(Player.px, Player.py);
            if (item->type == PORTAL) {
            //Player.talked_to_npc = true; //DONT FORGET TO DELETE, THIS IS ONLY FOR TESTING PURPOSES
                if (Player.talked_to_npc) {
                    const char* speech[] = {"You are about", "to go into the", "dark forest and", "face Voldemort's",
                        "wrath. Be", "prepared and", "good luck Harry!"};
                    long_speech(speech, 7);
                    set_active_map(1);
                    pc.printf("going to mini map\n");
                    Player.x = Player.y = 5;
                    return FULL_DRAW;
                } else {
                    const char* speech[] = {"You cannot", "enter the portal", "until you talk", "to Hedwig!"};
                    long_speech(speech, 4);
                    return FULL_DRAW;
                }
            }
            
            // 4. Check if on a portal (to teleport back to main map)
            //    - if so, move the player the correct coordinates
            //    - and set the map back to the main big map
            //     -return FULL_DRAW to redraw the scene
            item = get_here(Player.px, Player.py);
            pc.printf("button clicked? %d ", action);
            if (item->type == PORTAL2) {
                set_active_map(0);
                Player.x = Player.y = 5;
                return FULL_DRAW;
            }

            //check if on portal to end game
           item = get_here(Player.px, Player.py);
           if (item->type == END_PORTAL) {
                return GAME_OVER;
           }
        
            // Feel free to add more functions as you make the game!
            // for npc ron sidequest 
            item = get_east(Player.px, Player.py);
            if (item->type == RON) {         
                if (!Player.talked_to_Ron) {
                    draw_speech(60,48); // coordinates are for 11 x 11 map, not my main map       
                    Player.talked_to_Ron = true;
                    const char* speech[] = {"Hiya Harry!", "I haven't seen", "you in a while.", "I need some", "help! I lost",
                        "my broomstick", "and I don't", "know where. If", "you find it and", "bring it back", 
                        "to me, I can", "  give you", "something to", "help you later!"};
                    long_speech(speech, 14);
                    return FULL_DRAW;
                } else if (Player.has_broom) {
                    draw_speech(60,48); // coordinates are for 11 x 11 map, not my main map  
                    Player.has_broom = false; // to remove from inventory     
                    Player.lives++;
                    const char* speech[] = {"Oh wow you did", "it Harry! Thank", "you so so much!", "As promised,",
                    "here is your", "reward. It's an", "extra life! Good", " luck out there."};
                    long_speech(speech, 8);
                    return FULL_DRAW;
                }
            }

            //check if on the broomstick
            item = get_here(Player.px, Player.py);
            if(item->type == BROOM) {
                Player.has_broom = true;
                const char* speech[] = {"Looks like you", "have found Ron's", "broomstick!", "Bring it back", "to him for",
                " something", "  special!", "", "BROOMSTICK item", "has been added", "to your", "inventory."};
                long_speech(speech, 12);
                return FULL_DRAW;
            }
                
            break;
        
        case ATTACK_BUTTON:

            //******************
            // TODO: Implement
            //******************

            // 1. Check if near enemy, 
            //     - if so, mark enemy as defeated
            //      use speech bubbled to show what item was dropped
            //      update the player struct as needed
            //      if enemy is Pete, make sure that the right attack is being used
            //      If pete is defeated, update game as nescessary
            
            //following 2 if statements are for regular enemies
            item = get_east(Player.px, Player.py);
            if (item->type == ENEMY1) {
                draw_animation(70,45); // coordinates are for 11 x 11 map, not my main map       

                const char* speech[] = {"STUPEFY!", "The stupefy", "spell has", "been used.", 
                    " The golden", "snitch has been", "dropped."};
                long_speech(speech, 7);
                add_slain_enemy1(16, 21);
                Player.item_aquired = true; //set flag to true
                Player.has_snitch = true;
                return FULL_DRAW;
            }
            
            if (item->type == ENEMY2) {
                draw_animation(70,45); // coordinates are for 11 x 11 map, not my main map
                const char* speech[] = {"PETRIFICUS", "   TOTALUS!", "The spell has", "been used.", "The Felix", 
                    "Felicis has", "been dropped."};
                long_speech(speech, 7);
                add_slain_enemy2(24, 3);
                Player.item_aquired = true; //set flag to true
                Player.has_felix = true;
                return FULL_DRAW;
            }

            

            //following if statements are for boss1
            item = get_here(Player.px, Player.py);
            if (item->type == ATTACK1) { //code for the blue bolt (wrong attack)
                draw_animation(69,58); // coordinates are for 11 x 11 map, not my main map
                const char* speech[] = {"You silly", "wizard! That", "attack is", "too simple to", 
                    "defeat me!", "MWAHAHA"};
                long_speech(speech, 6);
                return FULL_DRAW;
            }
            
            if (item->type == ATTACK2) { //code for the green bold (correct attack)
                draw_animation(47,58); // coordinates are for 11 x 11 map, not my main map
                const char* speech[] = {"Voldemort:", "  AHHHHH!", "You killed me!", "" , "KEY item has", 
                "been dropped!"};
                long_speech(speech, 6);

                add_slain_boss(14, 16);
                Player.has_key = true;
                return FULL_DRAW;
            }      

            //following if statements are for boss2
            item = get_here(Player.px, Player.py);
            if (item->type == ATTACK3) { //code for the pink bolt (wrong attack)
                draw_animation(48,36); // coordinates are for 11 x 11 map, not my main map
                const char* speech[] = {"Don't be", "ridiculous.", "YELLOW spell", "won't work for", 
                    "someone as", "powerful as me."};
                long_speech(speech, 6);
                return FULL_DRAW;
            }
            

            if (item->type == ATTACK4) { //code for the purple bolt (correct attack)
                draw_animation(70,36); // coordinates are for 11 x 11 map, not my main map
                const char* speech[] = {"Bellatrix:", "  NOOOOO!", "You defeated me", "with the PURPLE", "spell.", "", 
                    "TOKEN item", "dropped!"};
                long_speech(speech, 8);
                add_slain_boss2(13, 3);
                Player.has_token = true;
                return FULL_DRAW;
            }      

            // for "Objs other than enemies drop items" advanced feature
            item = get_east(Player.px, Player.py);
            if (item->type == DRACO) {      
                draw_animation(70,47); // coordinates are for 11 x 11 map, not my main map   
                Player.has_teleport = true;
                const char* speech[] = {"Ow Potter! Why", "did you attack", "me?? My father", "will hear about",
                "this...", "","TELEPORT POTION", "has been dropped."};
                long_speech(speech, 8);

                add_teleportPotion(6,34); // draco npc turns into red potion
                const char* speech2[] = {"This potion will", "let you teleport", "to a random", "part of the map.",
                "You can use it", "3 times before", "the potion runs", "  out.", "Press the middle", "nav button to", 
                "activate.", "", "It's been added", "to inventory."};
                long_speech(speech2, 14);
                return FULL_DRAW;
            }
        
            

            break;

        //***********
        // Add more cases as needed
        //***********

        case NAV_CENTER :
            if (Player.has_teleport && Player.teleportTimeLeft > 0) { // if player has the teleport potion
                draw_borderTeleport();
                switch (Player.teleportTimeLeft) {                 
                    case 3 :             
                        Player.x = 24;
                        Player.y = 30;
                        break;
                    case 2 : 
                        Player.x = 43;
                        Player.y = 9;
                        break;
                    case 1 : 
                        Player.x = 37;
                        Player.y = 28;                        
                        Player.has_teleport = false;
                        draw_border(); // change border back to original
                        break;
                }
                Player.teleportTimeLeft--; // decrease amount of time
            }

            break;
    }

    
    // check if on a lava tile to decrease life 
    item = get_here(Player.px, Player.py);
    if (item->type == LAVA) {
        Player.lives--;
        draw_borderLava();
        return FULL_DRAW;
        
    }
    
    return NO_RESULT;
}



/////////////////////////
// Draw Game
/////////////////////////

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
           // if (init && i == 0 && j == 0) // Only draw the player on init
            if ( i == 0 && j == 0) // always draw the player
            {
                // ADVANCED FEATURE for object that is bigger than character (and blocks him from view)
                if (get_active_map_index() == 0 && // this first condition is so player doesn't become a rock in mini map
                    (Player.x == 11 && Player.y == 11
                    || Player.x == 22 && Player.y == 18 
                    || Player.x == 27 && Player.y == 8
                    || Player.x == 17 && Player.y == 5 )) {
                    // so player can walk behind it
                    draw = draw_rock;
                } else {
                    draw_player(u, v, Player.has_key);
                    continue;
                }
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
                else if (curr_item && curr_item->type == CLEAR)
                {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status(Player.x, Player.y, Player.lives);
    
    // lower status (diff because there are diff items )
    if (!Player.talked_to_npc) { //will happen when defeating enemies
        draw_lower_status(Player.has_snitch, Player.has_felix, Player.has_teleport, 0);
    } else { // will happen when defeating bosses 
        draw_lower_status(Player.has_key, Player.has_token, Player.has_teleport, 1);
    }
    
    // for sidequest items 
    if (Player.talked_to_Ron) { // will happen during sidequest
        draw_lower_status(Player.has_broom, 0,Player.has_teleport, 2);
    }

    //if (Player.has_teleport) { // will happen for power up 
        //draw_lower_status(0, 0, Player.has_teleport, 0);
    
    
    
}







/////////////////////////
// Map Intialization
/////////////////////////

// Important locations for all maps
int cb_loc[2] = {5,20}; //Location of the center of the cave


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    //Initialize and sets current map to the first map
    Map* map = set_active_map(0);

    /////////////////////////
    //Initial Environmnet
    /////////////////////////

    //Adding random plants
    pc.printf("Adding Plants!\r\n");
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width() + 2, i / map_width());
    }

    // adding rocks that player can walk behind
    add_rock(11,11);
    add_rock(22,18);
    add_rock(27,8);
    add_rock(17,5);

    //Adding wall borders 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    
    //Adding extra chamber borders 
    pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    add_door(33, 10, HORIZONTAL, 4);

    //adding portal 
    add_portal(cb_loc[0],cb_loc[1],1,1,5,5); 

    // adding lava tiles
    // near enemy 1 (goblin)
    add_lava(14,20);
    add_lava(13,21);
    add_lava(14,21);

    add_lava(16,23);
    add_lava(17,23);
    add_lava(17,24);
    add_lava(17,25);
    add_lava(18,25);
    
    // near enemy 2 (troll)
    add_lava(21,2);

    add_lava(21,5);
    add_lava(22,4);
    add_lava(22,5);

    add_lava(26,2);
    add_lava(26,3);
    add_lava(27,2);

    // near ron's broomstick
    add_lava(37,40);
    add_lava(37,39);
    add_lava(36,40);

    add_lava(40,41);
    add_lava(39,41);
    add_lava(39,38);

    //adding broomstick 
    add_broom(38,40);

    // adding draco npc for "Objects other than enemies also drop items when attacked"
    add_draco(6,34);

    
   //i don't think i need this tbh 

    // //Adding extra cave to Buzz's evil lair
    // pc.printf("Add cave\r\n");
    // add_cave(cb_loc[0],cb_loc[1],1,1,5,5);      //Cave is set as a 4x4 block to be bigger
    // add_cave(cb_loc[0]+1,cb_loc[1],2,1,5,5);
    // add_cave(cb_loc[0],cb_loc[1]+1,3,1,5,5);
    // add_cave(cb_loc[0]+1,cb_loc[1]+1,4,1,5,5);

    pc.printf("Initial environment completed\r\n");

    /////////////////////////////////
    // Characters and Items for the map
    /////////////////////////////////

    // Add NPC
    add_npc(10, 5);  //NPC is initialized to (x,y) = 10, 5. Feel free to move him around
    
    //***********************************
    // TODO: Implement As Needed
    //***********************************

    //Add any extra characters/items here for your project

    //adding enemy1
    add_enemy1(16, 21);
    //adding enemy2
    add_enemy2(24,3);

    //add end portal that ends the game
    add_endPortal(34, 5);

    // adds Ron NPC for side quest
    add_ron(32, 20);

    //Prints out map
    print_map();
}

void init_small_map()
{
    //Sets current map to the second map
    set_active_map(1);

    //Adding  spooky trees
    pc.printf("Adding Plants!\r\n");
    add_spookyTree(3, 4);
    add_spookyTree(1, 9);
    add_spookyTree(14, 7);
    add_spookyTree(8, 10);
    add_spookyTree(10, 16);
    add_spookyTree(3, 15);

    //Adding dungeon walls for boss2
    add_bossWall(9, 1, VERTICAL, 6);
    add_bossWall(11, 9, HORIZONTAL, 6);

    
    

    //***********************************
    // TODO: Implement 
    //***********************************

    // 1. Add walls to the smaller map.
    //    Set the dimensions to be 16x16  <-- you may change to your liking, but must be smaller than the main map
    //    Hint: See how walls are initialized in the main map
   
    //Adding wall borders 
    unsigned int dim = 18;
    pc.printf("Adding walls!\r\n");
    add_wall2(0, 0, HORIZONTAL, dim);
    add_wall2(0, dim - 1, HORIZONTAL, dim);

    //extra walls because idk why it's cutting off and this seemed to fix it so ? 
    add_wall2(0, dim, HORIZONTAL, dim + 4);
    add_wall2(0, dim + 1, HORIZONTAL, dim + 4);
    add_wall2(0, dim + 2, HORIZONTAL, dim + 4);

    add_wall2(0, 0, VERTICAL, dim);
    add_wall2(dim - 1,  0, VERTICAL,   dim);
    
    add_wall2(dim,  0, VERTICAL,   dim);
    add_wall2(dim + 1,  0, VERTICAL,   dim);
    add_wall2(dim + 2,  0, VERTICAL,   dim);
    add_wall2(dim + 3,  0, VERTICAL,   dim);

    // 2. Add a way to access your special attacks either here or in update_game() or anywhere you feel would be the best
    
    // attacks for boss 1
    add_blueLighting(13,15); // left 
    add_greenBolt(15, 15); // right

    // attacks for boss 3
    add_purpleBolt(12,4); // left
    add_pinkBolt(14,4); // right

    // 3. Add Bosses in the map
    add_boss(14, 16);
    add_boss2(13,3);



    // You may add any extra characters/items here for your project


    // Add stairs back to main (map 0)
    //add_stairs(4, 6, 0, cb_loc[0], cb_loc[1]);
    
    //add portal that teleports back to main map
    add_portal2(4,6, 0, cb_loc[0], cb_loc[1]);

    //prints out map
    //print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // creating start screen 

    uLCD.color(0xe7aded);
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.locate(0,0);
    uLCD.printf("  Harry\n");
    uLCD.printf(" Potter's");
    uLCD.printf(" Journey!\n");

    uLCD.text_width((double)1.5);
    uLCD.text_height((double)1.5);
    uLCD.color(0xFFFFFF);
    uLCD.locate(0,8);
    uLCD.printf("Controls:\n");
    
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.printf(" B1: Interactions\n");
    uLCD.printf(" B2: Attack\n");
    uLCD.printf(" Use nav switch\n");
    uLCD.printf("  for movement\n");

    uLCD.color(0x00ff43);
    uLCD.printf("\nPress B1 to start! \n");
    
   
    
    GameInputs inputs;
    int action;
    while (action != ACTION_BUTTON) {
        inputs = read_inputs();
        action = get_action(inputs);
    }
    uLCD.cls(); // closes start screen

    //uLCD.filled_rectangle(64, 64, 74, 74, RED); //DELETE OR COMMENT THIS LINE  <-- It is a temporary indicator that your LCD is working before you implement your code

    // Initialize the maps
    maps_init();
    init_main_map();
    init_small_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = false;
    Player.has_token = false;
    Player.game_solved = false;
    Player.talked_to_npc = false;
    Player.item_aquired = false;
    Player.has_token = false;
    Player.lives = 5;
    Player.has_broom = false;
    Player.talked_to_Ron = false;
    Player.has_teleport = false;
    Player.teleportTimeLeft = 3;


    // Initial drawing
    draw_game(true);


    // FOR TESTING ///////
    //Player.item_aquired = true; // enemy already attacked DELETE LATER

    //Player.talked_to_npc = true; // opens portal DELETE LATER
    //playerLose = true;
    //Player.has_teleport = true;



    //////////////////////
    


    // Main game loop
    while(1)
    {

        ////////////////////////////////
        // TODO: Implement 
        ////////////////////////////////

        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actually do the game update:
        // 1. Read inputs    
        GameInputs inputs = read_inputs();
            

        // 2. Determine action (get_action) 
        int action = get_action(inputs);   


        // 3. Update game (update_game)
        int result;  // Set this variable "result" for the resulting state after update game
        result = update_game(action);

        // 3b. Check for game over based on update game result
        if (result == GAME_OVER) {     
            uLCD.cls();
            //create background for the text
            uLCD.filled_rectangle(0,0, 130, 130, 0x000000);

            //settings for text
            uLCD.text_width(3);
            uLCD.text_height(3);
            uLCD.color(0x00FF00); // green color

            //print
            uLCD.printf("\n YAYY!\n");
            uLCD.printf(" YOU\n");
            uLCD.printf(" WON\n");
            break;
        }

        // 4. Draw screen to uLCD
        bool full_draw = false;
        if (result == FULL_DRAW) full_draw = true;
        draw_game(full_draw);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);

        // check if player lost (no lives left). will show lose screen 
        if (Player.lives <= 0) {
            uLCD.cls();
            uLCD.color(0xFFFFFF); //color

            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("\n   You\n");
            uLCD.printf(" lost :(\n");
            
            
            uLCD.printf("\n  GAME\n");
            
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.color(0xFF0000); //color
            uLCD.printf("  OVER");
            break;
                        
        }      
    }
}

/////////////////////////////
//Advanced Features
/////////////////////////////

// Plays a wavfile
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
    return;
}
