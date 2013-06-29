#include "Rules.hh"

namespace Color {

std::string colorizeString( const boost::smatch aMatch, ColorName aColor, ColorName aResetCol = RESET )
{
    std::stringstream lStream;
    if ( aMatch[0].str().size() )
    {
        color( aColor, aMatch[0].str(), lStream, aResetCol );
    }
    return lStream.str();
}

void IntermediateResult::putMarker( size_t aIndex, RuleIndex aRuleIndex )
{
}

void IntermediateResult::getMarkers( size_t aIndex, std::vector< RuleIndex >& aRules ) const
{
}

Rule::Rule( ColorName aColor, const std::string& aRegex, const RuleIndex aIndex, bool aWholeLines ) 
    : IRule( aIndex )
      , m_WholeLines( aWholeLines )
      , m_Color( aColor )
      , m_Regex( aRegex )
    {
    }


void Rule::apply( const std::string& aLine
        , IntermediateResult& aResContainer
        , uint64_t aLineNumber ) const
{
//    if ( mWholeLines )
//    {
//        if ( boost::regex_search( aLine, mRegex ) )
//        {
//            std::stringstream lStream;
//            color( mColor, aLine, lStream, aResetCol );
//            return lStream.str();
//        }
//        return aLine; 
//    }
//    else
//    {
//        Colorizer lColorizer = std::tr1::bind( colorizeString
//                , std::tr1::placeholders::_1, mColor, aResetCol );
//        return boost::regex_replace( aLine, mRegex, lColorizer );
//    }
}

NumberRule::NumberRule( const ColorName aInitialColor
        , const RuleIndex aRuleIndex 
        , const uint8_t aSimilarLinesCount ) 
    : IRule( aRuleIndex )
      , m_SimilarLinesCount( aSimilarLinesCount )
      , m_Colors()
{
    if ( !m_SimilarLinesCount )
        throw std::runtime_error( "Number of similar lines cannot be zero" );
    m_Colors.push_back(aInitialColor);
}

void NumberRule::addColor( const ColorName aColor )
{
    m_Colors.push_back( aColor );
}

void NumberRule::apply( const std::string& aLine
        , IntermediateResult& aResContainer
        , uint64_t aLineNumber ) const
{
//    const uint16_t lColorIndex( ( aLineNumber / mSimilarLinesCount ) % mColors.size() );
//    const ColorName lColor( mColors[ lColorIndex ] );
//    std::stringstream lStream;
//    color( lColor, aLine, lStream );
//    aResetCol = lColor; // it will be passed to the next rule
//    return lStream.str();
}


} // namespace Color
