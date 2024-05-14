//=================================================================
// The map class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "map.h"
#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items; // Hashtables for all items of the map
    int w, h;         // Map dimensions
    int index;        // Index of map (tells if its the first map or second map)
};

///////////////////////
//Important Definitions
///////////////////////

#define MHF_NBUCKETS 97     //  Hashing value
#define NUM_MAPS 2          //  You may add more maps here
static Map maps[NUM_MAPS];  //  Array of maps <-- 2 maps in the array
static int active_map;      //  Current active map on screen


//To erase a MapItem, we can simply replace it 
//with a clear type item called clear sentinel
static const MapItem CLEAR_SENTINEL = { 
    .type = CLEAR,
    .draw = draw_nothing
};


/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int x, int y) {

    //****************
    // TODO: Implement
    //****************

    // 1. Return the 1-D XY key
    // Hint: Think about how you accessed location previous projects/hw -----> finding george !
        // row + (scale * width)
    return x + (y * maps[active_map].w);

}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    //****************
    // TODO: Implement
    //****************

    // Hint: what math operator can be used as an easy way to hash a key? <-- %  ?
    // 1. Use MHF_NBUCKETS defined at the top as the value to hash for your hash function
    // 2. Return the hashed key

    return key % MHF_NBUCKETS;

}

/**
 * Initializes the map, using a hash_table, setting the width and height.
 */
void maps_init()
{
    //****************
    // TODO: Implement
    //****************

    // 1. Loop through all possible maps, where for each map's items, create a hashtable 
    for (int i = 0; i < NUM_MAPS; i++) {
        maps[i].items = createHashTable(map_hash, MHF_NBUCKETS);

        // 2. Set width & height for any maps 
        maps[i].w = 53;
        maps[i].h = 53;

        maps[i].index = i;  //set the index
    }
    // 3. Set the first map to be active
    active_map = 0;
}


Map* get_active_map()
{
    //****************
    // TODO: Implement
    //****************

    // 1. Return a pointer to the current map based on which map is active (active_map)
    //    Hint: What operator do we use for returning addresses?
    //    Note: active_map is set as a Global Variable 
    return &maps[active_map];

}

int get_active_map_index()
{
    //****************
    // TODO: Implement
    //****************
    
    // 1. Return the index to the current map based on which map is active (active_map)
    return active_map;
}

Map* set_active_map(int m)
{
    //****************
    // TODO: Implement
    //****************

    Map* currentMap = get_active_map(); //need to store current map before val is changed
    // 1. Set the Global Variable for active map to the map index passed in
    active_map = m;  
    // 2. Return a pointer to the current map based on which map is active (active_map)
    return currentMap;

}

/**
 *  Prints out the map for debugging on the terminal
 */

void print_map()
{
    // NOTE: As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S',
        'F', 'E', 'P', 'R', 'M', 'O'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}


/**
 * Returns width of active map
 */
int map_width()
{
    //****************
    // TODO: Implement 
    //****************
    // Hint 1 for this and below functions: use another function to GET the current active map
    // Hint 2: check the struct of a map item

    Map* currMap = get_active_map();
    return maps->w;
}

/**
 * Returns height of active map
 */
int map_height()
{
    //****************
    // TODO: Implement  
    //****************

    Map* currMap = get_active_map();
    return maps->h;
}

/**
 * Returns the area of the active map
 */
int map_area()
{
    //****************
    // TODO: Implement  
    //****************
    // HINT: You have the width and height to compute area
    return map_width() * map_height();
}


/**
 * Returns the MapItem at current coordinate location
 */
 MapItem* get_here(int x, int y)
 {
    //****************
    // TODO: Implement
    //****************
    
    // 1. Get map item 
    unsigned int key = XY_KEY(x,y);
    Map* map = get_active_map();
    MapItem* item = (MapItem*)getItem(map->items, key);

    // 2. If the item exist and is a clear type, remove item
    if (item && item->type == CLEAR) {
        removeItem(map->items, key);
    }
    // 3. Return the item 
    return item;
 }

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Get map item 
    // 2. If the item exist and is a clear type, remove item
    // 3. Return the item 
    return get_here(x, y - 1);

}

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Get map item 
    // 2. If the item exist and is a clear type, remove item
    // 3. Return the item
    
    return get_here(x, y + 1);
}

/**
 * Returns the MapItem immediately right the given location.
 */
MapItem* get_east(int x, int y)
{
   //****************
    // TODO: Implement
    //****************
    // 1. Get map item 
    // 2. If the item exist and is a clear type, remove item
    // 3. Return the item 

    return get_here(x + 1, y);
}

/**
 * Returns the MapItem immediately left the given location.
 */
MapItem* get_west(int x, int y)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Get map item 
    // 2. If the item exist and is a clear type, remove item
    // 3. Return the item 

    return get_here(x - 1, y);
}

/**
 * Erases item on a location by replacing it with a clear sentinel
 */
void map_erase(int x, int y)
{
    MapItem* item = (MapItem*)insertItem(get_active_map()->items, XY_KEY(x, y), (void*)&CLEAR_SENTINEL);
    if(item) free(item);
}


