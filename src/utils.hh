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

template < ColorName TCOLOR >
std::string colorizeString( const boost::smatch aMatch )
{
    std::stringstream lStream;
    color( TCOLOR, aMatch[0].str(), lStream );
    return lStream.str();
}

} // namespace Color
