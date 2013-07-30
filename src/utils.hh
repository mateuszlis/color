#pragma once

#include <string>
#include <boost/regex.hpp>

namespace Color {

extern const char* COLOR_NAMES[]; 

static const std::string COLORS_LIST = "(BLACK|RED|GREEN|BROWN|BLUE|MAGENTA|CYAN|GRAY|BOLD_BLACK|BOLD_RED|BOLD_GREEN|BOLD_BROWN|BOLD_BLUE|BOLD_MAGENTA|BOLD_CYAN|BOLD_GRAY|RESET)";
enum ColorName
{
    BLACK = 0
        , BOLD_BLACK
        , RED 
        , BOLD_RED
        , GREEN
        , BOLD_GREEN
        , BROWN
        , BOLD_BROWN
        , BLUE
        , BOLD_BLUE
        , MAGENTA
        , BOLD_MAGENTA
        , CYAN
        , BOLD_CYAN
        , GRAY 
        , BOLD_GRAY
        , RESET
};

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream, ColorName resetCol = RESET );

void displayHelp();
void findConfig( std::ifstream& aStr );


} // namespace Color
