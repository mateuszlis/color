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
        , RESET
};

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream );


} // namespace Color
