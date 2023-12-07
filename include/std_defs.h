#pragma once
#include <stdint.h>
const uint8_t ReturnSuccess = 0;
const uint8_t ReturnFail = 1;

#ifdef LOG_MSG_PORT
#define MSG_PORT LOG_MSG_PORT
#else
#define MSG_PORT Serial
#endif

#ifndef LOG_MSG_BASIC
//#define LOG_MSG_BASIC 0       // off
#define LOG_MSG_BASIC 1         // on
#endif

#ifndef LOG_MSG_LEVEL
//#define LOG_MSG_LEVEL 0         // off
#define LOG_MSG_LEVEL 1         // ERROR
//#define LOG_MSG_LEVEL 2         // ERROR, WARN
//#define LOG_MSG_LEVEL 3         // ERROR, WARN, INFO
#endif

#ifndef LOG_MSG_DEBUG
#define LOG_MSG_DEBUG 0  // off
//#define LOG_MSG_DEBUG 1         // on
#endif

#ifndef LOG_MSG_TOUCH_DEBUG
#define LOG_MSG_TOUCH_DEBUG 0  // off
//#define LOG_MSG_TOUCH_DEBUG 1         // on
#endif
#ifndef USE_DEBUG_PINS
#define USE_DEBUG_PINS 0  // off
#endif
#define INIT_DEBUG_PIN(x) \
    if (USE_DEBUG_PINS > 0) { \
        pinMode(x, OUTPUT);   \
    }
#define WRITE_DEBUG_PIN(x, y) \
    if (USE_DEBUG_PINS > 0) { \
        digitalWrite(x, y);   \
    }

#define MSG_BASICLN(x)       \
    if (LOG_MSG_BASIC) {     \
        MSG_PORT.println(x); \
    }
#define MSG_BASIC(x)       \
    if (LOG_MSG_BASIC) {   \
        MSG_PORT.print(x); \
    }
#define MSG_BASIC1(x, y)     \
    if (LOG_MSG_BASIC) {     \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(y); \
    }
#define MSG_BASIC1F(x, y)       \
    if (LOG_MSG_BASIC) {        \
        MSG_PORT.printf(x, y);  \
    }
#define MSG_BASIC2(x, y, z)  \
    if (LOG_MSG_BASIC) {     \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.print(y);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(z); \
    }
#define MSG_BASIC3(x, y, z, w) \
    if (LOG_MSG_BASIC) {       \
        MSG_PORT.print(x);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(y);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(z);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.println(w);   \
    }

#define MSG_ERRORLN(x)       \
    if (LOG_MSG_LEVEL > 0) { \
        MSG_PORT.println(x); \
    }
#define MSG_ERROR(x)         \
    if (LOG_MSG_LEVEL > 0) { \
        MSG_PORT.print(x);   \
    }
#define MSG_ERROR1(x, y)     \
    if (LOG_MSG_LEVEL > 0) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(y); \
    }
#define MSG_ERROR1F(x, y)      \
    if (LOG_MSG_LEVEL > 0) {   \
        MSG_PORT.printf(x, y); \
    }
#define MSG_ERROR2(x, y, z)  \
    if (LOG_MSG_LEVEL > 0) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.print(y);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(z); \
    }
#define MSG_ERROR3(x, y, z, w) \
    if (LOG_MSG_LEVEL > 0) {   \
        MSG_PORT.print(x);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(y);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(z);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.println(w);   \
    }

#define MSG_WARNLN(x)        \
    if (LOG_MSG_LEVEL > 1) { \
        MSG_PORT.println(x); \
    }
#define MSG_WARN(x)          \
    if (LOG_MSG_LEVEL > 1) { \
        MSG_PORT.print(x);   \
    }
#define MSG_WARN1(x, y)      \
    if (LOG_MSG_LEVEL > 1) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(y); \
    }
#define MSG_WARN1F(x, y)      \
    if (LOG_MSG_LEVEL > 1) {   \
        MSG_PORT.printf(x, y); \
    }
