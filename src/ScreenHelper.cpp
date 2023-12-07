#include "ScreenHelper.h"
/**
* @brief This function reads chuncks of 2 bytes of data from a
         file and returns the data.
*
* @param &f
*
* @return uint16_t
*
* @note litte-endian
*/
uint16_t read16(fs::File &f)
{
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read();  // LSB
    ((uint8_t *)&result)[1] = f.read();  // MSB
    return result;
}

/**
* @brief This function reads chuncks of 4 bytes of data from a
         file and returns the data.
*
* @param &f
*
* @return uint32_t
*
* @note litte-endian
*/
uint32_t read32(fs::File &f)
{
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read();  // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read();  // MSB
    return result;
}

/**
* @brief This functions accepts a HTML including the # colour code
         (eg. #FF00FF)  and returns it in RGB888 format.
*
* @param *html char (including #)
*
* @return unsigned long
*
* @note none
*/
unsigned long convertHTMLtoRGB888(char *html)
{
    char *hex = html + 1;  // remove the #
    unsigned long rgb = strtoul(hex, NULL, 16);
    return rgb;
}

/**
* @brief This functions accepts a RGB888 format color and returns an HTML including the # colour code
         (eg. #FF00FF).
*
* @param unsigned long
*
* @return *html char (including #)
*
* @note none
*/
void convertRGB888toHTML(unsigned long rgb, char *html, unsigned int htmlsize)
{
    snprintf(html, htmlsize, "#%6.6X", rgb & 0x00ffffff);
}

/**
 * @brief This function converts RGB888 to RGB565.
 *
 * @param rgb unsigned long
 *
 * @return unsigned int
 *
 * @note none
 */
unsigned int convertRGB888ToRGB565(unsigned long rgb)
{
    return (((rgb & 0xf80000) >> 8) | ((rgb & 0xfc00) >> 5) | ((rgb & 0xf8) >> 3));
}

/**
 * @brief This function converts RGB565 to RGB888.
 *
 * @param rgb unsigned int
 *
 * @return unsigned long
 *
 * @note none
 */
unsigned long convertRGB565ToRGB888(unsigned int rgb)
{
    return (((rgb & 0xf800) << 8) | ((rgb & 0x7e0) << 5) | ((rgb & 0x1f) << 3));
}

/**
 * @brief This function converts HTML color code to RGB565.
 *
 * @param *html char (including #)
 *
 * @return unsigned int
 *
 * @note none
 */
unsigned int convertHTMLtoRGB565(char *html)
{
    unsigned long rgb888 = convertHTMLtoRGB888(html);
    return convertRGB888ToRGB565(rgb888);
}

/**
 * @brief This function converts RGB565 to HTML color code.
 *
 * @param unsigned int
 *
 * @return *html char (including #)
 *
 * @note none
 */
void convertRGB565toHTML(unsigned int rgb, char *html, unsigned int htmlsize)
{
    unsigned long rgb888 = convertRGB565ToRGB888(rgb);
    convertRGB888toHTML(rgb888, html, htmlsize);
}

/**
* @brief This function draws a BMP on the TFT screen according
         to the given x and y coordinates.
*
* @param  *filename
* @param x int16_t
* @param y int16_t
* @param transparent uint8_t
*
* @return none
*
* @note If transparent is true then black pixels are not drawn (i.e. they are transparent). If false,
*       black pixels are drawn.
*/
void drawBmp(const char *filename, int16_t x, int16_t y, uint8_t transparent)
{
    if ((x >= tft.width()) || (y >= tft.height()))
        return;

    fs::File bmpFS;

    bmpFS = FILESYSTEM.open(filename, "r");

    if (!bmpFS) {
        MSG_WARNLN("[WARNING]: Bitmap not found: ");
        MSG_WARNLN(filename);
        filename = "/logos/question.bmp";
        bmpFS = FILESYSTEM.open(filename, "r");
    }

    uint32_t seekOffset;
    uint16_t w, h, row;
    uint8_t r, g, b;

    if (read16(bmpFS) == 0x4D42) {
        read32(bmpFS);
        read32(bmpFS);
        seekOffset = read32(bmpFS);
        read32(bmpFS);
        w = read32(bmpFS);
        h = read32(bmpFS);

        if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0)) {
            y += h - 1;

            bool oldSwapBytes = tft.getSwapBytes();
            tft.setSwapBytes(true);
            bmpFS.seek(seekOffset);

            uint16_t padding = (4 - ((w * 3) & 3)) & 3;
            uint8_t lineBuffer[w * 3 + padding];

            for (row = 0; row < h; row++) {
                bmpFS.read(lineBuffer, sizeof(lineBuffer));
                uint8_t *bptr = lineBuffer;
                uint16_t *tptr = (uint16_t *)lineBuffer;
                // Convert 24 to 16 bit colours
                for (uint16_t col = 0; col < w; col++) {
                    b = *bptr++;
                    g = *bptr++;
                    r = *bptr++;
                    *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                }

                // Push the pixel row to screen, pushImage will crop the line if needed
                // y is decremented as the BMP image is drawn bottom up

                if (transparent) {
                    tft.pushImage(x, y--, w, 1, (uint16_t *)lineBuffer, TFT_BLACK);
                }
                else {
                    tft.pushImage(x, y--, w, 1, (uint16_t *)lineBuffer);
                }
            }
            tft.setSwapBytes(oldSwapBytes);
        }
        else
            MSG_ERROR1("[ERROR] BMP format not recognized for file: ", filename);
    }
    bmpFS.close();
}

