#include "Rules.hh"

namespace Color {

IntermediateResult::IntermediateResult( const size_t aSize ) 
    : m_RulesOnPositions( aSize )
      , m_IsOpened( false )
{}


void IntermediateResult::putMarker( size_t aIndex, const ColorName aColor )
{
    increaseSize( aIndex );
    m_RulesOnPositions[ aIndex ].push_back( Marker( m_IsOpened, aColor ) );
    m_IsOpened = !m_IsOpened; 
}

void IntermediateResult::getMarkers( size_t aIndex, Markers& aRules ) const
{
    if ( aIndex > m_RulesOnPositions.size() )
    {
        return;
    }
    return aRules.insert( aRules.end()
                        , m_RulesOnPositions[ aIndex ].begin()
                        , m_RulesOnPositions[ aIndex ].end() );
}

void IntermediateResult::increaseSize( const size_t aSize )
{
    while ( aSize >= m_RulesOnPositions.size() )
    {
        m_RulesOnPositions.resize( 2 * m_RulesOnPositions.size() );
    }
}

Rule::Rule( ColorName aColor, const std::string& aRegex, bool aWholeLines ) 
    : m_WholeLines( aWholeLines )
      , m_Color( aColor )
      , m_Regex( aRegex )
    {
    }


void Rule::apply( const std::string& aLine
        , IntermediateResult& aResContainer
        , uint64_t /*aLineNumber*/ ) const
{
    if ( m_WholeLines )
    {
        applyWholeLine( aLine, aResContainer );
    }
    else
    {
        applyPartial( aLine, aResContainer );
    }
}

void Rule::applyWholeLine( const std::string& aLine
                        , IntermediateResult& aResContainer ) const
{
    if ( boost::regex_search( aLine, m_Regex ) )
    {
        aResContainer.putMarker( 0, m_Color );
        aResContainer.putMarker( aLine.size(), m_Color );
    }
}

void Rule::applyPartial( const std::string& aLine
                        , IntermediateResult& aResContainer ) const
{
    boost::smatch lSearchRes;
    size_t lGlobal( 0 );
    std::string::const_iterator lStart( aLine.begin() )
                                , lEnd( aLine.end() );
    while ( boost::regex_search( lStart, lEnd, lSearchRes, m_Regex )
            && !( lGlobal == aLine.size() ) ) // protection against full match
    {
        aResContainer.putMarker( lGlobal + lSearchRes.position(), m_Color );
        aResContainer.putMarker( lGlobal + lSearchRes.position() + lSearchRes.length(), m_Color );
        lStart += lSearchRes.position() + lSearchRes.length();
        lGlobal += lSearchRes.position() + lSearchRes.length();
    }
}

NumberRule::NumberRule( const ColorName aInitialColor
        , const uint8_t aSimilarLinesCount ) 
    :  m_SimilarLinesCount( aSimilarLinesCount )
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
    const uint16_t lColorIndex( ( aLineNumber / m_SimilarLinesCount ) % m_Colors.size() );
    const ColorName lColor( m_Colors[ lColorIndex ] );
    aResContainer.putMarker( 0, lColor );
    aResContainer.putMarker( aLine.size(), lColor );
}


} // namespace Color
