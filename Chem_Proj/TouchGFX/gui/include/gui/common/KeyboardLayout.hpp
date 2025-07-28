#ifndef KEYBOARD_LAYOUT
#define KEYBOARD_LAYOUT

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/hal/Types.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include "BitmapDatabase.hpp"

using namespace touchgfx;

/**
 * Array specifying the keys used in the CustomKeyboard.
 */
static const Keyboard::Key keyArray[30] =
{
    // Row 1: Original y=61, height=40. Scaled y=46, height=30. Original key width=26. Scaled key width=20.
    // Original x start=7, x increment=28. Scaled x start=5, x increment=21.
    { 1, Rect(5, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 2, Rect(5 + 21, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 3, Rect(5 + 21 * 2, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 4, Rect(5 + 21 * 3, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 5, Rect(5 + 21 * 4, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 6, Rect(5 + 21 * 5, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 7, Rect(5 + 21 * 6, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 8, Rect(5 + 21 * 7, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 9, Rect(5 + 21 * 8, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {10, Rect(5 + 21 * 9, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {11, Rect(5 + 21 * 10, 46, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    // Row 2: Original y=105, height=40. Scaled y=79, height=30. Original key width=26. Scaled key width=20.
    // Original x start=7, x increment=28. Scaled x start=5, x increment=21.
    {12, Rect(5, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {13, Rect(5 + 21, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {14, Rect(5 + 21 * 2, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {15, Rect(5 + 21 * 3, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {16, Rect(5 + 21 * 4, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {17, Rect(5 + 21 * 5, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {18, Rect(5 + 21 * 6, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {19, Rect(5 + 21 * 7, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {20, Rect(5 + 21 * 8, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {21, Rect(5 + 21 * 9, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {22, Rect(5 + 21 * 10, 79, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    // Row 3: Original y=149, height=40. Scaled y=112, height=30. Original key width=26. Scaled key width=20.
    // Original x start=63, x increment=28. Scaled x start=47, x increment=21.
    {23, Rect(47, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {24, Rect(47 + 21, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {25, Rect(47 + 21 * 2, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {26, Rect(47 + 21 * 3, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {27, Rect(47 + 21 * 4, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {28, Rect(47 + 21 * 5, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {29, Rect(47 + 21 * 6, 112, 20, 30), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    
    // Spacebar: Original Rect(62, 193, 252, 40). Scaled Rect(47, 145, 189, 30).
    {30, Rect(47, 145, 189, 30), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID}
};

/**
 * Callback areas for the special buttons on the CustomKeyboard.
 */
static Keyboard::CallbackArea callbackAreas[3] =
{
    // caps-lock: Original Rect(5, 149, 56, 40). Scaled Rect(4, 112, 42, 30).
    {Rect(4, 112, 42, 30), 0, BITMAP_KEYBOARD_KEY_SHIFT_HIGHLIGHTED_ID},     // caps-lock
    // backspace: Original Rect(258, 149, 56, 40). Scaled Rect(194, 112, 42, 30).
    {Rect(194, 112, 42, 30), 0, BITMAP_KEYBOARD_KEY_DELETE_HIGHLIGHTED_ID},  // backspace
    // mode: Original Rect(5, 193, 56, 40). Scaled Rect(4, 145, 42, 30).
    {Rect(4, 145, 42, 30), 0, BITMAP_KEYBOARD_KEY_NUM_HIGHLIGHTED_ID},       // mode
};

/**
 * The layout for the CustomKeyboard.
 */
static const Keyboard::Layout layout =
{
    BITMAP_KEYBOARD_BACKGROUND_ID,
    keyArray,
    30,
    callbackAreas,
    3,
    // Text input area: Original Rect(15, 10, 11 * 28, 40). Scaled Rect(11, 8, 231, 30).
    Rect(11, 8, 231, 30),
    TypedText(T_ENTEREDTEXT),
    0xFFFFFF,
    Typography::KEYBOARD, // You might need to adjust this Typography in your texts.xlsx if the font doesn't look right at the new size.
    0
};

#endif