/**
* @brief This function draws a BMP on the TFT screen according
         to the given x and y coordinates.
*
* @param pImage uint16_t*
* @param x int16_t
* @param y int16_t
* @param transparent uint8_t
*
* @return uint8_t 0 = success, other = error
*
* @note If transparent is true then black pixels are not drawn (i.e. they are transparent). If false,
*       black pixels are drawn.
*/
uint8_t drawLogoFromPSRAM(uint16_t *pImage, int16_t x, int16_t y, uint8_t transparent)
{
    uint16_t *pImagePtr;
    uint16_t w, h, row;

    uint8_t status = ReturnSuccess;

    if ((x >= tft.width()) || (y >= tft.height()))
        return ReturnFail;

    if (pImage != nullptr) {
        w = pImage[0];
        h = pImage[1];
        pImagePtr = pImage + 2;

        // y += h - 1;

        bool oldSwapBytes = tft.getSwapBytes();
        tft.setSwapBytes(true);

        if (transparent) {
            tft.pushImage(x, y, w, h, pImagePtr, TFT_BLACK);
        }
        else {
            tft.pushImage(x, y, w, h, pImagePtr);
        }

        tft.setSwapBytes(oldSwapBytes);
    }

    return status;
}

/**
* @brief This function loads a logo file into a buffer allocated in PSRAM. This gets all of the (slow)
         filesystem accesses out of the way once. After that the logo can be drawn directly from PSRAM.
*
* @param  *filename
* @param pImage *uint16_t
*
* @return 0 for success, other for failure
*
* @note Make sure to check the status of the return value. If it is not equal to zero, then the logo was not loaded.
*/
uint8_t loadBmpToPSRAM(const char *filename, uint16_t **pImage)
{
    uint8_t status = ReturnSuccess;

    fs::File bmpFS;
    bmpFS = FILESYSTEM.open(filename, "r");

    if (!bmpFS) {
        MSG_WARN1("[WARN]: Bitmap not found: ", filename);
        filename = "/logos/question.bmp";
        MSG_WARN1("[WARN]: Will use : ", filename);
        bmpFS = FILESYSTEM.open(filename, "r");
    }

    if (!bmpFS) {
        status = ReturnFail;
        MSG_ERROR1("[ERROR] Failed to open file: ", filename);
    }
    else {
        uint32_t seekOffset;
        uint16_t w, h;
        uint8_t r, g, b;
        uint16_t *pImagePtr = 0;

        if (read16(bmpFS) == 0x4D42) {
            read32(bmpFS);
            read32(bmpFS);
            seekOffset = read32(bmpFS);
            read32(bmpFS);
            w = read32(bmpFS);
            h = read32(bmpFS);

            if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0)) {
                uint16_t imageSize = 2 + 2 + (w * h * 2);  // 2 bytes for width, 2 bytes for height, 2 bytes per pixel
                uint16_t *pLogoImage = (uint16_t *)ps_malloc(imageSize);

                if (pLogoImage == NULL) {
                    MSG_ERROR1("[ERROR] Could not allocate memory for image: ", filename);
                    status = ReturnFail;
                }
                else {
                    pLogoImage[0] = w;
                    pLogoImage[1] = h;
                    pImagePtr = pLogoImage + 2;
                }

                if (status == ReturnSuccess) {
                    bmpFS.seek(seekOffset);

                    uint16_t padding = (4 - ((w * 3) & 3)) & 3;
                    uint8_t lineBuffer[w * 3 + padding];

                    for (int row = h - 1; row >= 0; row--) {
                        bmpFS.read(lineBuffer, sizeof(lineBuffer));
                        uint8_t *bptr = lineBuffer;
                        uint16_t *tptr = (uint16_t *)lineBuffer;
                        // Convert 24 to 16 bit colours
                        for (int col = 0; col < w; col++) {
                            b = *bptr++;
                            g = *bptr++;
                            r = *bptr++;
                            pImagePtr[row * w + col] = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                        }
                    }
                    *pImage = pLogoImage;
                }
            }
            else {
                MSG_ERROR1("[ERROR] BMP format not recognized for file: ", filename);
                status = ReturnFail;
            }
        }
        bmpFS.close();
    }

    return status;
}

