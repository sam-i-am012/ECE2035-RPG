//=================================================================
// The speech class file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "speech.h"

#include "globals.h"
#include "hardware.h"

///////////////////////////////
//Static function declarations
///////////////////////////////
/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();



/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);


///////////////////////////////
//Drawing function declarations
///////////////////////////////


//***
// HINT: for this function and below: Check out the ULCD demo in the docs to see what
// uLCD methods you can use to draw graphics 

//Link: https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
//***


void draw_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    
    //1. Draw a speech bubble at the bottom of the screen 
    // Hint: You can stack ULCD rectangles to make a border
    uLCD.filled_rectangle(0, 85, 128, 128, 0x915F6D); //border is purple
    uLCD.filled_rectangle(5, 90, 123, 123, 0x00000000);

}

void erase_speech_bubble()
{
    //****************
    // TODO: Implement
    //****************
    
    //1. Erase the speech bubble at the bottom of the screen
    // Hint: You can overwrite the bubble with a empty (black) one
    uLCD.filled_rectangle(0, 85, 128, 128, 0x00000000);
    //draw_game(1);
}

void draw_speech_line(const char* line, int which)
{
    //****************
    // TODO: Implement
    //****************
    //1. Set the location which line of text will go the uLCD
    // Hint: Change the y coordinate based on which line (top or bottom)
    if (which == TOP) {
        uLCD.locate(1, 12);
    } else {
        uLCD.locate(1, 14);
    }

    //2. For each character in the text, write it to uLCD
    for (int i = 0; line[i] && i < 17; i++) //We can simplify by limiting each line to 17 char
    {
        //TODO: Write the character
        uLCD.printf("%c", line[i]);
        wait_ms(30); 
    }
}

void speech_bubble_wait()
{
    //****************
    // TODO: Implement
    //****************
    
    // 1. Keep waiting until the action button is pressed 
    //   Hint: What type of loop can we use to stay until a condition is met?
    GameInputs inputs = read_inputs();
    while (!inputs.b1) {
        inputs = read_inputs();
    }
}

void speech(const char* line1, const char* line2)
{
    //****************
    // TODO: Implement
    //****************
    // 1. Place the two lines into an array of lines
    // 2. Pass in to long_speech with the corresponding number of lines
    const char* lines[] = {line1, line2};
    long_speech(lines, 2);
}

void long_speech(const char* lines[], int n)
{

    //****************
    // TODO: Implement
    //****************

    //1. Create a speech bubble
    draw_speech_bubble();

    //2. For each lines, display only two lines at a time
    //   If two lines are displayed, make sure to wait (call the wait function)
    for (int i = 0; i < n; i += 2) { //index by 2 because we only display two lines at once
        draw_speech_line(lines[i], TOP); //print line on the top
        if (i + 1 < n) {
            draw_speech_line(lines[i + 1], BOTTOM); //print line to the bottom
        }
        speech_bubble_wait();
        draw_speech_bubble();
    }
    
    //3. Erase the speech bubble when you are done
    erase_speech_bubble();
}

