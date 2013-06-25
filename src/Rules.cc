#include "Rules.hh"

namespace Color {

std::string colorizeString( const boost::smatch aMatch, ColorName aColor )
{
    std::stringstream lStream;
    if ( aMatch[0].str().size() )
    {
        color( aColor, aMatch[0].str(), lStream );
    }
    return lStream.str();
}

Rule::Rule( ColorName aColor, const std::string& aRegex, bool aWholeLines ) 
    : mWholeLines( aWholeLines )
      , mColor( aColor )
      , mRegex( aRegex )
      , mColorizer( std::tr1::bind( colorizeString
                    , std::tr1::placeholders::_1, mColor ) )
    {
    }

std::string Rule::apply( const std::string& aLine, uint64_t aLineNumber ) const
{
    if ( mWholeLines )
    {
        if ( boost::regex_search( aLine, mRegex ) )
        {
            std::stringstream lStream;
            color( mColor, aLine, lStream );
            return lStream.str();
        }
        return aLine; 
    }
    else
    {
        return boost::regex_replace( aLine, mRegex, mColorizer );
    }
}

NumberRule::NumberRule( const uint8_t aSimilarLinesCount, const ColorName aInitialColor ) 
    : mSimilarLinesCount( aSimilarLinesCount )
      , mColors()
{
    if ( !mSimilarLinesCount )
        throw std::runtime_error( "Number of similar lines cannot be zero" );
    mColors.push_back(aInitialColor);
}

std::string NumberRule::apply( const std::string& aLine, uint64_t aLineNumber ) const
{
    const uint16_t lColorIndex( ( aLineNumber / mSimilarLinesCount ) % mColors.size() );
    const ColorName lColor( mColors[ lColorIndex ] );
    std::stringstream lStream;
    color( lColor, aLine, lStream );
    return lStream.str();
}


} // namespace Color
