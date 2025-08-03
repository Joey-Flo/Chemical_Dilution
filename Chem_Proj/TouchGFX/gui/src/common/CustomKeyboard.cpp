#include <gui/common/CustomKeyboard.hpp>
#include <string.h>
#include <touchgfx/Color.hpp>

CustomKeyboard::CustomKeyboard() : keyboard(),
    modeBtnTextArea(),
    capslockPressed(this, &CustomKeyboard::capslockPressedHandler),
    backspacePressed(this, &CustomKeyboard::backspacePressedHandler),
    modePressed(this, &CustomKeyboard::modePressedHandler),
    keyPressed(this, &CustomKeyboard::keyPressedhandler),
    alphaKeys(true),
    uppercaseKeys(false),
    firstCharacterEntry(false)
{
    //Set the callbacks for the callback areas of the keyboard and set its layout.
    layout.callbackAreaArray[0].callback = &capslockPressed;
    layout.callbackAreaArray[1].callback = &backspacePressed;
    layout.callbackAreaArray[2].callback = &modePressed;
    keyboard.setLayout(&layout);
    keyboard.setKeyListener(keyPressed);
    
    // Original: keyboard.setPosition(0, 0, 320, 240);
    // Changed to: keyboard.setPosition(0, 0, 240, 180);
    // Explanation: The keyboard's width (320) is scaled by 0.75 to 240.
    // The keyboard's height (240) is also scaled by 0.75 to 180 to maintain aspect ratio.
    // This assumes the overall height of your new screen layout allows for a 180px high keyboard.
    keyboard.setPosition(0, 0, 240, 180); 
    
    keyboard.setTextIndentation();
    //Allocate the buffer associated with keyboard.
    memset(buffer, 0, sizeof(buffer));
    keyboard.setBuffer(buffer, BUFFER_SIZE);

    uppercaseKeys = true;
    firstCharacterEntry = true;

    // Original: modeBtnTextArea.setPosition(5, 196, 56, 40);
    // Changed to: modeBtnTextArea.setPosition(4, 147, 42, 30);
    // Explanation:
    // X-position: 5 * 0.75 = 3.75, rounded to 4.
    // Y-position: 196 * 0.75 = 147.
    // Width: 56 * 0.75 = 42.
    // Height: 40 * 0.75 = 30.
    modeBtnTextArea.setPosition(4, 147, 42, 30);
    
    modeBtnTextArea.setColor(Color::getColorFromRGB(0xFF, 0xFF, 0xFF));

    setKeyMappingList();

    add(keyboard);
    add(modeBtnTextArea);
}

void CustomKeyboard::setKeyMappingList()
{
    if (alphaKeys)
    {
        modeBtnTextArea.setTypedText(TypedText(T_ALPHAMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListAlphaUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListAlphaLower);
        }
    }
    else
    {
        modeBtnTextArea.setTypedText(TypedText(T_NUMMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListNumUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListNumLower);
        }
    }
}

void CustomKeyboard::backspacePressedHandler()
{
    uint16_t pos = keyboard.getBufferPosition();
    if (pos > 0)
    {
        //Delete the previous entry in the buffer and decrement the position.
        buffer[pos - 1] = 0;
        keyboard.setBufferPosition(pos - 1);

        //Change keymappings if we have reached the first position.
        if (1 == pos)
        {
            firstCharacterEntry = true;
            uppercaseKeys = true;
            setKeyMappingList();
        }
    }
}

void CustomKeyboard::capslockPressedHandler()
{
    uppercaseKeys = !uppercaseKeys;
    setKeyMappingList();
}

void CustomKeyboard::modePressedHandler()
{
    alphaKeys = !alphaKeys;

    // if we have changed back to alpha and still has no chars in the buffer,
    // we show upper case letters.
    if (firstCharacterEntry && alphaKeys)
    {
        uppercaseKeys = true;
    }
    else
    {
        uppercaseKeys = false;
    }
    setKeyMappingList();
}

void CustomKeyboard::keyPressedhandler(Unicode::UnicodeChar keyChar)
{
    // After the first keypress, the keyboard will shift to lowercase.
    if (firstCharacterEntry && keyChar != 0)
    {
        firstCharacterEntry = false;
        uppercaseKeys = false;
        setKeyMappingList();
    }
}

void CustomKeyboard::setTouchable(bool touch)
{
    Container::setTouchable(touch);
    keyboard.setTouchable(touch);
}

Unicode:: UnicodeChar* CustomKeyboard::getBuffer ()
{
    return keyboard.getBuffer();
}

void CustomKeyboard::clearBuffer ()
{
    memset(buffer, 0, BUFFER_SIZE+1);
    keyboard.setBufferPosition(0);
    firstCharacterEntry = true;
    uppercaseKeys = true;
    setKeyMappingList();
}