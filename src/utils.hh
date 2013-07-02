#pragma once

#include <iostream>
#include <string>
#include <boost/regex.hpp>

namespace Color {

extern const char* COLOR_NAMES[]; 

enum ColorName
{
    RED = 0
        , BOLD_RED
        , BROWN
        , BOLD_BROWN
        , CYAN
        , BOLD_CYAN
        , LIGHT_GRAY
        , BOLD_LIGHT_GRAY
        , BLUE
        , BOLD_BLUE
        , RESET
};

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream, ColorName resetCol = RESET );


} // namespace Color