#define MSG_WARN2(x, y, z)   \
    if (LOG_MSG_LEVEL > 1) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.print(y);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(z); \
    }
#define MSG_WARN3(x, y, z, w) \
    if (LOG_MSG_LEVEL > 1) {  \
        MSG_PORT.print(x);    \
        MSG_PORT.print(" ");  \
        MSG_PORT.print(y);    \
        MSG_PORT.print(" ");  \
        MSG_PORT.print(z);    \
        MSG_PORT.print(" ");  \
        MSG_PORT.println(w);  \
    }

#define MSG_INFOLN(x)        \
    if (LOG_MSG_LEVEL > 2) { \
        MSG_PORT.println(x); \
    }
#define MSG_INFO(x)          \
    if (LOG_MSG_LEVEL > 2) { \
        MSG_PORT.print(x);   \
    }
#define MSG_INFO1(x, y)      \
    if (LOG_MSG_LEVEL > 2) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(y); \
    }
#define MSG_INFO1F(x, y)       \
    if (LOG_MSG_LEVEL > 2) {   \
        MSG_PORT.printf(x, y); \
    }
#define MSG_INFO2(x, y, z)   \
    if (LOG_MSG_LEVEL > 2) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.print(y);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(z); \
    }
#define MSG_INFO3(x, y, z, w) \
    if (LOG_MSG_LEVEL > 2) {  \
        MSG_PORT.print(x);    \
        MSG_PORT.print(" ");  \
        MSG_PORT.print(y);    \
        MSG_PORT.print(" ");  \
        MSG_PORT.print(z);    \
        MSG_PORT.print(" ");  \
        MSG_PORT.println(w);  \
    }

#define MSG_DEBUGLN(x)       \
    if (LOG_MSG_DEBUG > 0) { \
        MSG_PORT.println(x); \
    }
#define MSG_DEBUG(x)         \
    if (LOG_MSG_DEBUG > 0) { \
        MSG_PORT.print(x);   \
    }
#define MSG_DEBUG1(x, y)     \
    if (LOG_MSG_DEBUG > 0) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(y); \
    }
#define MSG_DEBUG1F(x, y)      \
    if (LOG_MSG_DEBUG > 0) {   \
        MSG_PORT.printf(x, y); \
    }
#define MSG_DEBUG2(x, y, z)  \
    if (LOG_MSG_DEBUG > 0) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.print(y);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(z); \
    }
#define MSG_DEBUG3(x, y, z, w) \
    if (LOG_MSG_DEBUG > 0) {   \
        MSG_PORT.print(x);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(y);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(z);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.println(w);   \
    }

#define MSG_TOUCH_DEBUGLN(x)       \
    if (LOG_MSG_TOUCH_DEBUG > 0) { \
        MSG_PORT.println(x); \
    }
#define MSG_TOUCH_DEBUG(x)         \
    if (LOG_MSG_TOUCH_DEBUG > 0) { \
        MSG_PORT.print(x);   \
    }
#define MSG_TOUCH_DEBUG1(x, y)     \
    if (LOG_MSG_TOUCH_DEBUG > 0) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(y); \
    }
#define MSG_TOUCH_DEBUG1F(x, y)      \
    if (LOG_MSG_TOUCH_DEBUG > 0) {   \
        MSG_PORT.printf(x, y); \
    }
#define MSG_TOUCH_DEBUG2(x, y, z)  \
    if (LOG_MSG_TOUCH_DEBUG > 0) { \
        MSG_PORT.print(x);   \
        MSG_PORT.print(" "); \
        MSG_PORT.print(y);   \
        MSG_PORT.print(" "); \
        MSG_PORT.println(z); \
    }
    
#define MSG_TOUCH_DEBUG3(x, y, z, w) \
    if (LOG_MSG_TOUCH_DEBUG > 0) {   \
        MSG_PORT.print(x);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(y);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.print(z);     \
        MSG_PORT.print(" ");   \
        MSG_PORT.println(w);   \
    }
    