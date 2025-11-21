#include "led_map.h"

// Matrix to LED mapping based on physical layout
// This mapping was built using the debug mode by pressing each key
// and recording which LED index corresponds to each matrix position
const uint8_t PROGMEM matrix_to_led[14][7] = {
    // LEFT HALF (rows 0-6, LEDs 0-32)
    //   col0  col1  col2  col3  col4  col5  col6
    {   255,   21,   22,   23,   24,   25,   255 },  // row 0
    {    26,   255,  20,   19,   18,   17,   255 },  // row 1
    {    16,   15,   255,   9,   10,   11,   255 },  // row 2
    {    12,   13,   14,   255,    8,    7,   255 },  // row 3
    {     6,    4,   255,   255,  255,    5,   255 },  // row 4
    {   255,  255,  255,   255,  255,   255,  255 },  // row 5 (no keys mapped)
    {   255,  255,  255,   255,  255,   255,  255 },  // row 6 (no keys mapped)

    // RIGHT HALF (rows 7-13, LEDs 33-65)
    //   col0  col1  col2  col3  col4  col5  col6
    {   255,   54,   55,   255,   57,   58,   255 },  // row 7
    {    59,   255,   53,   52,   51,   50,   255 },  // row 8
    {    49,   48,   255,   42,   43,   44,   255 },  // row 9
    {    45,   46,   47,   255,   41,   40,   255 },  // row 10
    {    39,   37,   36,   255,  255,   38,   255 },  // row 11
    {   255,  255,  255,   255,  255,   255,  255 },  // row 12 (no keys mapped)
    {   255,  255,  255,   255,  255,   255,  255 },  // row 13 (no keys mapped)
};
