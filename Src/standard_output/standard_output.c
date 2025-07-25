#include "standard_output.h"
#include "cmsis_os2.h"
#include "fsl_debug_console.h"

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <embfmt/embformat.h>

#define STDIO_OUTPUT_LINEBUF_LEN (2048)
static char lineBuf[STDIO_OUTPUT_LINEBUF_LEN + 1];

static uint32_t insert_carriage_return(char *str, uint32_t maxLen) {
    uint32_t len = strlen(str);
    for (uint32_t i = 0; (i < len) && (len <= maxLen); i++) {
        // if a \n is found and no \r precedes it or it is the first character os the string
        if ((str[i] == '\n') && ((i == 0) || (str[i - 1] != '\r'))) {
            len++;                               // increase length
            for (uint32_t k = len; k > i; k--) { // copy each character one position right
                str[k] = str[k - 1];
            }
            str[i] = '\r'; // insert '\r'
            i++;           // skip the current \r\n block
        }
    }
    str[len] = '\0';
    return len;
}

void MSG(const char *format, ...) {
    va_list vaArgP;
    va_start(vaArgP, format);
    uint32_t lineLen = vembfmt(lineBuf, STDIO_OUTPUT_LINEBUF_LEN, format, vaArgP);
    va_end(vaArgP);
    lineLen = insert_carriage_return(lineBuf, STDIO_OUTPUT_LINEBUF_LEN); // insert carriage returns
    if (lineLen > 0) {
        // for (uint8_t i = 0; i < lineLen; i++) {
        //     DbgConsole_Putchar(lineBuf[i]); // send line to the CDC
        // }
        DbgConsole_Printf("%s", lineBuf);
    }
}

void MSGchar(int c) {
    DbgConsole_Putchar((int)c);
}

void MSGraw(const char *str) {
    for (uint8_t i = 0; i < strlen(str); i++) {
        DbgConsole_Putchar(str[i]); // send line to the CDC
    }
}