/**
* @brief This function reads a number of bytes from the given
         file at the given position.
*
* @param *p_file File
* @param position int
* @param nBytes byte
*
* @return int32_t
*
* @note none
*/
int32_t readNbytesInt(File *p_file, int position, byte nBytes)
{
    if (nBytes > 4)
        return 0;

    p_file->seek(position);

    int32_t weight = 1;
    int32_t result = 0;
    for (; nBytes; nBytes--) {
        result += weight * p_file->read();
        weight <<= 8;
    }
    return result;
}

/**
* @brief This function reads the RGB565 colour of the first pixel for a
         given the logo number. The pagenumber is global.
*
* @param *filename const char
*
* @return uint16_t
*
* @note Uses readNbytesInt
*/
uint16_t getBMPColor(const char *filename)
{
    uint16_t color = 0x0000;

    // Open File
    File bmpImage;
    bmpImage = FILESYSTEM.open(filename, FILE_READ);

    int32_t dataStartingOffset = readNbytesInt(&bmpImage, 0x0A, 4);
    int16_t pixelsize = readNbytesInt(&bmpImage, 0x1C, 2);

    if (pixelsize != 24) {
        MSG_ERROR2("[ERROR] getBMPColor: ", filename, " Image is not 24 bpp");
        MSG_ERROR2("        logo is *", filename, "*");
        color = 0x0000;
    }
    else {
        // Read first pixel
        bmpImage.seek(dataStartingOffset);  // skip bitmap header

        byte R, G, B;

        B = bmpImage.read();
        G = bmpImage.read();
        R = bmpImage.read();

        bmpImage.close();

        color = tft.color565(R, G, B);
    }

    return color;
}

/**
* @brief This function returns the RGB565 colour of the first pixel in a logo for a
         given button. The pagenumber is global.
*
* @param logonumber int
*
* @return uint16_t
*
* @note Uses getBMPColor to read the actual image data.
*/
uint16_t getImageBG(uint8_t page, uint8_t row, uint8_t col)
{
    uint16_t bg_color = 0x0000;

    if ((page >= 0) && (page < NUM_PAGES)) {
        if ((row >= 0) && (row < BUTTON_ROWS) && (col >= 0) && (col < BUTTON_COLS)) {
            bg_color = getBMPColor(menu[page].button[row][col].logo);
        }
        else {
            MSG_ERROR3("[ERROR] getImageBG: Invalid logo index ", page, row, col);
            bg_color = 0x0000;
        }
    }
    else {
        MSG_ERROR1("[ERROR] getImageBG: Invalid pageNum", page);
        bg_color = 0x0000;
    }
    return bg_color;
}

/**
 * @brief This function returns the RGB565 colour of the first pixel of the image which
 *          is being latched to for a given the logo number. The pagenumber is global.
 *
 * @param logonumber int
 *
 * @return uint16_t
 *
 * @note Uses getBMPColor to read the actual image data.
 */
uint16_t getLatchImageBG(uint8_t page, uint8_t row, uint8_t col)
{
    uint16_t bg_color;

    if ((page >= 0) && (page < NUM_PAGES)) {
        if ((row >= 0) && (row < BUTTON_ROWS) && (col >= 0) && (col < BUTTON_COLS)) {
            if (strcmp(menu[page - 1].button[row][col].latchlogo, "/logos/") == 0) {
                bg_color = getBMPColor(menu[page - 1].button[row][col].logo);
            }
            else {
                bg_color = getBMPColor(menu[page - 1].button[row][col].latchlogo);
            }
        }
        else {
            MSG_ERRORLN("[ERROR] getLatchImageBG: Invalid latch logo index");
            bg_color = 0x0000;
        }
    }
    else {
        MSG_ERRORLN("[ERROR] getLatchImageBG: Invalid page");
        bg_color = 0x0000;
    }
    return bg_color;
}
