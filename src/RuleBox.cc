#include "RuleBox.hh"

#include <stack>
namespace Color {

RuleBox::RuleBox()
{
    ;
}

void RuleBox::addRule( const IRule::Ptr& aRule )
{
    m_Rules.push_front( aRule );
}

RuleBox& RuleBox::addBox( const RuleBox& aBox )
{
    for( auto lRule : aBox.m_Rules )
    {
        addRule( lRule );
    }

    return *this;
}

RuleBox RuleBox::operator+( const RuleBox& aBox ) const
{
    RuleBox lTempRes;
    for ( auto lRule : aBox.m_Rules )
        lTempRes.addRule( lRule );
    for ( auto lRule : m_Rules )
        lTempRes.addRule( lRule );
    return lTempRes;
}

std::string RuleBox::process( const std::string& aText
        , const uint64_t aLineNumber ) const
{
    auto lIResult( applyRules( aText, aLineNumber ) );
    std::stringstream lOutput;
    std::list< ColorName > lColors;
    ColorName lCurrentColor( RESET );
    for ( size_t lIndex( 0 ) ; lIndex < aText.size() ; ++lIndex )
    {
        processLetter( lIndex, lColors, lOutput, lIResult, lCurrentColor );
        lOutput << aText[ lIndex ];
    }
    if ( !lColors.empty() ) // loop might omit last sign
        lOutput << COLOR_NAMES[ RESET ];
    return lOutput.str();
}

IntermediateResult RuleBox::applyRules( const std::string& aText
                , const size_t aLineNumber ) const
{
    IntermediateResult lIResult( aText.size() );
    auto lProcessor ( std::bind( &RuleBox::singleProcess
                    , this
                    , std::placeholders::_1
                    , std::ref( lIResult )
                    , aText
                    , aLineNumber )
                   );
    std::for_each( begin( m_Rules ), end( m_Rules ), lProcessor );
    return lIResult;
}

void RuleBox::singleProcess( const IRule::Ptr& aRule
                            , IntermediateResult& aResult
                            , const std::string& aLine
                            , const uint64_t aLineNumber ) const
{
    aRule->apply( aLine, aResult, aLineNumber );
}

void RuleBox::processLetter( const size_t aIndex
                , std::list< ColorName >& aColors
                , std::ostream& aOutput
                , const IntermediateResult& aIResult
                , ColorName& aCurrentColor ) const
{
    IntermediateResult::Markers lMarkers;
    aIResult.getMarkers( aIndex, lMarkers );
    bool lReset( false );
    if ( !lMarkers.empty() )
    {
        auto lMarkerProcessor( 
                std::bind( 
                    &RuleBox::processMarkerForLetter
                    , this
                    , std::placeholders::_1
                    , std::ref( aColors )
                    , std::ref( lReset )
                    , std::ref( aCurrentColor ) ) );
        std::for_each( begin( lMarkers ), end( lMarkers ), lMarkerProcessor );
        updateCurrentColor( aOutput, lReset, aCurrentColor, aColors.front() );
    }
}

void RuleBox::removeFirstOccurence( const ColorName aColor
                            , std::list< ColorName >& aColors ) const
{
    for ( auto lColIt( aColors.begin() )
            ; lColIt != aColors.end()
            ; ++lColIt )
    {
        if ( *lColIt == aColor )
        {
            aColors.erase( lColIt );
            return;
        }
    }
}

void RuleBox::processMarkerForLetter( 
                const IntermediateResult::Marker& aMarker
                , std::list< ColorName >& aColors
                , bool& aReset
                , ColorName& aCurrentColor ) const
{
    if ( !aMarker.first )
    {
        aColors.push_front( aMarker.second );
    }
    else
    {
        removeFirstOccurence( aMarker.second
                , aColors );
        if ( aColors.empty() )
        {
            aReset = true;
            aCurrentColor = RESET;
        }
    }
}

void RuleBox::updateCurrentColor( std::ostream& aOutput
                , bool& aReset
                , ColorName& aCurrentColor
                , const ColorName& aTopColor ) const
{
    if ( aReset )
    {
        aOutput << COLOR_NAMES[ RESET ];
        aReset = false;
    }
    else if ( aCurrentColor != aTopColor )
    {
        aOutput << COLOR_NAMES[ aTopColor ];
    }
    aCurrentColor = aTopColor;
}
} // namespace Color