////////////////////////////////////
// Adding specific items to the map
////////////////////////////////////


void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_npc(int x, int y)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Implement the same way as how we add plant
    //     *Make the NPC NOT walkable

    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_npc;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it

}


// void add_water(int x, int y)
// {

//    //****************
//     // TODO: Implement
//     //****************
//     // 1. Implement the same way as how we add plant
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = WATER;
//     w1->draw = draw_water;
//     w1->walkable = true;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it

// }

void add_fire(int x, int y)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Implement the same way as how we add plant
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = FIRE;
    w1->draw = draw_fire;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it

}

void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_snitch(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNITCH;
    w1->draw = draw_snitch;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_felix(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = FELIX;
    w1->draw = draw_felix;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_token(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TOKEN;
    w1->draw = draw_token;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it

}

// void add_earth(int x, int y)
// {
//     //****************
//     // TODO: Implement
//     //****************
//     // 1. Implement the same way as how we add plant
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = EARTH;
//     w1->draw = draw_earth;
//     w1->walkable = true;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it

// }


void add_slain_boss(int x, int y)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Implement the same way as how we add plant
    // Note: This function is to ovewrite enemy when it is defeated
    //      You can do the following:
    //      -   Overwrite enemy with any other item (say plant, mud)
    //      -   Make a new drawing for a defeated enemy
    //      -   Erase enemy instead 
    add_key(x,y); //boss gets replaced by a key item
}



//add functions i created: 

void add_slain_boss2(int x, int y)
{
    add_token(x,y); //boss gets replaced by a token item
}
void add_boss(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BOSS;
    w1->draw = draw_boss;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  

}

void add_rock(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ROCK;
    w1->draw = draw_rock;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_speech(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ANIMATION;
    w1->draw = draw_speech;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_animation(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ANIMATION;
    w1->draw = draw_animation;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

void add_portal(int x, int y, int n, int tm, int tx, int ty) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PORTAL;
    w1->draw = draw_portal;
    w1->walkable = true;
    StairsData* data = (StairsData*) malloc(sizeof(StairsData));
    data->tm = tm;
    data->tx = tx;
    data->ty = ty;
    w1->data = data;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it   
}

void add_portal2(int x, int y, int tm, int tx, int ty) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PORTAL2;
    w1->draw = draw_portal2;
    w1->walkable = true;
    Portal2Data* data = (Portal2Data*) malloc(sizeof(Portal2Data));
    data->tm = tm;
    data->tx = tx;
    data->ty = ty;
    w1->data = data;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_spookyTree(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_spookyTree;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_enemy1(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ENEMY1;
    w1->draw = draw_enemy1;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_blueLighting(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ATTACK1;
    w1->draw = draw_blueLighting;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_greenBolt(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ATTACK2;
    w1->draw = draw_greenBolt;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_playerWin(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLAYER_WIN;
    w1->draw = draw_playerWin;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_slain_enemy1(int x, int y) {
    add_snitch(x,y); //enemy gets replaced by a fire item
}

void add_slain_enemy2(int x, int y) {
    add_felix(x,y); //enemy gets replaced by a fire item
}

void add_enemy2(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ENEMY2;
    w1->draw = draw_enemy2;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_endPortal(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = END_PORTAL;
    w1->draw = draw_endPortal;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_boss2(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BOSS;
    w1->draw = draw_boss2;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_pinkBolt(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ATTACK3;
    w1->draw = draw_pinkBolt;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_purpleBolt(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ATTACK4;
    w1->draw = draw_purpleBolt;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_lava(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LAVA;
    w1->draw = draw_lava;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_broom(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BROOM;
    w1->draw = draw_broom;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_empty(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = EMPTY;
    w1->draw = draw_empty;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_draco(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DRACO;
    w1->draw = draw_draco;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_teleportPotion(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TELEPORT;
    w1->draw = draw_teleportPotion;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_heart(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = HEART;
    w1->draw = draw_heart;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}

void add_ron(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = RON;
    w1->draw = draw_ron;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it  
}








////////////////////////////////////
// Adding environment to the map
////////////////////////////////////


void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_wall2(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall2;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_bossWall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_bossWall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_door(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = DOOR;
        w1->draw = draw_door;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}


void add_stairs(int x, int y, int tm, int tx, int ty)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = STAIRS;
    w1->draw = draw_stairs;
    w1->walkable = true;
    StairsData* data = (StairsData*) malloc(sizeof(StairsData));
    data->tm = tm;
    data->tx = tx;
    data->ty = ty;
    w1->data = data;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it    
}


void add_cave(int x, int y, int n, int tm, int tx, int ty)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CAVE;
    if (n==1){
        w1->draw = draw_cave1;
    }
    if (n==2){
        w1->draw = draw_cave2;
    }
    if (n==3){
        w1->draw = draw_cave3;
    }
    if (n==4){
        w1->draw = draw_cave4;
    }
    w1->walkable = true;
    StairsData* data = (StairsData*) malloc(sizeof(StairsData));
    data->tm = tm;
    data->tx = tx;
    data->ty = ty;
    w1->data = data;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it    
}


void add_mud(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = MUD;
        w1->draw = draw_mud;
        w1->walkable = true;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

