#include "Rules.hh"
#include "RuleBox.hh"
#include "Config.hh"

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
        if ( lSearchRes.size() > 1 )
        {
            for ( size_t i = 1; i < lSearchRes.size(); ++i )
            {
                aResContainer.putMarker( lGlobal + std::distance(lStart, lSearchRes[i].first), m_Color );
                aResContainer.putMarker( lGlobal + std::distance(lStart, lSearchRes[i].second), m_Color );
            }
        }
        else
        {
            aResContainer.putMarker( lGlobal + lSearchRes.position(), m_Color );
            aResContainer.putMarker( lGlobal + lSearchRes.position() + lSearchRes.length(), m_Color );
        }
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

RuleGroup::RuleGroup( const RuleBox& aRuleBox )
{
    for( auto lRule : aRuleBox.getRules() )
        m_Rules.push_back( lRule );
}

void RuleGroup::apply( const std::string& aLine
                     , IntermediateResult& aResContainer
                     , uint64_t aLineNumber ) const
{
    for ( auto lRule : m_Rules )
        lRule->apply( aLine, aResContainer, aLineNumber );
}

ReferenceRule::ReferenceRule( const std::string& aSchemeName
                            , const Config& aConfig )
                : m_SchemeName( aSchemeName )
                , m_Config( aConfig )
                , m_InvestigatedString( NULL )
{
}

void ReferenceRule::apply( const std::string& aLine
                         , IntermediateResult& aResContainer
                         , uint64_t aLineNumber ) const
{
    checkLoops( aLine );
    auto lBox( m_Config.getRuleBox( m_SchemeName ) );
    for ( auto lRule : lBox->getRules() )
        lRule->apply( aLine, aResContainer, aLineNumber );
}


void ReferenceRule::checkLoops( const std::string& aLine ) const
{
    // heuristic algorithm
    // it's actually pretty hard to do this with exact solution
    // (ok, in fact it's easy but I don't want to waste time)
    static uint64_t lSimCounter = 0;

    if ( m_InvestigatedString != aLine.c_str() )
    {
        m_InvestigatedString = aLine.c_str();
        m_InvestigatedStringCopy = aLine;
        lSimCounter = 0;
    }
    else if ( m_InvestigatedStringCopy == aLine 
            && lSimCounter++ > 1000 )
        throw std::runtime_error( "It seems that there is a loop "
                             "in dependencies between schemes" );
}

} // namespace Color
