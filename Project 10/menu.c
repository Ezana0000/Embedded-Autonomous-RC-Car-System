//------------------------------------------------------------------------------
//
//  Description: Menu logic and arrays. Uses the thumbwheel to scroll through stuff.
//
//  Ezana
//  Mar 2026
//  Built with Code Composer Studio
//------------------------------------------------------------------------------

#include "msp430.h"
#include <string.h>
#include "LCD.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern volatile unsigned int ADC_Thumb;

// Arrays for the resistor menu
const char* Resistor_Colors[10] = {"  Black   ", "  Brown   ", "   Red    ", "  Orange  ", "  Yellow  ", "  Green   ", "   Blue   ", "  Violet  ", "   Gray   ", "  White   "};
const char* Resistor_Values[10] = {"    0     ", "    1     ", "    2     ", "    3     ", "    4     ", "    5     ", "    6     ", "    7     ", "    8     ", "    9     "};

// Array for the shapes menu
const char* Shapes[10] = {"  CIRCLE  ", "  SQUARE  ", " TRIANGLE ", " OCTAGON  ", " PENTAGON ", " HEXAGON  ", "   CUBE   ", "   OVAL   ", "  SPHERE  ", " CYLINDER "};

// Song broken up into 10 character chunks so it fits the screen
const char* Song_Lyrics[25] = {
    "We're the ", "Red and   ", "White from", "State And ", "we know we",
    "are the   ", "best. A   ", "hand be-  ", "hind our  ", "back, We  ",
    "can take  ", "on all the", "rest. Come", "over the  ", "hill, Car-",
    "olina.    ", "Devils and", "Deacs     ", "stand in  ", "line. The ",
    "Red and   ", "White from", "N.C. State", ". Go State", "!         "
};

// Variables to keep track of where I am in the song
int current_song_index = 0;
unsigned int last_adc_value = 0;
int song_toggle = 0;

void Run_Main_Menu(void){
    // Divide the thumbwheel value so the 3 menu items get even space on the wheel
    int selection = ADC_Thumb / MAIN_MENU_DIVIDER;

    strcpy(display_line[0], " MAIN MENU");
    strcpy(display_line[1], "----------");

    // Put a little arrow next to whatever I'm currently hovering over
    if (selection == 0) {
        strcpy(display_line[2], ">Resistors");
        strcpy(display_line[3], " Shapes   ");
    } else if (selection == 1) {
        strcpy(display_line[2], ">Shapes   ");
        strcpy(display_line[3], " Song     ");
    } else {
        strcpy(display_line[2], ">Song     ");
        strcpy(display_line[3], "          ");
    }
    display_changed = 1;
}

void Run_Resistor_Menu(void){
    // Figure out which resistor to show based on the wheel
    int selection = ADC_Thumb / SUB_MENU_DIVIDER;
    if (selection > MAX_SUB_MENU_INDEX) selection = MAX_SUB_MENU_INDEX;

    strcpy(display_line[0], "  Color:  ");
    strcpy(display_line[1], (char*)Resistor_Colors[selection]);
    strcpy(display_line[2], "  Value:  ");
    strcpy(display_line[3], (char*)Resistor_Values[selection]);
    display_changed = 1;
}

void Run_Shape_Menu(void){
    int selection = ADC_Thumb / SUB_MENU_DIVIDER;
    if (selection > MAX_SUB_MENU_INDEX) selection = MAX_SUB_MENU_INDEX;

    // Show the previous shape on the top line, or blank if at the start
    if (selection == 0) strcpy(display_line[0], "          ");
    else strcpy(display_line[0], (char*)Shapes[selection - 1]);

    // Put the current shape on the big middle line
    strcpy(display_line[1], (char*)Shapes[selection]);

    // Show the next shape on the bottom line, or blank if at the end
    if (selection == MAX_SUB_MENU_INDEX) strcpy(display_line[2], "          ");
    else strcpy(display_line[2], (char*)Shapes[selection + 1]);

    display_changed = 1;
}

void Run_Song_Menu(void){
    // Ratchet logic: only go to the next lyric if I spin it forward enough
    if (ADC_Thumb > last_adc_value + SONG_SCROLL_THRESHOLD) {
        current_song_index++;
        if (current_song_index > MAX_SONG_INDEX) current_song_index = MAX_SONG_INDEX;

        last_adc_value = ADC_Thumb;
        song_toggle = !song_toggle; // Flip the borders
    }
    else if (ADC_Thumb < last_adc_value - SONG_SCROLL_THRESHOLD) {
        // If I spin backwards, just update the baseline but don't change the lyric
        last_adc_value = ADC_Thumb;
    }

    // Alternate the borders between lines 1 and 3
    if (song_toggle) {
        strcpy(display_line[0], "Red/White ");
        strcpy(display_line[2], "White/Red ");
    } else {
        strcpy(display_line[0], "White/Red ");
        strcpy(display_line[2], "Red/White ");
    }

    strcpy(display_line[1], (char*)Song_Lyrics[current_song_index]);
    display_changed = 1;
}
