#include "Rules.hh"

namespace Color {

Rule::Rule( ColorName aColor, const std::string& aRegex, bool aWholeLines ) 
    : mWholeLines( aWholeLines )
      , mColor( aColor )
      , mRegex( aRegex )
    {
        ;
    }

std::string Rule::apply( const std::string& aLine, uint64_t aLineNumber ) const
{
    return "";
}

NumberRule::NumberRule( const uint8_t aSimilarLinesCount, const ColorName aInitialColor ) 
    : mSimilarLinesCount( aSimilarLinesCount )
      , mColors()
{
    ;
}

std::string NumberRule::apply( const std::string& aLine, uint64_t aLineNumber ) const
{
    return "";
}


} // namespace Color
