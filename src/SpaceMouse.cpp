#include "SpaceMouse.h"

SpaceMouse::SpaceMouse(
    //    HardwareSerial &serial,
    unsigned long baud,
    uint32_t config,
    uint8_t rx,
    uint8_t tx)
{
    //    serialPort = serial;
    baudRate = baud;
    configParameters = config;
    pinRx = rx;
    pinTx = tx;
    cmd_end = '\r';
    buttonMask = 0;
    lastButtonMask = 0;
}

SpaceMouse::~SpaceMouse()
{
}

void SpaceMouse::Init()
{
    Serial1.begin(baudRate, configParameters, pinRx, pinTx);
}

void SpaceMouse::SendViewpointDataPacket(uint16_t x, uint16_t y, uint16_t z, uint16_t a, uint16_t b, uint16_t c)
{
    Serial1.write('d');
    SendUInt16(x);
    SendUInt16(y);
    SendUInt16(z);
    SendUInt16(a);
    SendUInt16(b);
    SendUInt16(c);
    Serial1.write(cmd_end);
}

void SpaceMouse::SendUInt16(uint16_t data)
{
    Serial1.write(EncodeNibble((uint8_t)(data >> (12) & 0x000f)));
    Serial1.write(EncodeNibble((uint8_t)(data >> (8) & 0x000f)));
    Serial1.write(EncodeNibble((uint8_t)(data >> (4) & 0x000f)));
    Serial1.write(EncodeNibble((uint8_t)(data & 0x000f)));
}

void SpaceMouse::SendButtonsUInt32(uint32_t buttons)
{
    uint8_t nibble = 0;

    Serial1.write('k');

    nibble = buttons & 0x0000000f;
    Serial1.write(EncodeNibble(nibble));

    nibble = (buttons >> 4) & 0x0000000f;
    Serial1.write(EncodeNibble(nibble));

    nibble = (buttons >> 8) & 0x0000000f;
    Serial1.write(EncodeNibble(nibble));

    nibble = (buttons >> 12) & 0x0000000f;
    Serial1.write(EncodeNibble(nibble));

    nibble = (buttons >> 16) & 0x0000000f;
    Serial1.write(EncodeNibble(nibble));

    Serial1.write(cmd_end);
}

void SpaceMouse::SendButtonPacket()
{
    if (buttonMask != lastButtonMask) {
        SendButtonsUInt32(buttonMask);
    }
    lastButtonMask = buttonMask;
}

void SpaceMouse::SetButtonMask(uint32_t newButtonMask)
{
    buttonMask = newButtonMask;
}

void SpaceMouse::SetButton(uint8_t button_index)
{
    if ((button_index < SPACEMOUSE_MAX_BUTTON) && (button_index > 0)) {
        buttonMask |= (1 << (button_index - 1));
    }
    else {
        MSG_ERRORLN("[ERROR] Invalid button index");
    }
}

void SpaceMouse::ClearButtonMask()
{
    buttonMask = 0;
}

void SpaceMouse::ClearButton(uint8_t button_index)
{
    if ((button_index < SPACEMOUSE_MAX_BUTTON) && (button_index > 0)) {
        buttonMask &= ~(1 << (button_index - 1));
    }
    else {
        MSG_ERRORLN("[ERROR] Invalid button index");
    }
}

// TODO: Rework to allow multiple buttons to be pressed at once
void SpaceMouse::SendKeyPacket(uint8_t button_index)
{
    Serial1.write('k');
    uint8_t nibble = 0;
    char encoded_zero = EncodeNibble(0);
    if (button_index == 0) {
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }
    else if (button_index < 5) {
        nibble = 1 << (button_index - 1);
        Serial1.write(EncodeNibble(nibble));
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }
    else if (button_index < 9) {
        Serial1.write(encoded_zero);
        nibble = 1 << (button_index - 5);
        Serial1.write(EncodeNibble(nibble));
        Serial1.write(encoded_zero);
    }
    else if (button_index < 13) {
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        nibble = 1 << (button_index - 9);
        Serial1.write(EncodeNibble(nibble));
    }
    else {
        MSG_ERRORLN("[ERROR] Invalid button index");
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }

    Serial1.write(cmd_end);
}

// This function is to allow more than the 12 buttons allowed by
// the standard SpaceMouse protocol so as to support the extra
// buttons on a SpaceMouse Pro. the 3dconnexxion information says that
// 15 are supported, but there is other information that there are
// actually 19 (15 + 4 for long press on 1-4).
//
// Two more bytes are added to the packet to allow for the full
// complement of 20 buttons to be supported.
// TODO: Rework to allow multiple buttons to be pressed at once
void SpaceMouse::SendKeyPacketExtended(uint8_t button_index)
{
    Serial1.write('k');
    uint8_t nibble = 0;
    char encoded_zero = EncodeNibble(0);
    if (button_index == 0) {
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }
    else if (button_index < 5) {
        nibble = 1 << (button_index - 1);
        Serial1.write(EncodeNibble(nibble));
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }
    else if (button_index < 9) {
        Serial1.write(encoded_zero);
        nibble = 1 << (button_index - 5);
        Serial1.write(EncodeNibble(nibble));
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }
    else if (button_index < 13) {
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        nibble = 1 << (button_index - 9);
        Serial1.write(EncodeNibble(nibble));
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }
    else if (button_index < 17) {
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        nibble = 1 << (button_index - 13);
        Serial1.write(EncodeNibble(nibble));
        Serial1.write(encoded_zero);
    }
    else if (button_index < 21) {
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        nibble = 1 << (button_index - 17);
        Serial1.write(EncodeNibble(nibble));
    }
    else {
        MSG_ERRORLN("[ERROR] Invalid button index");
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
        Serial1.write(encoded_zero);
    }

    Serial1.write(cmd_end);
}

char SpaceMouse::EncodeNibble(uint8_t nibble)
{
    //    Hex equivalents of char_codes are provided for convenience during debugging
    //
    //                   {0x30, 0x41, 0x42, 0x33, 0x44, 0x35,
    //                    0x36, 0x47, 0x48, 0x39, 0x3a, 0x4b,
    //                    0x3c, 0x4d, 0x4e, 0x3f};

    char char_codes[] = {'0', 'A', 'B', '3',
                         'D', '5', '6', 'G',
                         'H', '9', ':', 'K',
                         '<', 'M', 'N', '?'};

    char encoded_nibble = char_codes[nibble & 0x0f];

    return encoded_nibble;
}
