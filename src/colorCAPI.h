#pragma once
#include <cstddef>

enum ColorErrorCode
{
    OK = 0,
    INIT_NOT_CALLED_SETTINGS_DEFAULT,
    FAILED_TO_PARSE_ARGUMENTS,
    RESULT_LINE_TOO_LONG
};


#ifdef __cplusplus
extern "C"
#endif
/**
 * @brief Initialize color with appropriate arguments
 * Should be called simirarly to command line args passed
 * to binary of color. If is not called, default arguments 
 * will be assumed (mode syslog)
 **/
void color_init( int argc, char** argv );

#ifdef __cplusplus
extern "C"
#endif
/**
 * @brief colorizes C-string given in aCString and puts it in aDest.
 * Does not allocate anything. User is responsible for allocation
 * of aDest. Will not write more than aDestSize chars.
 *
 * @param aCString - string to colorize
 * @param aDest - returned coloured string (could be the same)
 * @param aDestSize - will not write more than aDestSize chars
 *
 * @return colorErrorCode
 **/
ColorErrorCode color_colorize( char* aCString, char* aDest, size_t aDestSize );


