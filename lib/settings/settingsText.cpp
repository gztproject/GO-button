#include <Arduino.h>
#include "settings.h"
namespace SETTINGS
{
    /**
     * @brief Converts a byte to 2 HEX chars
     *
     * @param byte Input byte
     * @param ch Output char buffer (must hold 2 chars)
     */
    void formatHex(uint8_t byte, char *ch)
    {
        uint8_t first = (byte >> 4);    // isolate first 4 bits
        uint8_t second = (byte & 0x0F); // isolate last 4 bits;

        if (first < 10U)
            first = ('0' + first);
        else
            first = ('A' + first - 10);

        if (second < 10U)
            second = ('0' + second);
        else
            second = ('A' + second - 10);

        ch[0] = first;
        ch[1] = second;
        ch[2] = 0;
    }
   
    /**
     * @brief Converts a byte to an array of chars (string representation)
     *
     * @param byte Input byte
     * @param ch Output char buffer
     */
    void formatByte(uint8_t byte, char *ch)
    {
        uint8_t i = 0;
        uint8_t b = byte;
        char c[3];
        do
        {
            c[i] = 48 + (b % 10);
            b = b / 10;
            i++;
        } while (b > 0);

        for (uint8_t j = 0; j < i; j++)
        {
            ch[j] = c[i - j - 1];
        }
        ch[i] = 0x00;
    }

    /**
     * @brief Converts a word (2 bytes) to an array of chars (string representation)
     *
     * @param byte Input word
     * @param ch Output char buffer
     */
    void formatWord(uint16_t dbl, char *ch)
    {
        uint8_t i = 0;
        uint16_t b = dbl;
        char c[5];
        do
        {
            c[i] = 48 + (b % 10);
            b = b / 10;
            i++;
        } while (b > 0);
        // in c[5] digits are IN REVERSE ORDER
        for (uint8_t j = 0; j < i; j++)
        {
            ch[j] = c[i - j - 1];
        }
        ch[i] = 0x00;
    }
}